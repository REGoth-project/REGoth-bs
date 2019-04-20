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

  private:

    /************************************************************************/
    /* RTTI                                                                 */
    /************************************************************************/
  public:
    friend class RTTI_VisualInteractiveObject ;
    static bs::RTTITypeBase* getRTTIStatic();
    bs::RTTITypeBase* getRTTI() const override;

    // protected:
  public:  // FIXME: Should be protected, it is only used by RTTI but friend doesn't seem to work?!
    VisualInteractiveObject () = default;  // Serialization only
  };
}  // namespace REGoth
