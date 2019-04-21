#pragma once

#include <BsPrerequisites.h>
#include <Math/BsVector3.h>
#include <Scene/BsComponent.h>

namespace REGoth
{
  /**
   * Component that manages a set of text labels which are anchored at 3D world positions
   * and drawn to the GUI widget component of the parent scene object.
   */
  class AnchoredTextLabels : public bs::Component
  {
  public:
    AnchoredTextLabels(const bs::HSceneObject& parent);

    /**
     * Adds a label with the given text at the anchor position in world coordinates.
     */
    void addLabel(const bs::Vector3& anchorPosition, const bs::HString& text);

    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;

  private:
    struct AnchoredLabel
    {
      bs::Vector3 mWorldPosition;
      bs::GUILabel* mLabel;
    };

    bs::HGUIWidget mGui;
    bs::Vector<AnchoredLabel> mLabels;
  };

  using HAnchoredTextLabels = bs::GameObjectHandle<AnchoredTextLabels>;
}  // namespace bs
