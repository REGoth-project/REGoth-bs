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
  AnchoredTextLabels::AnchoredTextLabels(const bs::HSceneObject& parent, const bs::HGUIWidget& gui)
      : Component(parent)
      , mGui(gui)
      , mMaximumDistance(std::numeric_limits<float>::max())
  {
    // Set a name for the component, so we can find it later if needed
    setName("AnchoredTextLabels");

    // The GUI widget component is required for drawing the labels
    if (mGui.isDestroyed())
    {
      REGOTH_THROW(InvalidStateException, "The provided CGUIWidget component is invalid.");
    }
  }

  void AnchoredTextLabels::onDestroyed()
  {
    if (!mGui.isDestroyed())
    {
      bs::GUIPanel* mainPanel = mGui->getPanel();

      for (const auto& anchoredLabel : mLabels)
      {
        mainPanel->removeElement(anchoredLabel.mLabel);
      }
    }
  }

  void AnchoredTextLabels::addLabel(const bs::Vector3& anchorPosition, const bs::HString& text)
  {
    bs::GUILabel* label = bs::GUILabel::create(text);
    mLabels.emplace_back(AnchoredLabel{anchorPosition, label});

    bs::GUIPanel* mainPanel = mGui->getPanel();
    mainPanel->addElement(label);
  }

  void AnchoredTextLabels::setMaximumDistance(float maximumDistance)
  {
    assert(maximumDistance >= 0.f);

    mMaximumDistance = maximumDistance;
  }

  void AnchoredTextLabels::update()
  {
    auto camera = mGui->getCamera();

    const bs::Transform& cameraTransform = camera->getTransform();

    for (const auto& anchoredLabel : mLabels)
    {
      // Only show labels with an anchor point in front of the camera
      if (isPointWithinForwardDrawDistance(cameraTransform, anchoredLabel.mWorldPosition))
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

  bool AnchoredTextLabels::isPointWithinForwardDrawDistance(const bs::Transform& cameraTransform,
                                                            const bs::Vector3& point)
  {
    const bs::Vector3& cameraPosition = cameraTransform.getPosition();
    const bs::Vector3& cameraForward  = cameraTransform.getForward();

    bs::Vector3 positionalDifference = point - cameraPosition;

    bool isInFrontOfCamera = positionalDifference.dot(cameraForward) > 0.f;
    bool isWithinDistance =
        positionalDifference.isZeroLength() || positionalDifference.length() <= mMaximumDistance;

    return isInFrontOfCamera && isWithinDistance;
  }

}  // namespace REGoth
