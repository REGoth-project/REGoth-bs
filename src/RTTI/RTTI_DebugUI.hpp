#pragma once

#include "RTTIUtil.hpp"
#include <components/DebugUI.hpp>

namespace REGoth
{
  class RTTI_DebugUI
      : public bs::RTTIType<DebugUI, UIElement, RTTI_DebugUI>
  {
    BS_BEGIN_RTTI_MEMBERS
    // This class should not be serialized
    BS_END_RTTI_MEMBERS

  public:
    RTTI_DebugUI()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(DebugUI)
  };

}  // namespace REGoth
