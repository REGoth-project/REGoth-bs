#pragma once

#include "RTTIUtil.hpp"
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

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(CharacterKeyboardInput)
  };

}  // namespace REGoth
