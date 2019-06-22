#include "GameUI.hpp"
#include <components/UIFocusText.hpp>
#include <GUI/BsGUIButton.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUILayoutY.h>
#include <GUI/BsGUIScrollArea.h>
#include <RTTI/RTTI_GameUI.hpp>
#include <components/UIDialogueChoice.hpp>
#include <components/UISubtitleBox.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  static HGameUI s_GameUI;

  GameUI::GameUI(const bs::HSceneObject& parent, bs::HCamera camera)
      : UIElement(parent, camera)
  {
    setName("GameUI");
  }

  GameUI::~GameUI()
  {
  }

  void GameUI::onInitialized()
  {
    UIElement::onInitialized();

    if (!mChoices)
    {
      mChoices = addChildElement<UIDialogueChoice>(
          "UIDialogueChoice", [this](const UIDialogueChoice::Choice& choice) {
            bs::gDebug().logDebug("Choice taken: " + choice.text + " (" + choice.scriptFunction +
                                  ")");
          });
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

  void GameUI::createGlobal(bs::HCamera camera)
  {
    if (s_GameUI)
    {
      REGOTH_THROW(InvalidStateException, "GameUI has already been created!");
    }

    auto so = bs::SceneObject::create("GameUI");

    s_GameUI = so->addComponent<GameUI>(camera);
  }

  HGameUI gGameUI()
  {
    if (!s_GameUI)
    {
      REGOTH_THROW(InvalidStateException, "GameUI::createGlobal() has not been called before!");
    }

    return s_GameUI;
  }

  REGOTH_DEFINE_RTTI(GameUI)
}  // namespace REGoth
