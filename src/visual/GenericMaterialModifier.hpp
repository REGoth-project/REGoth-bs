#pragma once

#include <BsPrerequisites.h>
#include <Material/BsMaterial.h>

#include <RTTI/RTTIUtil.hpp>
#include <visual/MaterialModifier.hpp>

namespace REGoth
{
  template<typename T>
  class RTTI_GenericMaterialModifier;

  template <typename T>
  class GenericMaterialModifier : public MaterialModifier
  {
  public:
    GenericMaterialModifier(const bs::String& name, T value, bs::UINT32 index = 0)
        : mName(name)
        , mValue(value)
        , mIndex(index)
    {
    }

    static bs::SPtr<bs::IReflectable> createEmpty()
    {
      return bs::bs_shared_ptr(new (bs::bs_alloc<GenericMaterialModifier<T>>()) GenericMaterialModifier<T>());
    }

    virtual void apply(bs::HMaterial material);

    virtual bs::String getIdentifier() const
    {
      return mName;
    }

  private:
    bs::String mName;
    T mValue;
    bs::UINT32 mIndex;

  public:
    friend class RTTI_GenericMaterialModifier<T>;
    static bs::RTTITypeBase* getRTTIStatic();
    virtual bs::RTTITypeBase* getRTTI() const override; 

  private:
    GenericMaterialModifier() = default;
  };

}  // namespace REGoth