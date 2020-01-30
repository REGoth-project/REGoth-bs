#include "Console.hpp"
#include "components/Character.hpp"
#include "components/GameClock.hpp"
#include "components/GameWorld.hpp"
#include "components/Inventory.hpp"
#include <String/BsString.h>
#include <log/logging.hpp>

namespace REGoth
{
  Console::Console()
  {
    registerAllCommand();
  }

  Console::~Console()
  {
  }

  void Console::parseAndExecuteCommand(const bs::String& input)
  {
    bs::String sanitized_input = input;
    bs::StringUtil::trim(sanitized_input);
    bs::Vector<bs::String> tokenized_input = bs::StringUtil::split(sanitized_input, " ");

    if(tokenized_input.empty()) return; // TODO: feedback

    for (const auto& token : tokenized_input)
      REGOTH_LOG(Info, Uncategorized, "[Console] Token in tokenized input: {0}!", token);

    // detect command
    auto it = mCommands.begin();
    for (; it != mCommands.end(); it++)
    {
      bs::String command;
      command = tokenized_input[0]; // Command is always one token at the beginning of the input for now

      // if the command matches the input we just break and it will contain the correct command
      if (bs::StringUtil::compare(command, it->name) == 0) break;
    }

    // Command not found
    if (it == mCommands.end())
    {
      mOnOutputChanged("Unkown command!"); //TODO mention command
      return;
    }
    REGOTH_LOG(Info, Uncategorized, "[Console] Command was found and is: {0}!", it->name);

    // call callback and pass arguments
    tokenized_input.erase(tokenized_input.begin(), tokenized_input.begin() + 1);
    // FIXME: allow trailing arguments that are ignored for now
    if (tokenized_input.size() < it->args.size())
    {
      REGOTH_LOG(Info, Uncategorized,
                 "[Console] Too few arguments: Should be {0}, but is {1}!", it->args.size(),
                 tokenized_input.size());
      return;
    }

    if(it->prerequisites.isSet(bs::PrerequisiteFlag::NeedsGameWorld) && !mGameWorld)
    {
      mOnOutputChanged("Command needs GameWorld, but no GameWorld is present.");
      return;
    }

    if(it->prerequisites.isSet(bs::PrerequisiteFlag::NeedsHeroPlayer) && (!mGameWorld || !mGameWorld->hero()))
    {
      mOnOutputChanged("Command needs HeroPlayer, but no HeroPlayer is present.");
      return;
    }

    (this->*it->callback)(tokenized_input);
  }

  void Console::command_Clear(bs::Vector<bs::String>& args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'clear' executed!");

    mOnClearOutput();
  }

  void Console::command_List(bs::Vector<bs::String>& args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'list' executed!");

    mOnOutputChanged("List of all commands:");
    for (auto it = mCommands.begin(); it != mCommands.end(); it++)
    {
      const bs::String& command = it->name;

      mOnOutputChanged(command);
    }
  }

  void Console::command_Help(bs::Vector<bs::String>& args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'help' executed!");

    // detect command
    auto it = mCommands.begin();
    for (; it != mCommands.end(); it++)
    {
      bs::String command;
      command = args[0];

      // if the command matches the input we just break and it will contain the correct command
      if (bs::StringUtil::compare(command, it->name) == 0) break;
    }

    if (it == mCommands.end())
    {
      mOnOutputChanged("Unkown command!"); //TODO mention command
    }
    else
    {
      const bs::String& usage = it->usage;
      mOnOutputChanged(usage);
      const bs::String& help = it->help;
      mOnOutputChanged(help);
    }
  }

  void Console::command_Change_World(bs::Vector<bs::String>& args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'change_world' executed!");
  }

  void Console::command_Give(bs::Vector<bs::String>& args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'give' executed!");
    
    bs::String& instance = args[0];
    bs::StringUtil::replaceAll(instance, "_", "");
    bs::StringUtil::toUpperCase(instance);
    const bs::UINT32 amount = bs::parseINT32(args[1]);

    auto inventory = mGameWorld->hero()->SO()->getComponent<Inventory>();
    inventory->giveItem(instance, amount);

    const bs::String& output = bs::StringUtil::format("Gave {0} {1} to hero.", instance, amount);
    mOnOutputChanged(output);
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
                  .name("change_world")
                  .callback(&This::command_Change_World)
                  .arg(TokenType::World)
                  .usage("Usage: change_world [world]")
                  .help("...")
                  .build();
    registerCommand(command);

    command = CommandBuilder()
                  .name("give")
                  .callback(&This::command_Give)
                  .arg(TokenType::Instance)
                  .arg(TokenType::Literal)
                  .usage("Usage: give [item_name] {amount}")
                  .help("...")
                  .prerequisites(bs::PrerequisiteFlag::NeedsHeroPlayer)
                  .build();
    registerCommand(command);
  }
}  // namespace REGoth
