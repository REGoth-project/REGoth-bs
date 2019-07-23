#include "Console.hpp"
#include <RTTI/RTTI_Console.hpp>
#include <String/BsString.h>
#include <components/GameplayUI.hpp>
#include <components/UIConsole.hpp>
#include <log/logging.hpp>

namespace REGoth
{
  Console::Console(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("Console");

    registerAllCommand();
  }

  Console::~Console()
  {
  }

  void Console::onInitialized()
  {
  }

  void Console::onInputChanged(const bs::String& input)
  {
    BS_LOG(Info, Uncategorized, "[UIConsole] Input changed! Current input: {0}!", input);
  }

  bs::Vector<bs::String> Console::onCommandConfirmed(bs::String input)
  {
    // Get Input from ConsoleBox
    bs::StringUtil::trim(input);

    bs::Vector<bs::String> outputs;

#if 1
    // O(n) solution, dont need map for this
    for (auto it = mCommands.begin(); it != mCommands.end(); it++)
    {
      bs::String command = it->first;
      if (bs::StringUtil::startsWith(input, command))
      {
        input                       = bs::StringUtil::replaceAll(input, command, "");
        bs::Vector<bs::String> args = bs::StringUtil::split(input, " ");
        size_t num_of_args          = it->second.num_of_args;
        /* TODO: I just want to get rid of empty strings :) */
        /* TODO: first arg is skipped here or something ? */
        for (auto sit = args.begin(); sit != args.end();)
        {
          if ((*sit).empty())
          {
            args.erase(sit);
            continue;
          }
          sit++;
          BS_LOG(Info, Uncategorized, "[Console] args: {0} !", *sit);
        }
        if (args.size() == num_of_args)
        {
          outputs = (this->*it->second.callback)(args);
        }
        else
        {
          bs::String usage = it->second.usage;
          outputs.push_back(usage);
        }
        break;
      }
    }
#endif

#if 0
    // 2*O(1) solution, I dont know
    bs::Vector<bs::String> args = bs::StringUtil::split(input, bs::HString(" "));
    bs::String command          = args.at(0);

    // first pass for single word commands
    auto it = mCommands.find(command);
    if (it != mCommands.end())
    {
      args.erase(args.begin());
      outputs = (this->*it->second.callback)(args);
    }
    // second pass for two word commands
    else if (args.size() >= 2)
    {
      command = command + " " + args.at(1);
      args.erase(args.begin(), args.begin() + 2);

      it = mCommands.find(command);
      if (it != mCommands.end())
      {
        outputs = (this->*it->second.callback)(args);
      }
    }
#endif

    return outputs;
  }

  bs::Vector<bs::String> Console::command_List(bs::Vector<bs::String> args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'list' executed!");
    bs::Vector<bs::String> outputs;

    outputs.push_back("List of all commands:");
    for (auto it = mCommands.begin(); it != mCommands.end(); it++)
    {
      bs::String command = it->first;

      outputs.push_back(command);
    }

    return outputs;
  }

  bs::Vector<bs::String> Console::command_Help(bs::Vector<bs::String> args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'help' executed!");
    bs::Vector<bs::String> outputs;

    bs::String& command = args.front();
    auto it             = mCommands.find(command);
    if (it == mCommands.end())
    {
      outputs.push_back("Unkown command: " + command);
    }
    else
    {
      bs::String usage = it->second.usage;
      outputs.push_back(usage);
      bs::String help = it->second.help;
      outputs.push_back(help);
    }

    return outputs;
  }

