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
   * TODO: Documentation of GameUI
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
