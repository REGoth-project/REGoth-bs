#pragma once
#include "RTTIUtil.hpp"
#include <components/UIInventory.hpp>

namespace REGoth
{
  class RTTI_UIInventory : public bs::RTTIType<UIInventory, UIElement, RTTI_UIInventory>
  {
    BS_BEGIN_RTTI_MEMBERS
    // This class should not be serialized
    BS_END_RTTI_MEMBERS

  public:
    RTTI_UIInventory()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(UIInventory)
  };
}  // namespace REGoth
