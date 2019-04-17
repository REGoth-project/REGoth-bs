#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/CharacterKeyboardInput.hpp>

namespace REGoth
{
  class RTTI_CharacterKeyboardInput
    : public bs::RTTIType<CharacterKeyboardInput, bs::Component, RTTI_CharacterKeyboardInput>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

    public:
    RTTI_CharacterKeyboardInput()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<CharacterKeyboardInput>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "CharacterKeyboardInput";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_CharacterKeyboardInput;
    }
  };

}  // namespace REGoth
