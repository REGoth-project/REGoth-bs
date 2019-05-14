#pragma once

#include "RTTIUtil.hpp"
#include <components/ThirdPersonCamera.hpp>

namespace REGoth
{
  class RTTI_ThirdPersonCamera
    : public bs::RTTIType<ThirdPersonCamera, bs::Component, RTTI_ThirdPersonCamera>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mFollowedCharacter, 0)
    BS_END_RTTI_MEMBERS

    public:
    RTTI_ThirdPersonCamera()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(ThirdPersonCamera)
  };

}  // namespace REGoth
