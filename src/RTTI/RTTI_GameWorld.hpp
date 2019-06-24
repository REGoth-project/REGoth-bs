#pragma once

#include "RTTIUtil.hpp"
#include <components/GameWorld.hpp>

namespace REGoth
{
  class RTTI_GameWorld
    : public bs::RTTIType<GameWorld, bs::Component, RTTI_GameWorld>
  {
    using UINT32 = bs::UINT32;

    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_PLAIN(mZenFile, 0)
    BS_RTTI_MEMBER_REFL(mWaynet, 1)
    BS_RTTI_MEMBER_REFLPTR(mScriptVM, 2)
    BS_RTTI_MEMBER_PLAIN(mIsInitialized, 3)
    BS_RTTI_MEMBER_REFL(mGameClock, 4)
    BS_RTTI_MEMBER_REFL_ARRAY(mAllCharacters, 5)
    BS_RTTI_MEMBER_REFL_ARRAY(mAllItems, 6)
    BS_END_RTTI_MEMBERS

    public:
    RTTI_GameWorld()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(GameWorld)
  };

}  // namespace REGoth
