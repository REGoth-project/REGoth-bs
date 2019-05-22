#include "ScriptState.hpp"
#include <RTTI/RTTI_ScriptState.hpp>
#include <Scene/BsSceneObject.h>
#include <components/Character.hpp>
#include <components/CharacterAI.hpp>
#include <components/CharacterEventQueue.hpp>
#include <components/GameClock.hpp>
#include <components/GameWorld.hpp>
#include <scripting/ScriptVMForGameWorld.hpp>

namespace REGoth
{
  namespace AI
  {
    /**
     * Valid script-states an NPC can be in.
     */
    const bs::String s_EnabledPlayerStates[] = {
        "ZS_ASSESSMAGIC", "ZS_ASSESSSTOPMAGIC", "ZS_MAGICFREEZE", "ZS_WHIRLWIND",
        "ZS_SHORTZAPPED", "ZS_ZAPPED",          "ZS_PYRO",        "ZS_MAGICSLEEP",
    };

    /**
     * Names for the native states passed to the scripts.
     */
    const bs::String s_NativeStateNames[] = {
        "INVALID_PRG", "ZS_ANSWER", "ZS_DEAD", "ZS_UNCONSCIOUS", "ZS_FADEAWAY", "ZS_FOLLOW",
    };

    ScriptState::ScriptState(::REGoth::HGameWorld world, ::REGoth::HCharacter hostCharacter,
                             ::REGoth::HCharacterEventQueue hostEventQueue,
                             ::REGoth::HCharacterAI hostAI)
        : mWorld(world)
        , mHostCharacter(hostCharacter)
        , mHostEventQueue(hostEventQueue)
        , mHostAI(hostAI)
    {
    }

    ScriptState::~ScriptState()
    {
    }

    void ScriptState::interruptActiveState()
    {
      mCurrentState.phase = AIState::Phase::Interrupt;

      // Set to invalid so the queued state can take over in the next cycle
      mCurrentState.isValid = false;

      // Non-Player Characters can use any state. But the Player character has only
      // a limited subset. It is unclear who would start a state that is invalid on
      // the player however. Also, shouldn't that check be done much earlier?
      if (!mHostCharacter->isPlayer() || canPlayerUseAIState(mNextState))
      {
        // FIXME: I think we need to also kill the active route the character is traveling on
        mHostEventQueue->clear();
      }
    }

    void ScriptState::requestEndActiveState()
    {
      // Let the state exit normally over the next cycles
      mCurrentState.phase = AIState::Phase::End;
    }

    void ScriptState::startNativeAIState(NativeState state)
    {
      bs::UINT32 index = (bs::UINT32)state;

      // Save script variables used by the state
      mStateOther  = scriptVM().otherInstance();
      mStateVictim = scriptVM().victimInstance();
      mStateItem   = scriptVM().itemInstance();

      // Save name to the main function, to which we can append the tags like _LOOP and _END
      mNextState.name        = s_NativeStateNames[index];
      mNextState.nativeState = state;

      fillStateScriptFunctions(mNextState);

      // Native states can never be routine states
      mNextState.isRoutineState = false;

      mNextState.isValid = true;
    }

    void ScriptState::startScriptAIState(const bs::String& state)
    {
      // Save script variables used by the state
      mStateOther  = scriptVM().otherInstance();
      mStateVictim = scriptVM().victimInstance();
      mStateItem   = scriptVM().itemInstance();

      // Save name to the main function, to which we can append the tags like _LOOP and _END
      mNextState.name        = state;
      mNextState.nativeState = NativeState::ScriptBased;

      fillStateScriptFunctions(mNextState);

      // Script states CAN be routine states, so while this is set to false here, it will
      // be set to true in startRoutineState() which calls this method first
      mNextState.isRoutineState = false;

      mNextState.isValid = true;
    }

    void ScriptState::startRoutineState(const bs::String& state)
    {
      startScriptAIState(state);

      mNextState.isRoutineState = true;
    }

    void ScriptState::runInstruction(const bs::String& instruction)
    {
      // Disable routine-flag for this, since scripts could let the npc assess something
      bool oldIsRoutineState       = mCurrentState.isRoutineState;
      mCurrentState.isRoutineState = false;

      scriptVM().runFunctionOnSelf(instruction, mHostCharacter);

      mCurrentState.isRoutineState = oldIsRoutineState;
    }

