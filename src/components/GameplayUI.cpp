#include "GameplayUI.hpp"
#include <GUI/BsGUIButton.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUILayoutY.h>
#include <GUI/BsGUIScrollArea.h>
#include <RTTI/RTTI_GameplayUI.hpp>
#include <components/UIDialogueChoice.hpp>
#include <components/UIFocusText.hpp>
#include <components/UISubtitleBox.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  static HGameplayUI s_GameplayUI;

  GameplayUI::GameplayUI(const bs::HSceneObject& parent, bs::HCamera camera)
      : UIElement(parent, camera)
  {
    setName("GameplayUI");
  }

  GameplayUI::~GameplayUI()
  {
  }

  void GameplayUI::onInitialized()
  {
    UIElement::onInitialized();

    if (!mChoices)
    {
      mChoices = addChildElement<UIDialogueChoice>(
          "UIDialogueChoice");
    }

    if (!mSubtitleBox)
    {
      mSubtitleBox = addChildElement<UISubtitleBox>("UISubtitleBox");
    }

    if (!mFocusText)
    {
      mFocusText = addChildElement<UIFocusText>("UIFocusText");
    }
  }

  void GameplayUI::createGlobal(bs::HCamera camera)
  {
    if (s_GameplayUI)
    {
      REGOTH_THROW(InvalidStateException, "GameplayUI has already been created!");
    }

    auto so = bs::SceneObject::create("GameplayUI");

    s_GameplayUI = so->addComponent<GameplayUI>(camera);
  }

  void GameplayUI::startDialogue()
  {
    choices()->SO()->setActive(true);
    choices()->clearChoices();
  }

  bool GameplayUI::isDialogueInProgress() const
  {
    return choices()->SO()->getActive();
  }

  void GameplayUI::stopDialogue()
  {
    choices()->SO()->setActive(false);
    choices()->clearChoices();
  }

  HGameplayUI gGameplayUI()
  {
    if (!s_GameplayUI)
    {
      REGOTH_THROW(InvalidStateException, "GameplayUI::createGlobal() has not been called before!");
    }

    return s_GameplayUI;
  }

  REGOTH_DEFINE_RTTI(GameplayUI)
}  // namespace REGoth
