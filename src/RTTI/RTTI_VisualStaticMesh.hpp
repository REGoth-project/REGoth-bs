#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/VisualStaticMesh.hpp>

namespace REGoth
{
  class RTTI_VisualStaticMesh
    : public bs::RTTIType<VisualStaticMesh, bs::Component, RTTI_VisualStaticMesh>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mRenderable, 0)
    BS_END_RTTI_MEMBERS

    public:
    RTTI_VisualStaticMesh()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<VisualStaticMesh>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "VisualStaticMesh";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_VisualStaticMesh;
    }
  };

}  // namespace REGoth