    void ScriptState::fillStateScriptFunctions(AIState& state)
    {
      const bs::String end       = state.name + "_END";
      const bs::String loop      = state.name + "_LOOP";
      const bs::String interrupt = state.name + "_INTERRUPT";
      const auto& symbols        = scriptVM().scriptSymbolsConst();

      // No check whether this exists here, let getSymbol throw if the main-function does not exist
      mNextState.symIndex = symbols.getSymbol<Scripting::SymbolScriptFunction>(state.name).index;

      // End, Loop and Interrupt are optional
      if (symbols.hasSymbolWithName(end))
      {
        const auto& symbol = symbols.getSymbol<Scripting::SymbolScriptFunction>(end);
        mNextState.symEnd  = symbol.index;
      }

      if (symbols.hasSymbolWithName(loop))
      {
        const auto& symbol = symbols.getSymbol<Scripting::SymbolScriptFunction>(loop);
        mNextState.symLoop = symbol.index;
      }

      if (symbols.hasSymbolWithName(interrupt))
      {
        const auto& symbol      = symbols.getSymbol<Scripting::SymbolScriptFunction>(interrupt);
        mNextState.symInterrupt = symbol.index;
      }
    }

    bool ScriptState::applyStateChange()
    {
      return doAIState(0.0f);
    }

    bool ScriptState::canPlayerUseAIState(const ScriptState::AIState& state)
    {
      if (state.isValid)
      {
        if (state.nativeState != NativeState::ScriptBased)
        {
          // States that are allowed for every NPC at any time
          return (state.nativeState == NativeState::Dead ||
                  state.nativeState == NativeState::Unconscious);
        }
        else
        {
          for (const bs::String s : s_EnabledPlayerStates)
          {
            if (state.name == s) return true;
          }
        }
      }

      return false;
    }

    bool ScriptState::doAIState(float deltaTime)
    {
      // Increase time this state is already running
      if (mCurrentState.isValid && mCurrentState.phase == AIState::Phase::Loop)
      {
        mCurrentState.timeRunning += deltaTime;
      }

      if (isInRoutine())
      {
        bs::INT32 hour   = mWorld->gameclock()->getHour();
        bs::INT32 minute = mWorld->gameclock()->getMinute();

        if (!isTimeInTaskRange(activeTask(), hour, minute))
        {
          startNewRoutineTaskMatchingTime();
        }
      }

      // Only do states if we do not have messages pending
      if (mHostEventQueue->isEmpty())
      {
        bool startNewRoutine = true;

        // Start daily routine if wanted
        if (!mRoutine.shouldStartNewRoutine)
        {
          startNewRoutine = false;
        }
        else if (!mRoutine.hasRoutine)
        {
          startNewRoutine = false;
        }
        else if (!isInRoutine())
        {
          startNewRoutine = false;
        }
        else if (!mHostCharacter->isPlayer())
        {
          startNewRoutine = false;
        }

        if (startNewRoutine)
        {
          startDailyRoutine();
        }

        if (!mCurrentState.isValid)
        {
          // Can we move to the next state?
          if (mNextState.isValid)
          {
            // Remember the last state we were in
            mLastStateSymIndex = mCurrentState.symIndex;

            // Move to next state
            mCurrentState = mNextState;

            mCurrentState.timeRunning = 0.0f;

            mNextState.isValid = false;
          }
          else
          {
            // No active state at all, start routine
            startDailyRoutine();
          }
        }

        // If this is the player, only allow states the player is allowed to have
        if (mHostCharacter->isPlayer() && !canPlayerUseAIState(mCurrentState)) return false;

        if (mCurrentState.isValid)
        {
          scriptVM().setOther(mStateOther);
          scriptVM().setVictim(mStateVictim);
          scriptVM().setItem(mStateItem);

          if (mCurrentState.phase == AIState::Phase::Uninitialized)
          {
            // TODO: Set perception-time to 5000

            if (mCurrentState.symIndex != Scripting::SYMBOL_INDEX_INVALID)
            {
              scriptVM().runFunctionOnSelf(mCurrentState.symIndex, mHostCharacter);
            }

            // Now do the looping function every frame
            mCurrentState.phase = AIState::Phase::Loop;
          }
          else if (mCurrentState.phase == AIState::Phase::Loop)
          {
            bool end = true;

            // Call looping-function
            if (mCurrentState.symLoop != Scripting::SYMBOL_INDEX_INVALID)
            {
              end = scriptVM().runStateLoopFunction(mCurrentState.symLoop, mHostCharacter);
            }

            // Run a program based state
            if (mCurrentState.nativeState != NativeState::ScriptBased)
            {
              // Only CheckUnconscious() is called here in the original.
              //  There is also a state for following, but it doesn't do anything here
              switch (mCurrentState.nativeState)
              {
                  // case NPC_PRGAISTATE_ANSWER:break;
                  // case NPC_PRGAISTATE_DEAD:break;
                case NativeState::Unconscious:
                  mHostCharacter->handleUnconsciousness();
                  break;
                // case NPC_PRGAISTATE_FADEAWAY:break;
                // case NPC_PRGAISTATE_FOLLOW:break;
                default: /* TODO handle all missing states  */
                  break;
              }
            }

            // Check if we're done and remove the state in the next frame
            if (end)
            {
              mCurrentState.phase = AIState::Phase::End;
            }
          }
          else if (mCurrentState.phase == AIState::Phase::End)
          {
            // Call end-function
            if (mCurrentState.symEnd != Scripting::SYMBOL_INDEX_INVALID)
            {
              scriptVM().runFunctionOnSelf(mCurrentState.symEnd, mHostCharacter);
            }

            // Invalidate the state and get it ready for the next one
            mCurrentState.phase   = AIState::Phase::Interrupt;
            mCurrentState.isValid = false;
          }
        }
      }

      return true;
    }