  bs::Vector<bs::String> Console::command_CheatFull(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'cheat full' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_CheatGod(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'cheat god' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_Insert(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'insert' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_Spawnmass(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'spawnmass' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_Kill(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'kill' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_EditAbilities(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'edit abilities' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_EditFocus(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'edit focus' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_SetTime(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'set time' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_GotoWaypoint(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'goto waypoint' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_GotoCamera(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'goto camera' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_GotoPos(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'goto pos' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_AIGoto(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'aigoto' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_SetClippingfactor(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized,
               "[Console] Command 'set clippingfactor' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_ZFogZone(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'zfogzone' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_ToggleConsole(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'toggle console' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_ToggleFrame(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'toggle frame' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_ToggleWaynet(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'toggle waynet' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_Firstperson(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'firstperson' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_HeroExport(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'hero export' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  bs::Vector<bs::String> Console::command_HeroImport(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'hero import' is not implemented yet!");
    bs::Vector<bs::String> outputs;

    return outputs;
  }

  void Console::registerCommand(const bs::String& name, Command command)
  {
    mCommands[name] = command;
  }

  void Console::registerAllCommand()
  {
    using This = Console;
    Command command;

    command = {(commandCallback)&This::command_List, 0, "Usage: list", "Lists all commands."};
    registerCommand("list", command);
    command = {(commandCallback)&This::command_Help, 1, "Usage: help [command]",
               "Prints out helpful information about the given command."};
    registerCommand("help", command);
    command = {(commandCallback)&This::command_CheatFull, 0, "Usage: cheat full", ""};
    registerCommand("cheat full", command);
    command = {(commandCallback)&This::command_CheatGod, 0, "Usage: cheat god", ""};
    registerCommand("cheat god", command);
    command = {(commandCallback)&This::command_Insert, 1, "Usage: insert [name]", ""};
    registerCommand("insert", command);
    command = {(commandCallback)&This::command_Spawnmass, 1, "Usage: spawnmass {giga} [amount]", ""};
    registerCommand("spawnmass", command);
    command = {(commandCallback)&This::command_Kill, 0, "Usage: kill", ""};
    registerCommand("kill", command);
    command = {(commandCallback)&This::command_EditAbilities, 0, "Usage: edit abilities", ""};
    registerCommand("edit abilities", command);
    command = {(commandCallback)&This::command_EditFocus, 0, "Usage: edit focus", ""};
    registerCommand("edit focus", command);
    command = {(commandCallback)&This::command_SetTime, 2, "Usage: set time [hh] [mm]", ""};
    registerCommand("set time", command);
    command = {(commandCallback)&This::command_GotoWaypoint, 1, "Usage: goto waypoint [waypoint]",
               ""};
    registerCommand("goto waypoint", command);
    command = {(commandCallback)&This::command_GotoCamera, 0, "Usage: goto camera", ""};
    registerCommand("goto camera", command);
    command = {(commandCallback)&This::command_GotoPos, 3, "Usage: goto pos [x] [y] [z]", ""};
    registerCommand("goto pos", command);
    command = {(commandCallback)&This::command_AIGoto, 1, "Usage: aigoto [waypoint]", ""};
    registerCommand("aigoto", command);
    command = {(commandCallback)&This::command_SetClippingfactor, 0, "Usage: set clippingfactor [f]",
               ""};
    registerCommand("set clippingfactor", command);
    command = {(commandCallback)&This::command_ZFogZone, 0, "Usage: zfogzone", ""};
    registerCommand("zfogzone", command);
    command = {(commandCallback)&This::command_ToggleConsole, 0, "Usage: toggle console", ""};
    registerCommand("toggle console", command);
    command = {(commandCallback)&This::command_ToggleFrame, 0, "Usage: toggle frame", ""};
    registerCommand("toggle frame", command);
    command = {(commandCallback)&This::command_ToggleWaynet, 0, "Usage: toggle waynet", ""};
    registerCommand("toggle waynet", command);
    command = {(commandCallback)&This::command_Firstperson, 0, "Usage: firstperson", ""};
    registerCommand("firstperson", command);
    command = {(commandCallback)&This::command_HeroExport, 1, "Usage: hero export [filename]", ""};
    registerCommand("hero export", command);
    command = {(commandCallback)&This::command_HeroImport, 1, "Usage: hero import [filename]", ""};
    registerCommand("hero import", command);
  }

  REGOTH_DEFINE_RTTI(Console)
}  // namespace REGoth
