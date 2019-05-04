#pragma once

#include "RTTIUtil.hpp"
#include <components/NeedsGameWorld.hpp>

namespace REGoth
{
  class RTTI_NeedsGameWorld : public bs::RTTIType<NeedsGameWorld, bs::Component, RTTI_NeedsGameWorld>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mGameWorld, 0)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_NeedsGameWorld()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(NeedsGameWorld)
  };

}  // namespace REGoth
