#pragma once

#include "RTTIUtil.hpp"
#include <components/Debug.hpp>

namespace REGoth
{
  class RTTI_Debug : public bs::RTTIType<Debug, bs::Component, RTTI_Debug>
  {
    BS_BEGIN_RTTI_MEMBERS
    // This class should not be serialized
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Debug()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Debug)
  };

}  // namespace REGoth
