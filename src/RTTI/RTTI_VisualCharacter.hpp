#pragma once

#include "RTTIUtil.hpp"
#include <components/VisualCharacter.hpp>

namespace REGoth
{
  class RTTI_VisualCharacter
      : public bs::RTTIType<VisualCharacter, VisualSkeletalAnimation, RTTI_VisualCharacter>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_VisualCharacter()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(VisualCharacter)
  };

}  // namespace REGoth
