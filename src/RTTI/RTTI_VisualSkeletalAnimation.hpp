#pragma once

#include "RTTIUtil.hpp"
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

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(VisualSkeletalAnimation)
  };

}  // namespace REGoth
