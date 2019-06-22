#pragma once
#include "UIElement.hpp"
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class UISubtitleBox;
  using HUISubtitleBox = bs::GameObjectHandle<UISubtitleBox>;

  /**
   * TODO: Documentation of UISubtitleBox
   */
  class UISubtitleBox : public UIElement
  {
  public:
    UISubtitleBox(const bs::HSceneObject& parent, HUIElement parentUiElement);
    virtual ~UISubtitleBox();

    /**
     * Plays the *Growing*-Animation to show the subtitle. Nothing happens if the
     * subtitle box is already open or currently playing the *Growing* animation.
     * If the *Shrinking* animation is being played, it will just reverse and play the
     * *Growing* animation again.
     */
    void open();

    /**
     * Plays the *Shrinking*-Animation to show the subtitle. Nothing happens if the
     * subtitle box is already closed or currently playing the *Shrinking* animation.
     * If the *Growing* animation is being played, it will just reverse and play the
     * *Shrinking* animation again.
     */
    void close();

    /**
     * Sets the name displayed above the dialogue line. For example, if *Diego* is
     * talking, this should be set to *Diego*.
     */
    void setTalkerName(const bs::String& name);

    /**
     * Sets the dialogue text to be displayed inside the subtitle box.
     */
    void setDialogueText(const bs::String& text);

    /**
     * @return Whether the growing/shrinking animation is currently playing.
     */
    bool isDoingAnimation() const;

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

  private:
    bs::GUITexture* mBackgroundBox = nullptr;
    bs::GUILabel* mText            = nullptr;

    State mState = State::Closed;
    float mBoxSizeRatio = 0.0f;

  public:
    REGOTH_DECLARE_RTTI(UISubtitleBox)

  protected:
    UISubtitleBox() = default;  // For RTTI
  };
}  // namespace REGoth
