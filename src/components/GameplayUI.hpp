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

  class UIInventory;
  using HUIInventory = bs::GameObjectHandle<UIInventory>;

  class GameplayUI;
  using HGameplayUI = bs::GameObjectHandle<GameplayUI>;

  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

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
   * GameplayUI as base.
   *
   * The structure could look as follows (this is only an example):
   *
   *      +-- GameplayUI
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
   * is loaded and no gameplay session exists, thus there is no GameplayUI.
   *
   */
  class GameplayUI : public UIElement
  {
  public:
    GameplayUI(const bs::HSceneObject& parent, bs::HCamera camera);
    virtual ~GameplayUI();

    /**
     * Create the global GameplayUI-object. After this has been called,
     * the handle returned by gGameUI() will be valid.
     *
     * Throws if the GameplayUI has been created before.
     */
    static void createGlobal(bs::HCamera camera);

    /**
     * Starts a dialogue. I'm not happy with this being here, should be moved *somewhere* else,
     * but it seems like this needs to be a global object...
     */
    void startDialogue();
    bool isDialogueInProgress() const;
    void stopDialogue();

    /**
     * Set the character the HUD should be showing information for.
     */
    void setTargetCharacter(HCharacter character);

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

    HUIInventory inventoryUI() const
    {
      return mInventoryUI;
    }

  protected:
    void gatherInformationFromTargetCharacter();

    void onInitialized() override;
    void update() override;

    HUIDialogueChoice mChoices;
    HUISubtitleBox mSubtitleBox;
    HUIFocusText mFocusText;
    HUIInventory mInventoryUI;

    HCharacter mTargetCharacter;

  private:
  public:
    REGOTH_DECLARE_RTTI(GameplayUI)

  protected:
    GameplayUI() = default;  // For RTTI
  };

  /**
   * Global access to the GameplayUI. While this is global, it should not be accessed
   * directly from within game logic code where possible.
   *
   * Only valid after GameplayUI::createGlobal() has been called.
   *
   * Throws, if GameplayUI::createGlobal() has not been called.
   */
  HGameplayUI gGameplayUI();
}  // namespace REGoth
