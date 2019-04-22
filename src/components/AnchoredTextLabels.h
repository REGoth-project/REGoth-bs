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

    void onDestroyed() override;

    /**
     * Adds a label with the given text at the anchor position in world coordinates.
     */
    void addLabel(const bs::Vector3& anchorPosition, const bs::HString& text);

    /**
     * Set the maximum distance to which labels in front of the camera are drawn.
     */
    void setMaximumDistance(float maximumDistance);

    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;

  private:
    struct AnchoredLabel
    {
      bs::Vector3 mWorldPosition;
      bs::GUILabel* mLabel;
    };

    bool isPointWithinForwardDrawDistance(const bs::Transform& cameraTransform, const bs::Vector3& point);

    bs::HGUIWidget mGui;
    bs::Vector<AnchoredLabel> mLabels;

    float mMaximumDistance;
  };

  using HAnchoredTextLabels = bs::GameObjectHandle<AnchoredTextLabels>;
}  // namespace REGoth
