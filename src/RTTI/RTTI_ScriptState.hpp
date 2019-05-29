#pragma once

#include "RTTIUtil.hpp"
#include <AI/ScriptState.hpp>

namespace REGoth
{
  namespace AI
  {
    class RTTI_RoutineTask
        : public bs::RTTIType<ScriptState::RoutineTask, bs::IReflectable, RTTI_RoutineTask>
    {
      using RoutineTask = ScriptState::RoutineTask;

      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(hoursStart, 0)
      BS_RTTI_MEMBER_PLAIN(minutesStart, 1)
      BS_RTTI_MEMBER_PLAIN(hoursEnd, 2)
      BS_RTTI_MEMBER_PLAIN(minutesEnd, 3)
      BS_RTTI_MEMBER_PLAIN(scriptFunction, 4)
      BS_RTTI_MEMBER_PLAIN(waypoint, 5)
      BS_END_RTTI_MEMBERS

    public:
      RTTI_RoutineTask()
      {
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(RoutineTask)
    };

    class RTTI_ScriptState : public bs::RTTIType<AI::ScriptState, bs::IReflectable, RTTI_ScriptState>
    {
      using UINT32 = bs::UINT32;

      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_symIndex, mCurrentState.symIndex, 0)
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_symLoop, mCurrentState.symLoop, 1)
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_symEnd, mCurrentState.symEnd, 2)
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_symInterrupt, mCurrentState.symInterrupt, 3)
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_phase, mCurrentState.phase, 4)
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_isValid, mCurrentState.isValid, 5)
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_name, mCurrentState.name, 6)
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_timeRunning, mCurrentState.timeRunning, 7)
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_nativeState, mCurrentState.nativeState, 8)
      BS_RTTI_MEMBER_PLAIN_NAMED(currentState_isRoutineState, mCurrentState.isRoutineState, 9)

      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_symIndex, mNextState.symIndex, 10)
      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_symLoop, mNextState.symLoop, 11)
      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_symEnd, mNextState.symEnd, 12)
      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_symInterrupt, mNextState.symInterrupt, 13)
      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_phase, mNextState.phase, 14)
      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_isValid, mNextState.isValid, 15)
      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_name, mNextState.name, 16)
      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_timeRunning, mNextState.timeRunning, 17)
      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_nativeState, mNextState.nativeState, 18)
      BS_RTTI_MEMBER_PLAIN_NAMED(nextState_isRoutineState, mNextState.isRoutineState, 19)

      BS_RTTI_MEMBER_PLAIN(mLastStateSymIndex, 20)
      BS_RTTI_MEMBER_REFL(mWorld, 21)
      BS_RTTI_MEMBER_REFL(mHostCharacter, 22)
      BS_RTTI_MEMBER_REFL(mHostEventQueue, 23)
      BS_RTTI_MEMBER_REFL(mHostAI, 24)
      BS_RTTI_MEMBER_PLAIN(mStateOther, 25)
      BS_RTTI_MEMBER_PLAIN(mStateVictim, 26)
      BS_RTTI_MEMBER_PLAIN(mStateItem, 27)

      BS_RTTI_MEMBER_REFL_ARRAY_NAMED(mRoutine_routine, mRoutine.routine, 28)
      BS_RTTI_MEMBER_PLAIN_NAMED(mRoutine_activeRoutineIndex, mRoutine.activeRoutineIndex, 29)
      BS_RTTI_MEMBER_PLAIN_NAMED(mRoutine_shouldStartNewRoutine, mRoutine.shouldStartNewRoutine, 30)
      BS_RTTI_MEMBER_PLAIN_NAMED(mRoutine_hasRoutine, mRoutine.hasRoutine, 31)
      BS_END_RTTI_MEMBERS

    public:
      RTTI_ScriptState()
      {
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(ScriptState)
    };
  }  // namespace AI
}  // namespace REGoth
