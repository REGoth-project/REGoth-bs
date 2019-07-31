#pragma once
#include "RTTIUtil.hpp"
#include <components/Inventory.hpp>

namespace REGoth
{
  class RTTI_Inventory
      : public bs::RTTIType<Inventory, bs::Component, RTTI_Inventory>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_PLAIN(mItemCountsByInstancesByInstance, 0)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Inventory()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Inventory)
  };
}  // namespace REGoth
