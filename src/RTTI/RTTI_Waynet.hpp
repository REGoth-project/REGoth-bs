#pragma once

#include "RTTIUtil.hpp"
#include <components/Waynet.hpp>

namespace REGoth
{
  class RTTI_Waynet : public bs::RTTIType<Waynet, bs::Component, RTTI_Waynet>
  {
    using UINT32 = bs::UINT32;

    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL_ARRAY(mWaypoints, 0)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Waynet()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Waynet)
  };

}  // namespace REGoth
