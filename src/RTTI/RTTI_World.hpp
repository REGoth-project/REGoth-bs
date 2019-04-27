#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/World.hpp>

namespace REGoth
{
  class RTTI_World
    : public bs::RTTIType<World, bs::Component, RTTI_World>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

    public:
    RTTI_World()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<World>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "World";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_World;
    }
  };

}  // namespace REGoth
