#pragma once
#include "RTTIUtil.hpp"
#include <components/Console/Console.hpp>

namespace REGoth
{
  class RTTI_Console : public bs::RTTIType<Console, bs::Component, RTTI_Console>
  {
    BS_BEGIN_RTTI_MEMBERS
    // This class should not be serialized
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Console()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Console)
  };
}  // namespace REGoth
