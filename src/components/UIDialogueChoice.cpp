#include "UIDialogueChoice.hpp"
#include <GUI/BsGUIButton.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUILayoutY.h>
#include <GUI/BsGUIScrollArea.h>
#include <RTTI/RTTI_UIDialogueChoice.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  UIDialogueChoice::UIDialogueChoice(const bs::HSceneObject& parent, HUIElement parentUiElement)
      : UIElement(parent, parentUiElement, new bs::GUILayoutY())
  {
    mScrollArea = layout().addNewElement<bs::GUIScrollArea>(bs::ScrollBarType::ShowIfDoesntFit,
                                                            bs::ScrollBarType::NeverShow);
    setName("UIDialogueChoice");
  }

  UIDialogueChoice::~UIDialogueChoice()
  {
  }

  void UIDialogueChoice::addChoice(const Choice& choice)
  {
    bs::UINT32 index = mChoices.size();

    mChoices.push_back(choice);

    auto button = mScrollArea->getLayout().addNewElement<bs::GUIButton>(bs::HString(choice.text));
    mChoiceButtons.push_back(button);

    button->onClick.connect([this, index]() { mOnChoice(mChoices[index]); });
  }

  void UIDialogueChoice::clearChoices()
  {
    for (bs::GUIButton* pButton : mChoiceButtons)
    {
      mScrollArea->getLayout().removeElement(pButton);
    }

    mChoiceButtons.clear();
    mChoices.clear();
  }

  void UIDialogueChoice::update()
  {
    UIElement::update();

    bs::Rect2I bounds = parentLayout().getBounds();

    bounds.y      = parentLayout().getBounds().height * 0.80f;
    bounds.height = parentLayout().getBounds().height - bounds.y;

    layout().setBounds(bounds);
    mScrollArea->setWidth(layout().getBounds().width);
    mScrollArea->setHeight(layout().getBounds().height);
  }

  REGOTH_DEFINE_RTTI(UIDialogueChoice)
}  // namespace REGoth
