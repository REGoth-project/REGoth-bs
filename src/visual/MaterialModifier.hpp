#pragma once

#include <BsPrerequisites.h>

#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class MaterialModifier : public bs::IReflectable
  {
  public:
    virtual void apply(bs::HMaterial material) = 0;

    virtual bs::String getIdentifier() const = 0;

  public:
    REGOTH_DECLARE_RTTI(MaterialModifier);
  };
}  // namespace REGoth