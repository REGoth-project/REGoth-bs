#pragma once
#include <components/UIElement.hpp>
#include <debug/Console.hpp>
#include <GUI/BsGUITexture.h>
#include <GUI/BsGUIScrollArea.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUIInputBox.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class UIConsole;
  using HUIConsole = bs::GameObjectHandle<UIConsole>;

  /**
   * TODO: Documentation of UIConsole
   */
  class UIConsole : public UIElement
  {
  public:
    UIConsole(const bs::HSceneObject& parent, HUIElement parentUiElement);
    virtual ~UIConsole();

    /**
     * Plays the *Growing*-Animation to show the subtitle. Nothing happens if the
     * subtitle box is already open or currently playing the *Growing* animation.
     * If the *Shrinking* animation is being played, it will just reverse and play the
     * *Growing* animation again.
     */
    void toggle();
    bool isOpen();

  protected:
    enum class State
    {
      Closed,
      Growing,
      Open,
      Shrinking,
    };

    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;
    void onInitialized() override;

  private:
    State mState = State::Closed;
    Console mConsole;

    bs::GUITexture* mBackground    = nullptr;
    bs::GUIScrollArea* mScrollArea = nullptr;
    bs::Vector<bs::GUILabel*> mOutputLabels;
    bs::GUIInputBox* mInputBox = nullptr;
    float mVisibleRatio = 0.0f;

    void clearInput();
    void clearOutput();
    void setOutput(const bs::String& output);

  public:
    REGOTH_DECLARE_RTTI(UIConsole)

  protected:
    UIConsole() = default;  // For RTTI
  };
}  // namespace REGoth
