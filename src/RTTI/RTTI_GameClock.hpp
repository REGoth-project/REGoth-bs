#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/GameClock.hpp>

namespace REGoth
{
  class RTTI_GameClock
      : public bs::RTTIType<GameClock, bs::Component, RTTI_GameClock>
  {
    BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(mElapsedSeconds, 0)
      BS_RTTI_MEMBER_PLAIN(mElapsedIngameSeconds, 1)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_GameClock()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<GameClock>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "GameClock";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_GameClock;
    }
  };

}  // namespace REGoth
