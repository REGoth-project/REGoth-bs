#pragma once
#include "RTTIUtil.hpp"
#include <components/UIConsole.hpp>

namespace REGoth
{
  class RTTI_UIConsole
      : public bs::RTTIType<UIConsole, UIElement, RTTI_UIConsole>
  {
    BS_BEGIN_RTTI_MEMBERS
    // This class should not be serialized
    BS_END_RTTI_MEMBERS

  public:
    RTTI_UIConsole()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(UIConsole)
  };
}  // namespace REGoth
