#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/NodeVisuals.hpp>

namespace REGoth
{
  class RTTI_NodeVisuals
    : public bs::RTTIType<NodeVisuals, bs::Component, RTTI_NodeVisuals>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

    public:
    RTTI_NodeVisuals()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<NodeVisuals>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "NodeVisuals";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_NodeVisuals;
    }
  };

}  // namespace REGoth
