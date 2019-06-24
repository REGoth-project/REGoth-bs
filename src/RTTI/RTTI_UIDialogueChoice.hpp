#pragma once

#include "RTTIUtil.hpp"
#include <components/UIDialogueChoice.hpp>

namespace REGoth
{
  class RTTI_UIDialogueChoice
    : public bs::RTTIType<UIDialogueChoice, UIElement, RTTI_UIDialogueChoice>
  {
    BS_BEGIN_RTTI_MEMBERS
    // This class should not be serialized
    BS_END_RTTI_MEMBERS

  public:
    RTTI_UIDialogueChoice()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(UIDialogueChoice)
  };

}  // namespace REGoth
