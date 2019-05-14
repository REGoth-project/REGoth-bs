#pragma once

#include "RTTIUtil.hpp"
#include <components/Waypoint.hpp>

namespace REGoth
{
  class RTTI_Waypoint : public bs::RTTIType<Waypoint, bs::Component, RTTI_Waypoint>
  {
    using UINT32 = bs::UINT32;

    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL_ARRAY(mPaths, 0)
    BS_RTTI_MEMBER_PLAIN(mIndex, 1)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Waypoint()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Waypoint)
  };

}  // namespace REGoth
