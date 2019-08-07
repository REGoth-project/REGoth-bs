#include "Console.hpp"
#include "components/GameClock.hpp"
#include "components/GameWorld.hpp"
#include "components/GameplayUI.hpp"
#include "components/UIConsole.hpp"
#include <RTTI/RTTI_Console.hpp>
#include <String/BsString.h>
#include <log/logging.hpp>

namespace REGoth
{
  Console::Console(const bs::HSceneObject& parent, HGameWorld gameWorld)
      : bs::Component(parent)
      , mGameWorld(gameWorld)
  {
    setName("Console");

    mConsoleUI = gGameplayUI()->consoleUI();

    registerAllCommand();
  }

  Console::~Console()
  {
  }

  void Console::onInitialized()
  {
    auto valueChangedCallback = [&](const bs::String& input) {
      REGOTH_LOG(Info, Uncategorized, "[Console] !EVENT! Input changed to: {0}!", input);
      onInputChanged(input);
    };
    mConsoleUI->mOnInputChanged.connect(valueChangedCallback);

    auto confirmCallback = [&]() {
      const bs::String& input = mConsoleUI->getInput();
      REGOTH_LOG(Info, Uncategorized, "[Console] !EVENT! Command confirmed: {0}!", input);
      parseAndExecuteCommand(input);
      mConsoleUI->clearInput();
    };
    mConsoleUI->mOnConfirm.connect(confirmCallback);
  }

  void Console::onInputChanged(const bs::String& input)
  {
    // TODO + rename
  }

  void Console::parseAndExecuteCommand(const bs::String& input)
  {
    bs::String sanitized_input = input;
    bs::StringUtil::trim(sanitized_input);
    bs::Vector<bs::String> tokenized_input = bs::StringUtil::split(sanitized_input, " ");

    for (const auto& token : tokenized_input)
      REGOTH_LOG(Info, Uncategorized, "[Console] Token in tokenized input: {0}!", token);

    // detect command
    auto it = mCommands.begin();
    for (; it != mCommands.end(); it++)
    {
      // construct command based on number of tokens
      bs::UINT32 num_of_tokens = it->num_of_tokens;
      if (tokenized_input.size() < num_of_tokens) continue;
      bs::String command;
      for (bs::UINT32 i = 0; i < num_of_tokens; i++)
      {
        if (!command.empty())
        {
          command = command + " ";
        }
        command = command + tokenized_input[i];
      }

      // if the command matches the input we just break and it will contain the correct command
      if (bs::StringUtil::compare(command, it->name) == 0) break;
    }

    // Command not found
    if (it == mCommands.end()) return;
    REGOTH_LOG(Info, Uncategorized, "[Console] Command was found and is: {0}!", it->name);

    // call callback and pass arguments
    tokenized_input.erase(tokenized_input.begin(), tokenized_input.begin() + it->num_of_tokens);
    // FIXME: allow trailing arguments that are ignored for now
    if (tokenized_input.size() < it->args.size())
    {
      REGOTH_LOG(Info, Uncategorized,
                 "[Console] Wrong number of arguments: Should be {0}, but is {1}!", it->args.size(),
                 tokenized_input.size());
      mConsoleUI->setOutput(it->usage);
      return;
    }
    (this->*it->callback)(tokenized_input);
  }

  void Console::command_Clear(bs::Vector<bs::String>& args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'clear' executed!");

    mConsoleUI->clearOutput();
  }

  void Console::command_List(bs::Vector<bs::String>& args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'list' executed!");
    bs::Vector<bs::String> outputs;

    outputs.push_back("List of all commands:");
    for (auto it = mCommands.begin(); it != mCommands.end(); it++)
    {
      bs::String command = it->name;

      outputs.push_back(command);
    }

    mConsoleUI->setOutput(outputs);
  }

