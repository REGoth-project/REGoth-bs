#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/Character.hpp>

namespace REGoth
{
  class RTTI_Character
      : public bs::RTTIType<Character, bs::Component, RTTI_Character>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Character()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<Character>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "Character";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_Character;
    }
  };

}  // namespace REGoth
