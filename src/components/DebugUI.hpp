#pragma once
#include "UIElement.hpp"
#include <Input/BsVirtualInput.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class DebugUI;
  using HDebugUI = bs::GameObjectHandle<DebugUI>;

  class Debug;
  using HDebug = bs::GameObjectHandle<Debug>;

  class UIConsole;
  using HUIConsole = bs::GameObjectHandle<UIConsole>;

  /**
   * Debug UI
   * ===========
   *
   * This is the base for all debug UI related things, including:
   *
   *  - Console
   *  - Overlays
   *  - Other debug menus
   *
   * Rather than implementing the UI code of those functionalities here, each of
   * those functions is implemented as a component: For example, the dialogue
   * choice box will have its own component, as well es the dialogue subtitle
   * box.
   *
   * Each of those specialized UI components is then attached to a new scene
   * object to build a tree-like structure with the scene object holding the
   * DebugUI as base.
   *
   * The structure could look as follows (this is only an example):
   *
   *      +-- DebugUI
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
  class DebugUI : public UIElement
  {
  public:
    DebugUI(const bs::HSceneObject& parent, bs::HCamera camera);
    virtual ~DebugUI();

    /**
     * Create the global DebugUI-object. After this has been called,
     * the handle returned by gGameUI() will be valid.
     *
     * Throws if the DebugUI has been created before.
     */
    static void createGlobal(bs::HCamera camera);

    HDebug debug() const
    {
      return mDebug;
    }

    HUIConsole consoleUI() const
    {
      return mConsoleUI;
    }

  protected:
    void onInitialized() override;
    void update() override;

  private:
    HDebug mDebug;
    HUIConsole mConsoleUI;
    bs::VirtualButton mToggleConsole;

  public:
    REGOTH_DECLARE_RTTI(DebugUI)

  protected:
    DebugUI() = default;  // For RTTI
  };

  /**
   * Global access to the DebugUI. While this is global, it should not be accessed
   * directly from within game logic code where possible.
   *
   * Only valid after DebugUI::createGlobal() has been called.
   *
   * Throws, if DebugUI::createGlobal() has not been called.
   */
  HDebugUI gDebugUI();
}  // namespace REGoth
