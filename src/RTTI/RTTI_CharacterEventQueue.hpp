#pragma once
#include "RTTIUtil.hpp"
#include <components/CharacterEventQueue.hpp>

namespace REGoth
{
  class RTTI_CharacterEventQueue
      : public bs::RTTIType<CharacterEventQueue, EventQueue, RTTI_CharacterEventQueue>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mCharacter, 0)
    BS_RTTI_MEMBER_REFL(mCharacterAI, 1)
    BS_RTTI_MEMBER_REFL(mWorld, 2)
    BS_RTTI_MEMBER_REFLPTR(mPathfinder, 3)
    BS_RTTI_MEMBER_REFLPTR(mScriptState, 4)
    BS_RTTI_MEMBER_REFL(mVisualCharacter, 5)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_CharacterEventQueue()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(CharacterEventQueue)
  };
}  // namespace REGoth
