#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/Waypoint.hpp>

namespace REGoth
{
  class RTTI_Waypoint
    : public bs::RTTIType<Waypoint, bs::Component, RTTI_Waypoint>
  {
    using UINT32 = bs::UINT32;

    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL_ARRAY(mPaths, 0)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Waypoint()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<Waypoint>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "Waypoint";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_Waypoint;
    }
  };

}  // namespace REGoth
