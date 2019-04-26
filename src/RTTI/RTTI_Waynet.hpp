#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
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

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<Waynet>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "Waynet";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_Waynet;
    }
  };

}  // namespace REGoth
