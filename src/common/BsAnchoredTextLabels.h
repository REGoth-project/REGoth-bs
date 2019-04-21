#pragma once

#include "BsPrerequisites.h"
#include "Localization/BsHString.h"
#include "Math/BsVector3.h"
#include "Scene/BsComponent.h"

namespace bs
{
  /**
   * Component that manages a set of text labels which are anchored at 3D world positions
   * and drawn to the GUI widget component of the parent scene object.
   */
  class AnchoredTextLabels : public Component
  {
  public:
    AnchoredTextLabels(const HSceneObject& parent);

    /**
     * Adds a label with the given text at the anchor position in world coordinates.
     */
    void addLabel(const Vector3& anchorPosition, const HString& text);

    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;

  private:
    struct AnchoredLabel
    {
      Vector3 mWorldPosition;
      GUILabel* mLabel;
    };

    HGUIWidget mGui;
    Vector<AnchoredLabel> mLabels;
  };

  using HAnchoredTextLabels = GameObjectHandle<AnchoredTextLabels>;
}  // namespace bs
