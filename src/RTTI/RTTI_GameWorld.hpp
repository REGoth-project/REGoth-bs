#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/GameWorld.hpp>

namespace REGoth
{
  class RTTI_GameWorld
    : public bs::RTTIType<GameWorld, bs::Component, RTTI_GameWorld>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_PLAIN(mZenFile, 0)
    BS_RTTI_MEMBER_REFL(mWaynet, 1)
    BS_RTTI_MEMBER_REFLPTR(mScriptVM, 2)
    BS_RTTI_MEMBER_PLAIN(mIsInitialized, 3)
    BS_END_RTTI_MEMBERS

    public:
    RTTI_GameWorld()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<GameWorld>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "GameWorld";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_GameWorld;
    }
  };

}  // namespace REGoth
