#include "UISubtitleBox.hpp"
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUIPanel.h>
#include <GUI/BsGUITexture.h>
#include <RTTI/RTTI_UISubtitleBox.hpp>
#include <original-content/OriginalGameResources.hpp>

namespace REGoth
{
  /**
   * How fast the box should be growing/shrinking
   */
  constexpr float SIZE_CHANGE_SPEED = 2.0f;

  UISubtitleBox::UISubtitleBox(const bs::HSceneObject& parent, HUIElement parentUiElement)
      : UIElement(parent, parentUiElement, new bs::GUIPanel())
  {
    setName("UISubtitleBox");

    mBackgroundBox =
        layout().addNewElement<bs::GUITexture>(gOriginalGameResources().sprite("DLG_CHOICE.TGA"));

    mText =
        layout().addNewElement<bs::GUILabel>(bs::HString("Hello World!"));
  }

  UISubtitleBox::~UISubtitleBox()
  {
  }

  void UISubtitleBox::update()
  {
    UIElement::update();

    bs::Rect2I parentBounds  = parentLayout().getBounds();
    bs::UINT32 thirdOfParent = parentBounds.width / 3;

    layout().setPosition(thirdOfParent, parentBounds.height * 0.01);
    layout().setWidth(thirdOfParent * mBoxSizeRatio);
    layout().setHeight(parentBounds.height * 0.1 * mBoxSizeRatio);

    switch (mState)
    {
      case State::Closed:
        mBackgroundBox->setVisible(false);
        mText->setVisible(false);
        break;

      case State::Growing:
        mBoxSizeRatio =
            std::min(1.0f, mBoxSizeRatio + bs::gTime().getFrameDelta() * SIZE_CHANGE_SPEED);

        if (mBoxSizeRatio >= 1)
        {
          mState = State::Open;
        }
        break;

      case State::Open:
        mBackgroundBox->setVisible(true);
        mText->setVisible(true);
        break;

      case State::Shrinking:
        mBoxSizeRatio =
            std::max(0.0f, mBoxSizeRatio - bs::gTime().getFrameDelta() * SIZE_CHANGE_SPEED);

        if (mBoxSizeRatio <= 0)
        {
          mState = State::Closed;
        }

        break;
    }
  }

  void UISubtitleBox::open()
  {
    if (mState == State::Open) return;

    mState = State::Growing;
  }

  void UISubtitleBox::close()
  {
    if (mState == State::Closed) return;

    mState = State::Shrinking;
  }

  void UISubtitleBox::setTalkerName(const bs::String& name)
  {
    // TODO: Implement
  }

  void UISubtitleBox::setDialogueText(const bs::String& text)
  {
    mText->setContent(bs::HString(text));
  }

  bool UISubtitleBox::isDoingAnimation() const
  {
    if (mState == State::Growing) return true;
    if (mState == State::Shrinking) return true;

    return false;
  }

  REGOTH_DEFINE_RTTI(UISubtitleBox)
}  // namespace REGoth
