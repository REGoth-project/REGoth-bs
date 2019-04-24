#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/VisualInteractiveObject.hpp>

namespace REGoth
{
  class RTTI_VisualInteractiveObject
      : public bs::RTTIType<VisualInteractiveObject, VisualSkeletalAnimation,
                            RTTI_VisualInteractiveObject>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_VisualInteractiveObject()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<VisualInteractiveObject>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "VisualInteractiveObject";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_VisualInteractiveObject;
    }
  };

}  // namespace REGoth
