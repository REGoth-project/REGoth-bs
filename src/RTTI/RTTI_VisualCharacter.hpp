#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <components/VisualCharacter.hpp>

namespace REGoth
{
  class RTTI_VisualCharacter
      : public bs::RTTIType<VisualCharacter, bs::Component, RTTI_VisualCharacter>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_VisualCharacter()
    {
    }

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return bs::GameObjectRTTI::createGameObject<VisualCharacter>();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "VisualCharacter";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_VisualCharacter;
    }
  };

}  // namespace REGoth
