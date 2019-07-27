#pragma once
#include "UIElement.hpp"
#include <GUI/BsGUIInputBox.h>
#include <Input/BsVirtualInput.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class UIConsole;
  using HUIConsole = bs::GameObjectHandle<UIConsole>;

  class Console;
  using HConsole = bs::GameObjectHandle<Console>;

  /**
   * This class handles the visual side of the in-game Console. It consists of
   * scrollable output windows and an input box to enter the console commands. Additionally there are
   * UIElements for autocomplete suggestions and extra functionality such as a UI for editing npc
   * properties (TODO).
   *
   * The component is largely autonomous and works solely on the input of the user by delegated it to
   * the actual Console component.
   */
  class UIConsole : public UIElement
  {
  public:
    UIConsole(const bs::HSceneObject& parent, HUIElement parentUiElement);
    virtual ~UIConsole();

  protected:
    enum class State
    {
      Closed,
      Open,
    };

    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;

  private:
    /**
     * Sets internal variables for VirtualButtons and event callbacks (to the Console component) for
     * the following events:
     * onValueChanged
     * onConfirm
     */
    void onInitialized() override;

    /**
     * Resets the input of the internal InputBox UIElement to the empty String "",
     * Used to clear the input after a command has been entered.
     */
    void clearInput();

    /**
     * Creates UI Labels for each of the Strings in \p outputs to be put into the Scrollable Area.
     *
     * @param outputs A Vector of Strings with the ouputs to be added to the Scrollable Area.
     */
    void setOutput(bs::Vector<bs::String> outputs);

    /**
     * Creates a UI Label for \p output to be put into the Scrollable Area.
     *
     * @param output a String with the output to be added to the Scrollable Area.
     */
    void setOutput(bs::String output);

  private:
    State mState = State::Closed;
    HConsole mConsole;
    bs::GUITexture* mBackground    = nullptr;
    bs::GUIScrollArea* mScrollArea = nullptr;
    bs::GUIInputBox* mInputBox     = nullptr;
    bs::VirtualButton mToggleConsole;
    bs::VirtualButton mConfirm;

  public:
    REGOTH_DECLARE_RTTI(UIConsole)

  protected:
    UIConsole() = default;  // For RTTI
  };
}  // namespace REGoth
