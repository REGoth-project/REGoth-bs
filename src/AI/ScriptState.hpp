#pragma once
#include <BsCorePrerequisites.h>
#include <scripting/ScriptTypes.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class ScriptVMForGameWorld;
  }

  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  class CharacterAI;
  using HCharacterAI = bs::GameObjectHandle<CharacterAI>;

  class CharacterEventQueue;
  using HCharacterEventQueue = bs::GameObjectHandle<CharacterEventQueue>;

  namespace AI
  {
    /**
     * States used by the native game code. Called Program based AI-states in the
     * original engine, or short "PRG-States". This has to match up with the scripts
     * definition of PRG-States.
     *
     * The `ScriptBased`-state will execute script functions instead of the ones
     * defined by the engine.
     */
    enum class NativeState : bs::UINT32
    {
      ScriptBased = 0,
      Answer      = 1,
      Dead        = 2,
      Unconscious = 3,
      Fadeaway    = 4,
      Follow      = 5,
      Num         = 6
    };

    /**
     * This class handles the *Script-State* of a character.
     *
     * There are two major modes in this:
     *
     *  1. Do the daily routine
     *  2. Run some Script-defined state
     *
     * A script defined state could be something like `ZS_TALK`.
     *
     *
     * How a Script-State Looks
     * ========================
     *
     * A script state can have at most 4 script functions which are executed for several
     * reasons. For the state `ZS_TALK`, there could be the following script functions:
     *
     *  1. Main-function      (`ZS_TALK`)
     *  2. Loop-function      (`ZS_TALK_LOOP`, return BOOL)
     *  3. End-function       (`ZS_TALK_END`)
     *  4. Interrupt-function (`ZS_TALK_INTERRUPT`)
     *
     * As the names suggest, there is some looping going on. However, every function
     * except the Main-function is optional.
     *
     * In more detail, the Main-function is called as soon as the state went active. After
     * that, the Loop-function is called on every cycle, for as long as it returns FALSE.
     * Once it returns false, the End-function is called.
     *
     * If a different state is to be started while an other one is already running,
     * the running state can be interrupted. In that case, the Interrupt-function
     * is called on that state and the new state is started.
     *
     *
     * Daily Routine
     * =============
     *
     * If no script state is active, the daily routine will be played. A daily routine
     * is a set of tasks, which have a time range and a script function. Once the
     * game time is inside the range of a task, it's script function is called.
     * That script function might queue some event-messages like "Go to waypoint X",
     * then "Start cooking", then "Go to Waypoint Y".
     */
    class ScriptState
    {
    protected:
      struct AIState;

    public:
      ScriptState(HGameWorld world, HCharacter hostCharacter);
      ~ScriptState();

      /**
       * Starts a new native state. See NativeState for the list of possible states.
       * Native states are those which are not defined by the game scripts, but rather
       * the engine itself. This includes states such as `Follow` or `Unconscious`.
       *
       * Note that just starting the state may not be enough, as you need to request to end the
       * currently running state. See requestEndActiveState() amd interruptActiveState().
       *
       * @param  state  Native state to start.
       */
      void startNativeAIState(NativeState state);

      /**
       * Starts a script defined AI state, such as `ZS_TALK`.
       *
       * Note that just starting the state may not be enough, as you need to request to end the
       * currently running state. See requestEndActiveState() amd interruptActiveState().
       *
       * @param  state  Name of the state to start. This is also the name of the states main
       *                function inside the scripts, e.g. `ZS_TALK`. However, this can also
       *                be an instruction-function, like `B_SOMETHING`, which is just executed
       *                straight away. This seems weird to have here, but it's like the original
       *                is doing it.
       */
      void startScriptAIState(const bs::String& state);

      /**
       * Same as startScriptAIState(), but for states used for Routines.
       */
      void startRoutineState(const bs::String& state);

      /**
       * Runs a simple script instruction, such as `B_SOMETHING`.
       *
       * Seems like a weird place to have this here, but the original had it as well.
       *
       * @param  instruction  Script function to call, e.g. `B_SOMETHING`.
       */
      void runInstruction(const bs::String& instruction);


      /**
       * To be called after a new AI-state has just been queued. This method will interrupt
       * the currently active state and start the queued one as soon as possible;
       */
      void interruptActiveState();

      /**
       * To be called after a new AI-state has just been queued. This method will notify
       * the currently active state that it should end as soon as possible so the queued
       * state can be started.
       */
      void requestEndActiveState();

      /**
       * To be called after a new state has been started and the old one has been requested
       * to end or interrupted. This will try to switch to the new state by running the first
       * cycle.
       *
       * @return True, if the state switch was successful,
       *         False, if the currently active state did not want to end yet.
       */
      bool applyStateChange();

      /**
       * @return Index of the last states main-function
       * TODO: Does this needs to be public?
       */
      Scripting::SymbolIndex getLastState()
      {
        return m_LastStateSymIndex;
      }
      /**
       * Checks whether the PLAYER going to the given state would be valid at this time
       * @param state State to check
       * @return Whether we could go to the input state
       */
      bool canPlayerUseAIState(const AIState& state);

      /**
       * Checks if the current state is still active
       */
      bool isStateActive()
      {
        return m_CurrentState.isValid;
      }
      /**
       * Performs the actions needed for the current frame and the current state. Advances to the
       * next one, if needed
       * @param deltaTime time since last frame
       * @return Whether the state could be processed. This may be false when states are not
       * activated for this NPC.
       */
      bool doAIState(float deltaTime);

      /**
       * Starts the routine-state set for this NPC
       * @return Whether the state could be started
       */
      bool startDailyRoutine(bool force = false);

      /**
       * @return Whether this NPC currently is in a routine state or no state at all
       */
      bool isInRoutine();

      /**
       * Removes the currently set routine
       */
      void clearRoutine();

      /**
       * Checks whether this NPC is currently in the given state.
       *
       * @param  state State to check for (e.g. `ZS_TALK`)
       *
       * @return Whether the NPC is inside the given state.
       */
      bool isInState(const bs::String& state);

      /**
       * Activates the currently set routine state
       * @param force Do it, even though we are in an other state ATM
       * @return Success
       */
      bool activateRoutineState(bool force);

      struct RoutineTask
      {
        bs::INT32 hoursStart;
        bs::INT32 minutesStart;
        bs::INT32 hoursEnd;
        bs::INT32 minutesEnd;
        bs::String scriptFunction;
        bs::String waypoint;
      };

      /**
       * Inserts a new routine task for this character.
       *
       * @param  task  Task to add.
       */
      void insertRoutineTask(const RoutineTask& task);

      /**
       * @return Time the Character already is inside this state
       */
      float getCurrentStateRunningTime()
      {
        return m_CurrentState.isValid ? m_CurrentState.timeRunning : 0.0f;
      }

      /**
       * Sets the time the Character has already been in it's current state
       *
       * TODO: Do we need this? Seems odd.
       */
      void setCurrentStateTime(float time);

      /**
       * Gets the current routine function from the script instance and replaces the routine stored
       * with the new one. Must be called after the active routine has been changed via scripts.
       */
      void reinitRoutine();

    protected:

      /**
       * Quick access to the script VM
       */
      Scripting::ScriptVMForGameWorld& scriptVM();

      /**
       * Structure describing an NPC-state
       */
      struct AIState
      {
        enum class Phase : bs::INT32
        {
          Uninitialized,
          Loop,
          End,
          Interrupt,
        };

        // Index of the start-function symbol
        Scripting::SymbolIndex symIndex = Scripting::SYMBOL_INDEX_INVALID;

        // Index of the loop-function symbol
        Scripting::SymbolIndex symLoop = Scripting::SYMBOL_INDEX_INVALID;

        // Index of the end-function symbol
        Scripting::SymbolIndex symEnd = Scripting::SYMBOL_INDEX_INVALID;

        // Index of the Interrupt-function symbol
        Scripting::SymbolIndex symInterrupt = Scripting::SYMBOL_INDEX_INVALID;

        // Phase to figure out the correct script function to call
        // such as _LOOP, _END and _INTERRUPT
        Phase phase = Phase::Uninitialized;

        // Whether the script symbols defined above are valid or a native state is set.
        bool isValid;

        // Name of the state: ZS_name
        bs::String name;

        // How long this is already running
        float timeRunning;

        // User defined index to choose instead of calling a script-function.
        // If this says `ScriptBased`, the symbols defined above are used as state functions.
        NativeState nativeState;

        // Whether this is a state from a daily routine
        bool isRoutineState = false;
      };

      /**
       * Looks up the script functions for the state name inside the given script state.
       *
       * Each state is only passed the *Name* of the state to activate. That might be
       * something like `ZS_TALK`. However, this is only the script function to execute
       * on start of the state. If the state is running, we need to call `ZS_TALK_LOOP`
       * and if it ended, we need to call `ZS_STATE_END`. Therefore, we search for these
       * tagged function names and store their symbol indices for faster access.
       */
      void fillStateScriptFunctions(AIState& state);

      /**
       * @return Whether the time passed in lays in between the start- and stop times defined in
       *         the given task.
       */
      static bool isTimeInTaskRange(const RoutineTask& task, bs::INT32 hours, bs::INT32 minutes);

      /**
       * @return The currently active routine task.
       *
       * Throws if isInRoutine() is false, so better check that first.
       */
      RoutineTask& activeTask();

      // Currently executed AI-state
      AIState m_CurrentState;

      // Next AI-state in queue
      AIState m_NextState;

      // Index of the last states main-function
      Scripting::SymbolIndex m_LastStateSymIndex;

      // World this resides in
      HGameWorld mWorld;

      // Vob this belongs to
      HCharacter mHostCharacter;
      HCharacterEventQueue mHostEventQueue;
      HCharacterAI mHostAI;

      // Other/victim/item set when we started the state
      Scripting::ScriptObjectHandle m_StateOther;
      Scripting::ScriptObjectHandle m_StateVictim;
      Scripting::ScriptObjectHandle m_StateItem;

      struct
      {
        // Currently active routine.
        std::vector<RoutineTask> routine;

        // Index into the `routine`-vector for the currently active routine.
        bs::UINT32 activeRoutineIndex = 0;

        // Whether to start a new routine as soon as possible
        bool shouldStartNewRoutine = true;

        // Whether any routine has been registered yet
        bool hasRoutine = false;
      } m_Routine;
    };
  }  // namespace AI
}  // namespace REGoth
