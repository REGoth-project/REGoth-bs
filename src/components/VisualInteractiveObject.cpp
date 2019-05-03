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

  REGOTH_DEFINE_RTTI(VisualInteractiveObject)
}  // namespace REGoth
