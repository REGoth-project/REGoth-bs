#pragma once

#include "RTTIUtil.hpp"
#include <components/Character.hpp>

namespace REGoth
{
  class RTTI_Character
      : public bs::RTTIType<Character, ScriptBackedBy, RTTI_Character>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Character()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Character)
  };

}  // namespace REGoth
