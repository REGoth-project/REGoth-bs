#pragma once
#include "UIElement.hpp"
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class UISubtitleBox;
  using HUISubtitleBox = bs::GameObjectHandle<UISubtitleBox>;

  class UIDialogueChoice;
  using HUIDialogueChoice = bs::GameObjectHandle<UIDialogueChoice>;

  class UIFocusText;
  using HUIFocusText = bs::GameObjectHandle<UIFocusText>;

  class GameUI;
  using HGameUI = bs::GameObjectHandle<GameUI>;

  /**
   * Gameplay UI
   * ===========
   *
   * This is the base for all gameplay UI related things, including:
   *
   *  - Health-bars
   *  - Mana-bar
   *  - Dialogue
   *  - related
   *  - GUI
   *  - Stats-screen
   *  - Log-screen
   *  - In-game main menu
   *
   * Rather than implementing the UI code of those functionalities here, each of
   * those functions is implemented as a component: For example, the dialogue
   * choice box will have its own component, as well es the dialogue subtitle
   * box.
   *
   * Each of those specialized UI components is then attached to a new scene
   * object to build a tree-like structure with the scene object holding the
   * GameUI as base.
   *
   * The structure could look as follows (this is only an example):
   *
   *      +-- GameUI
   *          |
   *          +-- HUD
   *          |   +-- Bar (Health)
   *          |   +-- Bar (Mana)
   *          |   +-- Bar (Enemy Health)
   *          |
   *          +-- Dialogue UI
   *              +-- Subtitle Box
   *              +-- Choices
   *
   * While being a separation of concerns, this structure makes it possible to
   * use some UIs in different contexts. For example, the ingame menu is also
   * the main menu you see when you have just started the game. Then, no world
   * is loaded and no gameplay session exists, thus there is no GameUI.
   *
   */
  class GameUI : public UIElement
  {
  public:
    GameUI(const bs::HSceneObject& parent, bs::HCamera camera);
    virtual ~GameUI();

    /**
     * Create the global GameUI-object. After this has been called,
     * the handle returned by gGameUI() will be valid.
     *
     * Throws if the GameUI has been created before.
     */
    static void createGlobal(bs::HCamera camera);

    /**
     * Starts a dialogue. I'm not happy with this being here, should be moved *somewhere* else,
     * but it seems like this needs to be a global object...
     */
    void startDialogue();
    bool isDialogueInProgress() const;
    void stopDialogue();

    HUIDialogueChoice choices() const
    {
      return mChoices;
    }

    HUISubtitleBox subtitleBox() const
    {
      return mSubtitleBox;
    }

    HUIFocusText focusText() const
    {
      return mFocusText;
    }

  protected:

    void onInitialized() override;

    HUIDialogueChoice mChoices;
    HUISubtitleBox mSubtitleBox;
    HUIFocusText mFocusText;

  private:
  public:
    REGOTH_DECLARE_RTTI(GameUI)

  protected:
    GameUI() = default;  // For RTTI
  };

  /**
   * Global access to the GameUI. While this is global, it should not be accessed
   * directly from within game logic code where possible.
   *
   * Only valid after GameUI::createGlobal() has been called.
   *
   * Throws, if GameUI::createGlobal() has not been called.
   */
  HGameUI gGameUI();
}  // namespace REGoth