  void Console::command_Help(bs::Vector<bs::String>& args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'help' executed!");
    bs::Vector<bs::String> outputs;

    // detect command
    auto it = mCommands.begin();
    for (; it != mCommands.end(); it++)
    {
      // construct command based on number of tokens
      bs::UINT32 num_of_tokens = it->num_of_tokens;
      if (args.size() < num_of_tokens) continue;
      bs::String command;
      for (bs::UINT32 i = 0; i < num_of_tokens; i++)
      {
        if (!command.empty())
        {
          command = command + " ";
        }
        command = command + args[i];
      }

      // if the command matches the input we just break and it will contain the correct command
      if (bs::StringUtil::compare(command, it->name) == 0) break;
    }

    if (it == mCommands.end())
    {
      outputs.push_back("Unkown command!");
    }
    else
    {
      bs::String usage = it->usage;
      outputs.push_back(usage);
      bs::String help = it->help;
      outputs.push_back(help);
    }

    mConsoleUI->setOutput(outputs);
  }

  void Console::command_CheatFull(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'cheat full' is not implemented yet!");

    mConsoleUI->setOutput("Command 'cheat full' is not implemented yet!");
  }

  void Console::command_CheatGod(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'cheat god' is not implemented yet!");

    mConsoleUI->setOutput("Command 'cheat god' is not implemented yet!");
  }

  void Console::command_Insert(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'insert' is not implemented yet!");

    mConsoleUI->setOutput("Command 'insert' is not implemented yet!");
  }

  void Console::command_SpawnMass(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'spawnmass' is not implemented yet!");

    mConsoleUI->setOutput("Command 'spawnmass' is not implemented yet!");
  }

  void Console::command_Kill(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'kill' is not implemented yet!");

    mConsoleUI->setOutput("Command 'kill' is not implemented yet!");
  }

  void Console::command_EditAbilities(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'edit abilities' is not implemented yet!");

    mConsoleUI->setOutput("Command 'edit abilities' is not implemented yet!");
  }

  void Console::command_EditFocus(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'edit focus' is not implemented yet!");

    mConsoleUI->setOutput("Command 'edit focus' is not implemented yet!");
  }

  void Console::command_GetTime(bs::Vector<bs::String>& args)
  {
    auto clock       = mGameWorld->gameclock();
    bs::INT32 day    = clock->getDay();
    bs::INT32 hour   = clock->getHour();
    bs::INT32 minute = clock->getMinute();

    // FIXME: proper formatting for hour and day, maybe move to GameClock
    bs::String output = bs::StringUtil::format("Time: Day {0} {1}:{2}", day, hour, minute);

    mConsoleUI->setOutput(output);
    REGOTH_LOG(Info, Uncategorized, output);
  }

  void Console::command_SetTime(bs::Vector<bs::String>& args)
  {
    auto clock   = mGameWorld->gameclock();
    bs::INT32 hh = bs::parseINT32(args[0]);
    bs::INT32 mm = bs::parseINT32(args[1]);

    clock->setTime(hh, mm);

    mConsoleUI->setOutput("Set time called!");
  }

  void Console::command_GotoWaypoint(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'goto waypoint' is not implemented yet!");

    mConsoleUI->setOutput("Command 'goto waypoint' is not implemented yet!");
  }

  void Console::command_GotoCamera(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'goto camera' is not implemented yet!");

    mConsoleUI->setOutput("Command 'goto camera' is not implemented yet!");
  }

  void Console::command_GotoPos(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'goto pos' is not implemented yet!");

    mConsoleUI->setOutput("Command 'goto pos' is not implemented yet!");
  }

  void Console::command_AIGoto(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'aigoto' is not implemented yet!");

    mConsoleUI->setOutput("Command 'aigoto' is not implemented yet!");
  }

  void Console::command_SetClippingfactor(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized,
               "[Console] Command 'set clippingfactor' is not implemented yet!");

