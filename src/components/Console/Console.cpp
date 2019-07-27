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

  bs::Vector<bs::String> Console::onInputChanged(const bs::String& input)
  {
    bs::Vector<bs::String> suggestions = mAutoTries[TokenType::Command].findSuggestions(input);
    bs::String s;
    for (const auto& sugst : suggestions) s += sugst + " , ";
    BS_LOG(Info, Uncategorized, "[UIConsole] Following Suggestions were found: {0}", s);

    return suggestions;
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
        size_t num_of_args          = it->second.args.size();

        /* TODO: I just want to get rid of empty strings :) */
        /* TODO: first arg is skipped here or something ? */
        /*
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
        */
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
    /* Do some autocompletion/suggestion stuff with name */
    mAutoTries[TokenType::Command].insert(name);
    mCommands[name] = command;
  }

  void Console::registerAllCommand()
  {
    using This = Console;
    Command command;

    command = CommandBuilder()
                  .callback((commandCallback)&This::command_List)
                  .usage("Usage: list")
                  .help("Lists all commands.")
                  .build();
    registerCommand("list", command);

    command = CommandBuilder()
                  .callback(&This::command_Help)
                  .arg(TokenType::Command)
                  .usage("Usage: help [command]")
                  .help("Prints out helpful information about the given command.")
                  .build();
    registerCommand("help", command);

    command = CommandBuilder()
                  .callback(&This::command_CheatFull)
                  .usage("Usage: cheat full")
                  .help("")
                  .build();
    registerCommand("cheat full", command);

    command = CommandBuilder()
                  .callback(&This::command_CheatGod)
                  .usage("Usage: cheat god")
                  .help("")
                  .build();
    registerCommand("cheat god", command);

    command = CommandBuilder()
                  .callback(&This::command_Insert)
                  .arg(TokenType::Instance)
                  .usage("Usage: insert [name]")
                  .help("")
                  .build();
    registerCommand("insert", command);

    command = CommandBuilder()
                  .callback(&This::command_Spawnmass)
                  .arg(TokenType::Literal)
                  .usage("Usage: spawnmass {giga} [amount]")
                  .help("")
                  .build();
    registerCommand("spawnmass", command);

    command = CommandBuilder()
                  .callback(&This::command_Kill)
                  .usage("Usage: kill")
                  .help("Kill the NPC you have currently in focus")
                  .build();
    registerCommand("kill", command);

    command = CommandBuilder()
                  .callback(&This::command_EditAbilities)
                  .usage("Usage: edit abilities")
                  .help("")
                  .build();
    registerCommand("edit abilities", command);

    command = CommandBuilder()
                  .callback(&This::command_EditFocus)
                  .usage("Usage: edit focus")
                  .help("")
                  .build();
    registerCommand("edit focus", command);

    command = CommandBuilder()
                  .callback(&This::command_SetTime)
                  .arg(TokenType::Literal)
                  .arg(TokenType::Literal)
                  .usage("Usage: set time [hh] [mm]")
                  .help("")
                  .build();
    registerCommand("set time", command);

    command = CommandBuilder()
                  .callback(&This::command_GotoWaypoint)
                  .arg(TokenType::Waypoint)
                  .usage("Usage: goto waypoint [waypoint]")
                  .help("")
                  .build();
    registerCommand("goto waypoint", command);

    command = CommandBuilder()
                  .callback(&This::command_GotoCamera)
                  .usage("Usage: goto camera")
                  .help("")
                  .build();
    registerCommand("goto camera", command);

    command = CommandBuilder()
                  .callback(&This::command_GotoPos)
                  .arg(TokenType::Literal)
                  .arg(TokenType::Literal)
                  .arg(TokenType::Literal)
                  .usage("Usage: goto pos [x] [y] [z]")
                  .help("")
                  .build();
    registerCommand("goto pos", command);

    command = CommandBuilder()
                  .callback(&This::command_AIGoto)
                  .arg(TokenType::Waypoint)
                  .usage("Usage: aigoto [waypoint]")
                  .help("")
                  .build();
    registerCommand("aigoto", command);

    command = CommandBuilder()
                  .callback(&This::command_SetClippingfactor)
                  .usage("Usage: set clippingfactor [f]")
                  .help("")
                  .build();
    registerCommand("set clippingfactor", command);

    command = CommandBuilder()
                  .callback(&This::command_ZFogZone)
                  .usage("Usage: zfogzone")
                  .help("Some Fogzone stuff")
                  .build();
    registerCommand("zfogzone", command);

    command = CommandBuilder()
                  .callback(&This::command_ToggleConsole)
                  .usage("Usage: toggle console")
                  .help("")
                  .build();
    registerCommand("toggle console", command);

    command = CommandBuilder()
                  .callback(&This::command_ToggleFrame)
                  .usage("Usage: toggle frame")
                  .help("")
                  .build();
    registerCommand("toggle frame", command);

    command = CommandBuilder()
                  .callback(&This::command_ToggleWaynet)
                  .usage("Usage: toggle waynet")
                  .help("")
                  .build();
    registerCommand("toggle waynet", command);

    command = CommandBuilder()
                  .callback(&This::command_Firstperson)
                  .usage("Usage: firstperson")
                  .help("")
                  .build();
    registerCommand("firstperson", command);

    command = CommandBuilder()
                  .callback(&This::command_HeroExport)
                  .arg(TokenType::Literal)
                  .usage("Usage: hero export [filename]")
                  .help("")
                  .build();
    registerCommand("hero export", command);

    command = CommandBuilder()
                  .callback(&This::command_HeroImport)
                  .arg(TokenType::Literal)
                  .usage("Usage: hero import [filename]")
                  .help("")
                  .build();
    registerCommand("hero import", command);
  }

  REGOTH_DEFINE_RTTI(Console)
}  // namespace REGoth
