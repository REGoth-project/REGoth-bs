#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
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

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<Item>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "Item";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_Item;
    }
  };

}  // namespace REGoth
