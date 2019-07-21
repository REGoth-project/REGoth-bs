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
    typedef void (UIConsole::*commandCallback)(bs::Vector<bs::String>);
    struct Command
    {
      commandCallback callback;
      size_t num_of_args;
      bs::String usage;
      bs::String help;
    };
    bs::Map<bs::String, Command> mCommands;
    void command_List(bs::Vector<bs::String> args);
    void command_Help(bs::Vector<bs::String> args);
    void command_CheatFull(bs::Vector<bs::String> args);
    void command_CheatGod(bs::Vector<bs::String> args);
    void command_Insert(bs::Vector<bs::String> args);
    void command_Spawnmass(bs::Vector<bs::String> args);
    void command_Kill(bs::Vector<bs::String> args);
    void command_EditAbilities(bs::Vector<bs::String> args);
    void command_EditFocus(bs::Vector<bs::String> args);
    void command_SetTime(bs::Vector<bs::String> args);
    void command_GotoWaypoint(bs::Vector<bs::String> args);
    void command_GotoCamera(bs::Vector<bs::String> args);
    void command_GotoPos(bs::Vector<bs::String> args);
    void command_AIGoto(bs::Vector<bs::String> args);
    void command_SetClippingfactor(bs::Vector<bs::String> args);
    void command_ZFogZone(bs::Vector<bs::String> args);
    void command_ToggleConsole(bs::Vector<bs::String> args);
    void command_ToggleFrame(bs::Vector<bs::String> args);
    void command_ToggleWaynet(bs::Vector<bs::String> args);
    void command_Firstperson(bs::Vector<bs::String> args);
    void command_HeroExport(bs::Vector<bs::String> args);
    void command_HeroImport(bs::Vector<bs::String> args);
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
