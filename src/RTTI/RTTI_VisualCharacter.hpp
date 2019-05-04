#pragma once

#include "RTTIUtil.hpp"
#include <components/VisualCharacter.hpp>

namespace REGoth
{
  class RTTI_VisualCharacter
      : public bs::RTTIType<VisualCharacter, VisualSkeletalAnimation, RTTI_VisualCharacter>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_PLAIN_NAMED(headVisual, mBodyState.headVisual, 0)
    BS_RTTI_MEMBER_PLAIN_NAMED(bodyVisual, mBodyState.bodyVisual, 1)
    BS_RTTI_MEMBER_PLAIN_NAMED(headTextureIdx, mBodyState.headTextureIdx, 2)
    BS_RTTI_MEMBER_PLAIN_NAMED(teethTextureIdx, mBodyState.teethTextureIdx, 3)
    BS_RTTI_MEMBER_PLAIN_NAMED(bodySkinColorIdx, mBodyState.bodySkinColorIdx, 4)
    BS_RTTI_MEMBER_PLAIN_NAMED(bodyTextureIdx, mBodyState.bodyTextureIdx, 5)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_VisualCharacter()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(VisualCharacter)
  };

}  // namespace REGoth
