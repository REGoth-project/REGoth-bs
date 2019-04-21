#include "BsAnchoredTextLabels.h"
#include "Components/BsCCamera.h"
#include "GUI/BsCGUIWidget.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIPanel.h"
#include "Math/BsMath.h"
#include "Scene/BsSceneObject.h"

namespace bs
{
  AnchoredTextLabels::AnchoredTextLabels(const HSceneObject& parent)
      : Component(parent)
  {
    // Set a name for the component, so we can find it later if needed
    setName("AnchoredTextLabels");

    // A GUI widget component is required for drawing the labels
    mGui = parent->getComponent<CGUIWidget>();
  }

  void AnchoredTextLabels::addLabel(const Vector3& anchorPosition, const HString& text)
  {
    GUILabel* label = GUILabel::create(text);
    mLabels.emplace_back(AnchoredLabel{anchorPosition, label});

    GUIPanel* mainPanel = mGui->getPanel();
    mainPanel->addElement(label);
  }

  void AnchoredTextLabels::update()
  {
    auto camera                          = mGui->getCamera();
    const Transform& cameraTransform = camera->getTransform();
    const Vector3& cameraPosition    = cameraTransform.getPosition();
    const Vector3& cameraForward     = cameraTransform.getForward();
    for (const auto& anchoredLabel : mLabels)
    {
      // Only show labels with an anchor point in front of the camera
      if ((anchoredLabel.mWorldPosition - cameraPosition).dot(cameraForward) > 0.f)
      {
        auto anchorPointOnScreen = camera->worldToScreenPoint(anchoredLabel.mWorldPosition);
        anchoredLabel.mLabel->setPosition(anchorPointOnScreen.x, anchorPointOnScreen.y);
        anchoredLabel.mLabel->setVisible(true);
      }
      else
      {
        anchoredLabel.mLabel->setVisible(false);
      }
    }
  }

}  // namespace bs
