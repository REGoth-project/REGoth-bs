#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/VisualMorphMesh.hpp>

namespace REGoth
{
  class RTTI_VisualMorphMesh
    : public bs::RTTIType<VisualMorphMesh, bs::Component, RTTI_VisualMorphMesh>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

    public:
    RTTI_VisualMorphMesh()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<VisualMorphMesh>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "VisualMorphMesh";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_VisualMorphMesh;
    }
  };

}  // namespace REGoth