    void ScriptState::doAIStateDuringShrink()
    {
      bs::INT32 hour   = mWorld->gameclock()->getHour();
      bs::INT32 minute = mWorld->gameclock()->getMinute();

      if (mRoutine.hasRoutine)
      {
        bool isDoingScriptState = mCurrentState.isValid || mNextState.isValid;

        if (!isDoingScriptState || isInRoutine())
        {
          // If the characters are too far away from the camera, they will just be
          // teleported to the position they should be at according to their currently
          // active routine state. This is why Diego will be already in the old-camp
          // when you reach it, even though you just saw him walking very slowly towards it.
          if (!isTimeInTaskRange(activeTask(), hour, minute))
          {
            startNewRoutineTaskMatchingTime();
          }

          if (mRoutine.shouldStartNewRoutine)
          {
            // Force-start the new task so the character won't be stuck moving to its
            // current routine task target
            if (startDailyRoutine(true))
            {
              if (!activeTask().waypoint.empty())
              {
                mHostAI->teleport(activeTask().waypoint);
              }
            }
          }
        }
      }
    }

    bool ScriptState::startDailyRoutine(bool force)
    {
      // Just say "yes" for the player, as he never has any routine
      if (mHostCharacter->isPlayer()) return true;

      // Turn off, so we don't recurse
      mRoutine.shouldStartNewRoutine = false;

      // Start new state
      bool wasStarted = activateRoutineState(force);

      mRoutine.shouldStartNewRoutine = !wasStarted;

      return wasStarted;
    }

    bool ScriptState::isInRoutine()
    {
      if (!mRoutine.hasRoutine) return false;

      if (mCurrentState.isValid)
      {
        if (mCurrentState.isRoutineState)
        {
          return true;
        }
      }
      else
      {
        if (!mNextState.isValid)
        {
          return true;
        }
      }

      return false;
    }

    bool ScriptState::activateRoutineState(bool force)
    {
      // Player can't have routines
      if (mHostCharacter->isPlayer()) return false;

      // Check for death, etc
      if (!mHostCharacter->isReady()) return false;

      const RoutineTask& task = activeTask();

      if (mRoutine.hasRoutine)
      {
        // No routine present?
        if (mRoutine.routine.empty()) return false;

        if (!force)
        {
          if (!mHostEventQueue->isEmpty()) return false;  // EM not empty, don't start routine yet!

          // Check if the routine is already running
          if (mCurrentState.name == task.scriptFunction) return true;

          if (!isInRoutine()) return false;  // Currently in other state...
        }

        // Set current waypoint of script instance
        mHostCharacter->setCurrentWaypoint(task.waypoint);

        startRoutineState(task.scriptFunction);
        requestEndActiveState();

        return applyStateChange();
      }
      else if (mHostCharacter->isStateDriven())
      {
        if (force)
        {
          // Make it look like we don't have anything running now
          mNextState.isValid    = false;
          mCurrentState.isValid = false;
        }

        // Don't make a new state if we already have something running
        if (mCurrentState.isValid || mNextState.isValid) return false;

        startScriptAIState(mHostCharacter->getStartAIState());
        requestEndActiveState();

        return applyStateChange();
      }

      return false;
    }

