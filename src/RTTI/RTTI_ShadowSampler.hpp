#pragma once

#include "RTTIUtil.hpp"
#include <visual/ShadowSampler.hpp>

namespace REGoth
{
  class RTTI_ShadowSampler
      : public bs::RTTIType<ShadowSampler, bs::IReflectable, RTTI_ShadowSampler>
  {
    BS_BEGIN_RTTI_MEMBERS
    // No members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_ShadowSampler()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_ABSTRACT(ShadowSampler)
  };
}