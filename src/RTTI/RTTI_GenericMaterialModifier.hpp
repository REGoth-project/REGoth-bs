#pragma once

#include "RTTIUtil.hpp"
#include <visual/GenericMaterialModifier.hpp>

namespace REGoth
{
  template <typename T>
  class RTTI_GenericMaterialModifier
      : public bs::RTTIType<GenericMaterialModifier<T>, MaterialModifier,
                            RTTI_GenericMaterialModifier<T>>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_PLAIN(mName, 1)
    BS_RTTI_MEMBER_PLAIN(mValue, 2)
    BS_RTTI_MEMBER_PLAIN(mIndex, 3)
    BS_END_RTTI_MEMBERS

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return GenericMaterialModifier<T>::createEmpty();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = GenericModifierTypeTrait<T>::getRTTIName();
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return GenericModifierTypeTrait<T>::getRTTITypeId();
    }
  };

}  // namespace REGoth
