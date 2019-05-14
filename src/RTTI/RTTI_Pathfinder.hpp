#pragma once

#include "RTTIUtil.hpp"
#include <AI/Pathfinder.hpp>

namespace REGoth
{
  namespace AI
  {
    class RTTI_Pathfinder : public bs::RTTIType<AI::Pathfinder, bs::IReflectable, RTTI_Pathfinder>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN_NAMED(height, mUserConfiguration.height, 0)
      BS_RTTI_MEMBER_PLAIN_NAMED(radius, mUserConfiguration.radius, 1)
      BS_RTTI_MEMBER_PLAIN_NAMED(stepHeight, mUserConfiguration.stepHeight, 2)
      BS_RTTI_MEMBER_PLAIN_NAMED(maxSlopeAngle, mUserConfiguration.maxSlopeAngle, 3)
      BS_END_RTTI_MEMBERS

    public:
      RTTI_Pathfinder()
      {
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(Pathfinder)
    };
  }
  // namespace AI
}  // namespace REGoth
