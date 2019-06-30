#pragma once
#include "UIElement.hpp"
#include <RTTI/RTTIUtil.hpp>
#include <scripting/ScriptTypes.hpp>

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
   * To identify which choice has been taken by the user, the class also stores the
   * name of the script instance the info originates from. Some choices are custom-
   * built using `Info_AddChoice` inside the scripts and don't have any instance
   * name. Because of these, the class also stores the function to execute once a choice
   * has been taken.
   *
   * Once a choice has been taken by the user, the `onChoice`-callback is triggered.
   */
  class UIDialogueChoice : public UIElement
  {
  public:
    struct Choice
    {
      /** Name of the info inside the scripts */
      bs::String instanceName;

      /** Text to display in the choice box. This is what the hero should say. */
      bs::String text;

      /** Script function to execute once this choice has been chosen. */
      Scripting::SymbolIndex scriptFunction;
    };

    /** Passing Choice per value here since the user could decide to call clearChoices()
        inside the callback which would make a const reference invalid. */
    using OnChoiceCallback = std::function<void(Choice)>;

    UIDialogueChoice(const bs::HSceneObject& parent, HUIElement parentUiElement);
    virtual ~UIDialogueChoice();

    /**
     * Sets which callback to trigger once the user selected a choice
     */
    void setOnChoiceCallback(const OnChoiceCallback& onChoice)
    {
      mOnChoice = onChoice;
    }

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
