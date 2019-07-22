#pragma once
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class UIConsole;
  using HUIConsole = bs::GameObjectHandle<UIConsole>;

  class Console;
  using HConsole = bs::GameObjectHandle<Console>;

  class Console : public bs::Component
  {
  public:
    Console(const bs::HSceneObject& parent);
    virtual ~Console();

    void onInitialized() override;
    void onInputChanged(const bs::String& input);
    void onCommandConfirmed();

  private:
    HUIConsole mConsoleBox;

    typedef void (Console::*commandCallback)(bs::Vector<bs::String>);
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

  public:
    REGOTH_DECLARE_RTTI(Console)

  protected:
    Console() = default;  // For RTTI
  };
}  // namespace REGoth
