#include "AnchoredTextLabels.h"
#include <Components/BsCCamera.h>
#include <GUI/BsCGUIWidget.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUIPanel.h>
#include <Math/BsMath.h>
#include <Scene/BsSceneObject.h>
#include <excepction/Throw.hpp>

namespace REGoth
{
  AnchoredTextLabels::AnchoredTextLabels(const bs::HSceneObject& parent)
      : Component(parent)
  {
    // Set a name for the component, so we can find it later if needed
    setName("AnchoredTextLabels");

    // A GUI widget component is required for drawing the labels
    mGui = parent->getComponent<bs::CGUIWidget>();

    if (mGui.isDestroyed())
    {
      REGOTH_THROW(InvalidStateException,
                   bs::StringUtil::format("Scene Object {0} does not have a CGUIWidget component!",
                                          SO()->getName()));
    }
  }

  void AnchoredTextLabels::onDestroyed()
  {
    bs::GUIPanel* mainPanel = mGui->getPanel();

    for (const auto& anchoredLabel : mLabels)
    {
      mainPanel->removeElement(anchoredLabel.mLabel);
    }
  }

  void AnchoredTextLabels::addLabel(const bs::Vector3& anchorPosition, const bs::HString& text)
  {
    bs::GUILabel* label = bs::GUILabel::create(text);
    mLabels.emplace_back(AnchoredLabel{anchorPosition, label});

    bs::GUIPanel* mainPanel = mGui->getPanel();
    mainPanel->addElement(label);
  }

  void AnchoredTextLabels::update()
  {
    auto camera = mGui->getCamera();

    const bs::Transform& cameraTransform = camera->getTransform();
    const bs::Vector3& cameraPosition    = cameraTransform.getPosition();
    const bs::Vector3& cameraForward     = cameraTransform.getForward();

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

}  // namespace REGoth