    mConsoleUI->setOutput("Command 'set clippingfactor' is not implemented yet!");
  }

  void Console::command_ZFogZone(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'zfogzone' is not implemented yet!");

    mConsoleUI->setOutput("Command 'zfogzone' is not implemented yet!");
  }

  void Console::command_ToggleConsole(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'toggle console' is not implemented yet!");

    mConsoleUI->setOutput("Command 'toggle console' is not implemented yet!");
  }

  void Console::command_ToggleFrame(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'toggle frame' is not implemented yet!");

    mConsoleUI->setOutput("Command 'toggle frame' is not implemented yet!");
  }

  void Console::command_ToggleWaynet(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'toggle waynet' is not implemented yet!");

    mConsoleUI->setOutput("Command 'toggle waynet' is not implemented yet!");
  }

  void Console::command_Firstperson(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'firstperson' is not implemented yet!");

    mConsoleUI->setOutput("Command 'firstperson' is not implemented yet!");
  }

  void Console::command_HeroExport(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'hero export' is not implemented yet!");

    mConsoleUI->setOutput("Command 'hero export' is not implemented yet!");
  }

  void Console::command_HeroImport(bs::Vector<bs::String>& args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'hero import' is not implemented yet!");

    mConsoleUI->setOutput("Command 'hero import' is not implemented yet!");
  }

  void Console::registerCommand(const Command& command)
  {
    // TODO: Some auto suggestion stuff can happen here
    mCommands.push_back(command);
  }

  void Console::registerAllCommand()
  {
    using This = Console;
    Command command;

    command = CommandBuilder()
                  .name("clear")
                  .callback((commandCallback)&This::command_Clear)
                  .usage("Usage: clear")
                  .help("Clears console output.")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("list")
                  .callback((commandCallback)&This::command_List)
                  .usage("Usage: list")
                  .help("Lists all commands.")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("help")
                  .callback(&This::command_Help)
                  .arg(TokenType::Command)
                  .usage("Usage: help [command]")
                  .help("Prints out helpful information about the given command.")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("cheat full")
                  .callback(&This::command_CheatFull)
                  .usage("Usage: cheat full")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("cheat god")
                  .callback(&This::command_CheatGod)
                  .usage("Usage: cheat god")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("insert")
                  .callback(&This::command_Insert)
                  .arg(TokenType::Instance)
                  .usage("Usage: insert [name]")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("spawnmass")
                  .callback(&This::command_SpawnMass)
                  .arg(TokenType::Literal)
                  .usage("Usage: spawnmass {giga} [amount]")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("kill")
                  .callback(&This::command_Kill)
                  .usage("Usage: kill")
                  .help("Kill the NPC you have currently in focus")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("edit abilities")
                  .callback(&This::command_EditAbilities)
                  .usage("Usage: edit abilities")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("edit focus")
                  .callback(&This::command_EditFocus)
                  .usage("Usage: edit focus")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("get time")
                  .callback(&This::command_GetTime)
                  .usage("Usage: get time")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("set time")
                  .callback(&This::command_SetTime)
                  .arg(TokenType::Literal)
                  .arg(TokenType::Literal)
                  .usage("Usage: set time [hh] [mm]")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("goto waypoint")
                  .callback(&This::command_GotoWaypoint)
                  .arg(TokenType::Waypoint)
                  .usage("Usage: goto waypoint [waypoint]")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("goto camera")
                  .callback(&This::command_GotoCamera)
                  .usage("Usage: goto camera")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("goto pos")
                  .callback(&This::command_GotoPos)
                  .arg(TokenType::Literal)
                  .arg(TokenType::Literal)
                  .arg(TokenType::Literal)
                  .usage("Usage: goto pos [x] [y] [z]")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("aigoto")
                  .callback(&This::command_AIGoto)
                  .arg(TokenType::Waypoint)
                  .usage("Usage: aigoto [waypoint]")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("set clippingfactor")
                  .callback(&This::command_SetClippingfactor)
                  .usage("Usage: set clippingfactor [f]")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("zgofzone")
                  .callback(&This::command_ZFogZone)
                  .usage("Usage: zfogzone")
                  .help("Some Fogzone stuff")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("toggle console")
                  .callback(&This::command_ToggleConsole)
                  .usage("Usage: toggle console")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("toggle frame")
                  .callback(&This::command_ToggleFrame)
                  .usage("Usage: toggle frame")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("toggle waynet")
                  .callback(&This::command_ToggleWaynet)
                  .usage("Usage: toggle waynet")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("firstperson")
                  .callback(&This::command_Firstperson)
                  .usage("Usage: firstperson")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("hero export")
                  .callback(&This::command_HeroExport)
                  .arg(TokenType::Literal)
                  .usage("Usage: hero export [filename]")
                  .help("")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("hero import")
                  .callback(&This::command_HeroImport)
                  .arg(TokenType::Literal)
                  .usage("Usage: hero import [filename]")
                  .help("")
                  .build();
    registerCommand(command);
  }

  REGOTH_DEFINE_RTTI(Console)
}  // namespace REGoth
