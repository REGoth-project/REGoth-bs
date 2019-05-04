#pragma once

#include "RTTIUtil.hpp"
#include <components/Item.hpp>

namespace REGoth
{
  class RTTI_Item
    : public bs::RTTIType<Item, ScriptBackedBy, RTTI_Item>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

    public:
    RTTI_Item()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Item)
  };

}  // namespace REGoth
