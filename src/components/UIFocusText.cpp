#include "UIFocusText.hpp"
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUIPanel.h>
#include <RTTI/RTTI_UIFocusText.hpp>
#include <Renderer/BsCamera.h>
#include <components/Focusable.hpp>

namespace REGoth
{
  UIFocusText::UIFocusText(const bs::HSceneObject& parent, HUIElement parentUiElement)
      : UIElement(parent, parentUiElement, new bs::GUIPanel())
  {
    setName("UIFocusText");

    mLabelAboveObject = layout().addNewElement<bs::GUILabel>(bs::HString(""));
    mLabelAboveObject->setStyle("GothicLabelObjectFocus");
  }

  UIFocusText::~UIFocusText()
  {
  }

  void UIFocusText::putTextAbove(HFocusable focusable, bs::String text)
  {
    mFocusedObject = focusable;

    mLabelAboveObject->setContent(bs::GUIContent(bs::HString(text)));
  }

  void UIFocusText::setMaximumDistance(float maximumDistanceInMeters)
  {
    assert(maximumDistanceInMeters >= 0.0f);

    mMaximumDistanceInMeters = maximumDistanceInMeters;
  }

  void UIFocusText::update()
  {
    if (!mFocusedObject) return;

    if (shouldLabelBeVisible())
    {
      mLabelAboveObject->setVisible(true);

      moveLabelToProjectedTargetPos();
    }
    else
    {
      mLabelAboveObject->setVisible(false);
    }
  }

  void UIFocusText::moveLabelToProjectedTargetPos()
  {
    auto p = calculateLabelScreenPosition();

    mLabelAboveObject->setPosition(p.x, p.y);
  }

  bs::Vector2I UIFocusText::calculateLabelScreenPosition() const
  {
    return camera().worldToScreenPoint(labelWorldPosition());
  }

  bool UIFocusText::shouldLabelBeVisible() const
  {
    return isTargetInRange();
  }

  bool UIFocusText::isTargetInRange() const
  {
    const bs::Transform& cameraTransform = camera().getTransform();

    const bs::Vector3& cameraPosition = cameraTransform.getPosition();
    const bs::Vector3& cameraForward  = cameraTransform.getForward();

    bs::Vector3 positionalDifference = labelWorldPosition() - cameraPosition;

    bool isInFrontOfCamera = positionalDifference.dot(cameraForward) > 0.0f;

    if (!isInFrontOfCamera)
      return false;
    else if (positionalDifference.isZeroLength())
      return true;
    else if (positionalDifference.length() <= mMaximumDistanceInMeters)
      return true;
    else
      return false;
  }

  bs::Vector3 UIFocusText::labelWorldPosition() const
  {
    bs::Vector3 targetPosition = mFocusedObject->SO()->getTransform().pos();

    targetPosition.y += getLabelYOffset();

    return targetPosition;
  }

  float UIFocusText::getLabelYOffset() const
  {
    if (!mFocusedObject) return 0.0f;

    // TODO: Ask focus component about the height
    return 0.3f;
  }

  REGOTH_DEFINE_RTTI(UIFocusText)

}  // namespace REGoth
