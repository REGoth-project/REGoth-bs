#pragma once
#include "UIElement.hpp"
#include <Input/BsVirtualInput.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class UIConsole;
  using HUIConsole = bs::GameObjectHandle<UIConsole>;

  /**
   * TODO: Documentation of UISubtitleBox
   */
  class UIConsole : public UIElement
  {
  public:
    UIConsole(const bs::HSceneObject& parent, HUIElement parentUiElement);
    virtual ~UIConsole();

    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;

  protected:
    void onInitialized() override;
    enum class State
    {
      Closed,
      Open,
    };

  private:
    typedef bs::String (UIConsole::*commandCallback)(bs::Vector<bs::String>);
    struct Command
    {
      commandCallback callback;
      size_t num_of_args;
      bs::String usage;
      bs::String help;
    };
    bs::Map<bs::String, Command> mCommands;
    bs::String command_Dummy(bs::Vector<bs::String> args);
    bs::String command_List(bs::Vector<bs::String> args);
    bs::String command_Help(bs::Vector<bs::String> args);
    void registerCommand(const bs::String& name, Command command);
    void registerAllCommand();

    bs::GUITexture* mBackground    = nullptr;
    bs::GUIScrollArea* mScrollArea = nullptr;
    bs::GUIInputBox* mInputBox     = nullptr;
    bs::VirtualButton mToggleConsole;
    bs::VirtualButton mConfirm;
    State mState = State::Closed;

  public:
    REGOTH_DECLARE_RTTI(UIConsole)

  protected:
    UIConsole() = default;  // For RTTI
  };
}  // namespace REGoth
