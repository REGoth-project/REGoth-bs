#include "GenericMaterialModifier.hpp"
#include <RTTI/RTTI_GenericMaterialModifier.hpp>

namespace REGoth
{
  template <typename T>
  struct GenericModifierTypeTrait
  {
  };

  template<typename T>
  void GenericMaterialModifier<T>::apply(bs::HMaterial material)
  {
    GenericModifierTypeTrait<T>::apply(material, mName, mValue, mIndex);
  }

  template <typename T>
  bs::RTTITypeBase* GenericMaterialModifier<T>::getRTTIStatic()
  {
    return RTTI_GenericMaterialModifier<T>::instance();
  }

  template <typename T>
  bs::RTTITypeBase* GenericMaterialModifier<T>::getRTTI() const
  {
    return GenericMaterialModifier<T>::getRTTIStatic();
  }

#define REGOTH_DEFINE_GENERIC_MODIFIER(type, type_tag)                                                          \
template <>                                                                                                     \
struct GenericModifierTypeTrait<type>                                                                           \
{                                                                                                               \
  inline static void apply(bs::HMaterial material, const bs::String& name, const type& value, bs::UINT32 index) \
  {                                                                                                             \
    material->set##type_tag##(name, value, index);                                                              \
  }                                                                                                             \
                                                                                                                \
  inline static bs::UINT32 getRTTITypeId()                                                                      \
  {                                                                                                             \
    return TID_REGOTH_GenericMaterialModifier##type_tag;                                                        \
  }                                                                                                             \
                                                                                                                \
  inline static bs::String getRTTIName()                                                                        \
  {                                                                                                             \
    return "GenericModifierTypeTrait##type_tag##";                                                              \
  }                                                                                                             \
};                                                                                                              \
                                                                                                                \
template class GenericMaterialModifier<type>; 

  // Define all types which can be used to modify materials here
  REGOTH_DEFINE_GENERIC_MODIFIER(bs::Vector3, Vec3)

}  // namespace REGoth