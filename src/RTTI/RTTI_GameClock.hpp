#pragma once

#include "RTTIUtil.hpp"
#include <components/GameClock.hpp>

namespace REGoth
{
  class RTTI_GameClock : public bs::RTTIType<GameClock, bs::Component, RTTI_GameClock>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_PLAIN(mElapsedSeconds, 0)
    BS_RTTI_MEMBER_PLAIN(mElapsedIngameSeconds, 1)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_GameClock()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(GameClock)
  };

}  // namespace REGoth
