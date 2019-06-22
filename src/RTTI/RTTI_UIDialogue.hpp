#pragma once

#include "RTTIUtil.hpp"
#include <components/UIDialogue.hpp>

namespace REGoth
{
  class RTTI_UIDialogue
      : public bs::RTTIType<UIDialogue, UIElement, RTTI_UIDialogue>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_UIDialogue()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(UIDialogue)
  };

}  // namespace REGoth
