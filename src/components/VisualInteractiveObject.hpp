#pragma once
#include "VisualSkeletalAnimation.hpp"
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>

namespace REGoth
{
  /**
   * Component for rendering an interactive object (Called Mob in the original).
   *
   * This component extends the VisualSkeletalAnimation-Component and
   * adds support for interactive object specific features.
   */
  class VisualInteractiveObject : public VisualSkeletalAnimation
  {
  public:
    VisualInteractiveObject (const bs::HSceneObject& parent);
    virtual ~VisualInteractiveObject ();

  protected:

    bs::Vector<bs::String> listPossibleDefaultAnimations() const override;

  public:
    REGOTH_DECLARE_RTTI(Character);

  protected:
    VisualInteractiveObject() = default; // For RTTI
  };

  using HVisualInteractiveObject = bs::GameObjectHandle<VisualInteractiveObject>;
}  // namespace REGoth
