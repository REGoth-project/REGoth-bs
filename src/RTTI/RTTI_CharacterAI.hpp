#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/CharacterAI.hpp>

namespace REGoth
{
  class RTTI_CharacterAI
    : public bs::RTTIType<CharacterAI, bs::Component, RTTI_CharacterAI>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

    public:
    RTTI_CharacterAI()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<CharacterAI>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "CharacterAI";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_CharacterAI;
    }
  };

}  // namespace REGoth
