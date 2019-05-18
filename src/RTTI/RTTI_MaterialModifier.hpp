#pragma once

#include "RTTIUtil.hpp"
#include <visual/MaterialModifier.hpp>

namespace REGoth
{
  class RTTI_MaterialModifier
      : public bs::RTTIType<MaterialModifier, bs::IReflectable, RTTI_MaterialModifier>
  {
    BS_BEGIN_RTTI_MEMBERS
    // No members
    BS_END_RTTI_MEMBERS

  public:
    REGOTH_IMPLEMENT_RTTI_CLASS_ABSTRACT(MaterialModifier)
  };

}  // namespace REGoth
