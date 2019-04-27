#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/NeedsGameWorld.hpp>

namespace REGoth
{
  class RTTI_NeedsGameWorld
    : public bs::RTTIType<NeedsGameWorld, bs::Component, RTTI_NeedsGameWorld>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mGameWorld, 0)
    BS_END_RTTI_MEMBERS

    public:
    RTTI_NeedsGameWorld()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<NeedsGameWorld>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "NeedsGameWorld";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_NeedsGameWorld;
    }
  };

}  // namespace REGoth
