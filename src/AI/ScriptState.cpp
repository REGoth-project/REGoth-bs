#include "ScriptState.hpp"
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

    ScriptState::ScriptState(::REGoth::HGameWorld world, ::REGoth::HCharacter hostCharacter)
        : mWorld(world)
        , mHostCharacter(hostCharacter)
        , mHostEventQueue(hostCharacter->SO()->getComponent<CharacterEventQueue>())
        , mHostAI(hostCharacter->SO()->getComponent<CharacterAI>())
    {
    }

    ScriptState::~ScriptState()
    {
    }

    void ScriptState::interruptActiveState()
    {
      m_CurrentState.phase = AIState::Phase::Interrupt;

      // Set to invalid so the queued state can take over in the next cycle
      m_CurrentState.isValid = false;

      // Non-Player Characters can use any state. But the Player character has only
      // a limited subset. It is unclear who would start a state that is invalid on
      // the player however. Also, shouldn't that check be done much earlier?
      if (!mHostCharacter->isPlayer() || canPlayerUseAIState(m_NextState))
      {
        // FIXME: I think we need to also kill the active route the character is traveling on
        mHostEventQueue->clear();
      }
    }

    void ScriptState::requestEndActiveState()
    {
      // Let the state exit normally over the next cycles
      m_CurrentState.phase = AIState::Phase::End;
    }

    void ScriptState::startNativeAIState(NativeState state)
    {
      bs::UINT32 index = (bs::UINT32)state;

      // Save script variables used by the state
      m_StateOther  = scriptVM().otherInstance();
      m_StateVictim = scriptVM().victimInstance();
      m_StateItem   = scriptVM().itemInstance();

      // Save name to the main function, to which we can append the tags like _LOOP and _END
      m_NextState.name        = s_NativeStateNames[index];
      m_NextState.nativeState = state;

      fillStateScriptFunctions(m_NextState);

      // Native states can never be routine states
      m_NextState.isRoutineState = false;

      m_NextState.isValid = true;
    }

    void ScriptState::startScriptAIState(const bs::String& state)
    {
      // Save script variables used by the state
      m_StateOther  = scriptVM().otherInstance();
      m_StateVictim = scriptVM().victimInstance();
      m_StateItem   = scriptVM().itemInstance();

      // Save name to the main function, to which we can append the tags like _LOOP and _END
      m_NextState.name        = state;
      m_NextState.nativeState = NativeState::ScriptBased;

      fillStateScriptFunctions(m_NextState);

      // Script states CAN be routine states, so while this is set to false here, it will
      // be set to true in startRoutineState() which calls this method first
      m_NextState.isRoutineState = false;

      m_NextState.isValid = true;
    }

    void ScriptState::startRoutineState(const bs::String& state)
    {
      startScriptAIState(state);

      m_NextState.isRoutineState = true;
    }

    void ScriptState::runInstruction(const bs::String& instruction)
    {
      // Disable routine-flag for this, since scripts could let the npc assess something
      bool oldIsRoutineState        = m_CurrentState.isRoutineState;
      m_CurrentState.isRoutineState = false;

      scriptVM().runFunctionOnSelf(instruction, mHostCharacter);

      m_CurrentState.isRoutineState = oldIsRoutineState;
    }

    void ScriptState::fillStateScriptFunctions(AIState& state)
    {
      const bs::String end       = state.name + "_END";
      const bs::String loop      = state.name + "_LOOP";
      const bs::String interrupt = state.name + "_INTERRUPT";
      const auto& symbols        = scriptVM().scriptSymbolsConst();

      // No check whether this exists here, let getSymbol throw if the main-function does not exist
      m_NextState.symIndex = symbols.getSymbol<Scripting::SymbolScriptFunction>(state.name).index;

      // End, Loop and Interrupt are optional
      if (symbols.hasSymbolWithName(end))
      {
        const auto& symbol = symbols.getSymbol<Scripting::SymbolScriptFunction>(end);
        m_NextState.symEnd = symbol.index;
      }

      if (symbols.hasSymbolWithName(loop))
      {
        const auto& symbol  = symbols.getSymbol<Scripting::SymbolScriptFunction>(loop);
        m_NextState.symLoop = symbol.index;
      }

      if (symbols.hasSymbolWithName(interrupt))
      {
        const auto& symbol       = symbols.getSymbol<Scripting::SymbolScriptFunction>(interrupt);
        m_NextState.symInterrupt = symbol.index;
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
      if (m_CurrentState.isValid && m_CurrentState.phase == AIState::Phase::Loop)
      {
        m_CurrentState.timeRunning += deltaTime;
      }

      if (m_Routine.hasRoutine && isInRoutine())
      {
        bs::INT32 hour   = mWorld->gameclock()->getHour();
        bs::INT32 minute = mWorld->gameclock()->getMinute();

        if (!isTimeInTaskRange(activeTask(), hour, minute))
        {
          // Find next
          bs::UINT32 i = 0;

          for (RoutineTask& e : m_Routine.routine)
          {
            // Don't start the same routine again
            if (i == m_Routine.activeRoutineIndex) continue;
            if (!isTimeInTaskRange(e, hour, minute)) continue;

            m_Routine.activeRoutineIndex    = i;
            m_Routine.shouldStartNewRoutine = true;

            i++;
          }
        }
      }

      // Only do states if we do not have messages pending
      if (mHostEventQueue->isEmpty())
      {
        bool startNewRoutine = true;

        // Start daily routine if wanted
        if (!m_Routine.shouldStartNewRoutine)
        {
          startNewRoutine = false;
        }
        else if (!m_Routine.hasRoutine)
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

        if (!m_CurrentState.isValid)
        {
          // Can we move to the next state?
          if (m_NextState.isValid)
          {
            // Remember the last state we were in
            m_LastStateSymIndex = m_CurrentState.symIndex;

            // Move to next state
            m_CurrentState = m_NextState;

            m_CurrentState.timeRunning = 0.0f;

            m_NextState.isValid = false;
          }
          else
          {
            // No active state at all, start routine
            startDailyRoutine();
          }
        }

        // If this is the player, only allow states the player is allowed to have
        if (mHostCharacter->isPlayer() && !canPlayerUseAIState(m_CurrentState)) return false;

        if (m_CurrentState.isValid)
        {
          scriptVM().setOther(m_StateOther);
          scriptVM().setVictim(m_StateVictim);
          scriptVM().setItem(m_StateItem);

          if (m_CurrentState.phase == AIState::Phase::Uninitialized)
          {
            // TODO: Set perception-time to 5000

            if (m_CurrentState.symIndex != Scripting::SYMBOL_INDEX_INVALID)
            {
              scriptVM().runFunctionOnSelf(m_CurrentState.symIndex, mHostCharacter);
            }

            // Now do the looping function every frame
            m_CurrentState.phase = AIState::Phase::Loop;
          }
          else if (m_CurrentState.phase == AIState::Phase::Loop)
          {
            bool end = true;

            // Call looping-function
            if (m_CurrentState.symLoop != Scripting::SYMBOL_INDEX_INVALID)
            {
              end = scriptVM().runStateLoopFunction(m_CurrentState.symLoop, mHostCharacter);
            }

            // Run a program based state
            if (m_CurrentState.nativeState != NativeState::ScriptBased)
            {
              // Only CheckUnconscious() is called here in the original.
              //  There is also a state for following, but it doesn't do anything here
              switch (m_CurrentState.nativeState)
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
              m_CurrentState.phase = AIState::Phase::End;
            }
          }
          else if (m_CurrentState.phase == AIState::Phase::End)
          {
            // Call end-function
            if (m_CurrentState.symEnd != Scripting::SYMBOL_INDEX_INVALID)
            {
              scriptVM().runFunctionOnSelf(m_CurrentState.symEnd, mHostCharacter);
            }

            // Invalidate the state and get it ready for the next one
            m_CurrentState.phase   = AIState::Phase::Interrupt;
            m_CurrentState.isValid = false;
          }
        }
      }

      return true;
    }

    bool ScriptState::startDailyRoutine(bool force)
    {
      // Just say "yes" for the player, as he never has any routine
      if (mHostCharacter->isPlayer()) return true;

      // Turn off, so we don't recurse
      m_Routine.shouldStartNewRoutine = false;

      // Start new state
      bool wasStarted = activateRoutineState(force);

      m_Routine.shouldStartNewRoutine = !wasStarted;

      return wasStarted;
    }

    bool ScriptState::isInRoutine()
    {
      if (m_CurrentState.isValid)
      {
        if (m_CurrentState.isRoutineState)
        {
          return true;
        }
      }
      else
      {
        if (!m_NextState.isValid)
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
      if (mHostCharacter->isReady()) return false;

      const RoutineTask& task = activeTask();

      if (m_Routine.hasRoutine)
      {
        // No routine present?
        if (m_Routine.routine.empty()) return false;

        if (!force)
        {
          if (!mHostEventQueue->isEmpty()) return false;  // EM not empty, don't start routine yet!

          // Check if the routine is already running
          if (m_CurrentState.name == task.scriptFunction) return true;

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
          m_NextState.isValid    = false;
          m_CurrentState.isValid = false;
        }

        // Don't make a new state if we already have something running
        if (m_CurrentState.isValid || m_NextState.isValid) return false;

        startScriptAIState(mHostCharacter->getStartAIState());
        requestEndActiveState();

        return applyStateChange();
      }

      return false;
    }

    void ScriptState::insertRoutineTask(const RoutineTask& task)
    {
      // FIXME: HACK, let the npc teleport to the first entry of the routine
      if (!m_Routine.hasRoutine)
      {
        if (!task.waypoint.empty())
        {
          mHostAI->teleport(task.waypoint);
        }
      }

      m_Routine.hasRoutine = true;  // At least one routine-target present

      m_Routine.routine.push_back(task);
    }

    void ScriptState::reinitRoutine()
    {
      const bs::String& routine = mHostCharacter->getDailyRoutine();

      // Clear old routine
      m_Routine.routine.clear();
      m_Routine.activeRoutineIndex = 0;

      if (!routine.empty())
      {
        scriptVM().runFunctionOnSelf(routine, mHostCharacter);
      }
    }

    bool ScriptState::isInState(const bs::String& state)
    {
      if (m_CurrentState.isValid)
      {
        if (m_CurrentState.name == state) return true;
      }
      else if (m_NextState.isValid)
      {
        if (m_NextState.name == state) return true;
      }

      return false;
    }

    void ScriptState::clearRoutine()
    {
      m_Routine.hasRoutine = false;
      m_Routine.routine.clear();
    }

    void ScriptState::setCurrentStateTime(float time)
    {
      if (m_CurrentState.isValid)
      {
        m_CurrentState.timeRunning = time;
      }
    }

    Scripting::ScriptVMForGameWorld& ScriptState::scriptVM()
    {
      return mWorld->scriptVM();
    }

    ScriptState::RoutineTask& ScriptState::activeTask()
    {
      return m_Routine.routine[m_Routine.activeRoutineIndex];
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

  }  // namespace AI
}  // namespace REGoth
