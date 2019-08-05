#pragma once

#include "RTTIUtil.hpp"
#include <components/CharacterAI.hpp>

namespace REGoth
{
  class RTTI_CharacterAI : public bs::RTTIType<CharacterAI, bs::Component, RTTI_CharacterAI>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mVisual, 0)
    BS_RTTI_MEMBER_REFL(mCharacterController, 1)
    BS_RTTI_MEMBER_PLAIN(mIsPhysicsActive, 2)
    BS_RTTI_MEMBER_REFL(mWorld, 3)
    BS_RTTI_MEMBER_PLAIN(mWalkMode, 4)
    BS_RTTI_MEMBER_PLAIN(mWeaponMode, 5)
    BS_RTTI_MEMBER_PLAIN(mFallingVelocity, 6)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_CharacterAI()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(CharacterAI)
  };

}  // namespace REGoth
