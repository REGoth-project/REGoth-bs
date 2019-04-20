#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/VisualSkeletalAnimation.hpp>

namespace REGoth
{
  class RTTI_VisualSkeletalAnimation
    : public bs::RTTIType<VisualSkeletalAnimation, bs::Component, RTTI_VisualSkeletalAnimation>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

    public:
    RTTI_VisualSkeletalAnimation()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<VisualSkeletalAnimation>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "VisualSkeletalAnimation";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_VisualSkeletalAnimation;
    }
  };

}  // namespace REGoth