    void ScriptState::insertRoutineTask(const RoutineTask& task)
    {
      mRoutine.routine.push_back(task);

      RoutineTask& addedTask = mRoutine.routine.back();

      // Some task definitions use XXX to say that the character should stay where it is.
      // This is defined inside gothics scripts, nothing we can do about it.
      if (addedTask.waypoint == "XXX")
      {
        // Replace the XXX with the empty string to denote that there should not be a target
        // waypoint for this task.
        addedTask.waypoint = "";
      }

      // FIXME: HACK, let the npc teleport to the first entry of the routine
      if (!mRoutine.hasRoutine)
      {
        if (!addedTask.waypoint.empty())
        {
          mHostAI->teleport(addedTask.waypoint);
        }
      }

      mRoutine.hasRoutine = true;  // At least one routine-target present
    }

    void ScriptState::reinitRoutine()
    {
      const bs::String& routine = mHostCharacter->dailyRoutine();

      // Clear old routine
      mRoutine.routine.clear();
      mRoutine.activeRoutineIndex = 0;

      if (!routine.empty())
      {
        scriptVM().runFunctionOnSelf(routine, mHostCharacter);
      }
    }

    bool ScriptState::isInState(const bs::String& state)
    {
      if (mCurrentState.isValid)
      {
        if (mCurrentState.name == state) return true;
      }
      else if (mNextState.isValid)
      {
        if (mNextState.name == state) return true;
      }

      return false;
    }

    void ScriptState::clearRoutine()
    {
      mRoutine.hasRoutine = false;
      mRoutine.routine.clear();
    }

    void ScriptState::setCurrentStateTime(float time)
    {
      if (mCurrentState.isValid)
      {
        mCurrentState.timeRunning = time;
      }
    }

    Scripting::ScriptVMForGameWorld& ScriptState::scriptVM()
    {
      return mWorld->scriptVM();
    }

    ScriptState::RoutineTask& ScriptState::activeTask()
    {
      return mRoutine.routine[mRoutine.activeRoutineIndex];
    }

    bool ScriptState::isTimeInTaskRange(const RoutineTask& task, bs::INT32 hours, bs::INT32 minutes)
    {
      auto tbigger = [&](int h1, int m1, int h2, int m2) {
        return h1 > h2 || (h1 == h2 && m1 > m2);
      };

      auto tsmaller = [&](int h1, int m1, int h2, int m2) {
        return h1 < h2 || (h1 == h2 && m1 < m2);
      };

      auto trange = [&](int h1, int m1, int h, int m, int h2, int m2) {
        return tbigger(h, m, h1, m1) && tsmaller(h, m, h2, m2);
      };

      bool crossesZero = task.hoursEnd < task.hoursStart;

      if (!crossesZero)
        return trange(task.hoursStart, task.minutesStart, hours, minutes, task.hoursEnd,
                      task.minutesEnd);
      else
      {
        return trange(task.hoursStart, task.minutesStart, hours, minutes, 24, 0) &&
               trange(0, 0, hours, minutes, task.hoursEnd, task.minutesEnd);
      }
    }

    void ScriptState::startNewRoutineTaskMatchingTime()
    {
      bs::UINT32 i     = 0;
      bs::INT32 hour   = mWorld->gameclock()->getHour();
      bs::INT32 minute = mWorld->gameclock()->getMinute();

      for (RoutineTask& e : mRoutine.routine)
      {
        // Don't start the same routine again
        if (i == mRoutine.activeRoutineIndex)
        {
          if (isTimeInTaskRange(e, hour, minute))
          {
            mRoutine.activeRoutineIndex    = i;
            mRoutine.shouldStartNewRoutine = true;

            return;
          }
        }

        i++;
      }
    }

    REGOTH_DEFINE_RTTI(ScriptState)

    using RoutineTask = ScriptState::RoutineTask;
    REGOTH_DEFINE_RTTI(RoutineTask)
  }  // namespace AI
}  // namespace REGoth
