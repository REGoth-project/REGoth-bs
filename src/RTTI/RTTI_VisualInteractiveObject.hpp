#pragma once

#include "RTTIUtil.hpp"
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

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(VisualInteractiveObject)
  };

}  // namespace REGoth
