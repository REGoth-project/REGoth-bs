#include "VisualInteractiveObject.hpp"
#include <RTTI/RTTI_VisualInteractiveObject.hpp>

namespace REGoth
{
  VisualInteractiveObject::VisualInteractiveObject(const bs::HSceneObject& parent)
      : VisualSkeletalAnimation(parent)
  {
    setName("VisualInteractiveObject");
  }

  VisualInteractiveObject::~VisualInteractiveObject()
  {
  }

  bs::Vector<bs::String> VisualInteractiveObject::listPossibleDefaultAnimations() const
  {
    return {"S_S0"};
  }

  bs::RTTITypeBase* VisualInteractiveObject::getRTTIStatic()
  {
    return RTTI_VisualInteractiveObject::instance();
  }

  bs::RTTITypeBase* VisualInteractiveObject::getRTTI() const
  {
    return VisualInteractiveObject::getRTTIStatic();
  }
}  // namespace REGoth
