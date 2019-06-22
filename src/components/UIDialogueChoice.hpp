#pragma once
#include "UIElement.hpp"
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class UIDialogueChoice;
  using HUIDialogueChoice = bs::GameObjectHandle<UIDialogueChoice>;

  /**
   * UI which shows the dialogue lines you can select for the hero to say. This is
   * the small box on the bottom of the screen which you get when talking to an NPC
   * where you can select a dialogue option.
   *
   * This class has to work with how the scripts are using the dialogue system. For
   * once, there are the static dialogue choices which are defined as `INSTANCE` in
   * the script fields. Then, scripts can dynamically clear and add other choices.
   * Therefore, this class also allows the user to clear and add choices, which is
   * enough to handle both cases.
   *
   * Giving this class a list of choices would not work for the second case, as
   * scripts only add one choice in a single script function call.
   *
   * To identify which choice has been taken by the user, we need something unique
   * about the choice, like an index, so the calling code knows which choice the
   * user has taken. However, an index would make handling choices dynamically added
   * by scripts hard to handle. But all choices have one thing in common: A script
   * function to call once they have been taken! This script function will run the
   * dialogue script, so we would need to know it anyways once the choice as been made.
   * Therefore the name of this script function is our identifier which this also given
   * to the caller once a choice has been taken.
   *
   * Once a choice has been taken by the user, the `onChoice`-callback is triggered.
   */
  class UIDialogueChoice : public UIElement
  {
  public:
    struct Choice
    {
      /** Text to display in the choice box. This is what the hero should say. */
      bs::String text;

      /** Script function to execute once this choice has been chosen. */
      bs::String scriptFunction;
    };

    using OnChoiceCallback = std::function<void(const Choice&)>;

    UIDialogueChoice(const bs::HSceneObject& parent, HUIElement parentUiElement,
                     OnChoiceCallback onChoice);
    virtual ~UIDialogueChoice();

    /**
     * Adds an choice for the user to choose.
     */
    void addChoice(const Choice& choice);

    /**
     * Removes all choices.
     */
    void clearChoices();

  protected:
    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;

  private:
    bs::Vector<Choice> mChoices;
    bs::Vector<bs::GUIButton*> mChoiceButtons;
    bs::GUIScrollArea* mScrollArea;

    OnChoiceCallback mOnChoice;

  public:
    REGOTH_DECLARE_RTTI(UIDialogueChoice)

  protected:
    UIDialogueChoice() = default;  // For RTTI
  };
}  // namespace REGoth
