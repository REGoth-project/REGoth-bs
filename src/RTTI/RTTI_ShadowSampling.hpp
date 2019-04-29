#pragma once

#include "RTTIUtil.hpp"
#include <components/ShadowSampling.hpp>

namespace REGoth
{
  class RTTI_ShadowSampling
      : public bs::RTTIType<ShadowSampling, bs::Component, RTTI_ShadowSampling>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFLPTR(mSampler, 0)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_ShadowSampling() = default;

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(ShadowSampling)
  };
}  // namespace REGoth