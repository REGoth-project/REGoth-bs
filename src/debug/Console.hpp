#pragma once

#include <BsPrerequisites.h>
#include <Utility/BsFlags.h>

// TODO: not sure how to do this without namespace bs. Would like to keep this type private to the Console class
namespace bs
{
    enum class PrerequisiteFlag
    {
      Nothing = 1<<0,
      NeedsGameWorld = 1<<1,
      NeedsHeroPlayer = 1<<2,
    };

    typedef Flags<PrerequisiteFlag> PrerequisiteFlags;
    BS_FLAGS_OPERATORS(PrerequisiteFlag)
}

namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  /**
   * This class handles the logical side of the in-game Console.
   * This is where the command functions are defined and are registered to the corresponding command.
   * Executes correct command based on textual input.
   */
  class Console
  {
  public:
    Console();
    virtual ~Console();

    /**
     * Identifies the correct command based on the input string and executes its callback.
     *
     * @note   To be used in the callback of the onInputConfirmed event of the inputBox of the
     *         ConsoleUI component.
     *
     * @param  input
     *           Current input - Needs to be provided manually by the ConsoleUI component.
     */
    void parseAndExecuteCommand(const bs::String& input);

  private:
    /**
     * Clears console output
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_Clear(bs::Vector<bs::String>& args);

    /**
     * Lists all commands that are registered to this Console.
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_List(bs::Vector<bs::String>& args);

    /**
     * Prints a 'helpful' help message (aka a short description) of the given command.
     *
     * @param  args
     *           Name of the command.
     */
    void command_Help(bs::Vector<bs::String>& args);

    /**
     * Saves and Unloads currently active world then loads the given world.
     *
     * @param  args
     *           ZEN World to load.
     */
    void command_Change_World(bs::Vector<bs::String>& args);

    /**
     * Adds the given item to the HeroPlayers inventory.
     *
     * @param  args
     *           First argument is for the item name to be given.
     *           Second optional argument is for amount to be added.
     */
    void command_Give(bs::Vector<bs::String>& args);

    struct Command; //forward declaration
    void registerCommand(const Command& command);

    /**
     * Registers all commands.
     */
    void registerAllCommand();

  public:
    bs::Event<void()> mOnClearOutput;
    bs::Event<void(const bs::String&)> mOnOutputChanged;

  private:
    HGameWorld mGameWorld;
    bs::Vector<Command> mCommands;
    using commandCallback = void (Console::*)(bs::Vector<bs::String>&);

    /**
     * A collection of all types of tokesn that can appear in a command like the command itself and
     * various types of arguments. Used to describe the arguments for a command and to have
     * 'context-aware' autocomplete suggestions.
     */
    enum class TokenType
    {
      Literal,
      Command,
      Instance,  // FIXME: Yeah I am not so sure about this in relation to the original commands
      Waypoint,
      Freepoint,
      World,
    };

    /**
     * @struct Command
     *
     * @var Command::callback
     *      Member 'callback' holds the function pointer to a method of Component Console.
     * @var Command::args
     *      Member 'args' is a Vector of TokenTypes that specifies how many arguments and of which
     * type the corresponding function in 'callback' takes
     * @var Command::usage
     *      Member 'usage' describes how this command is supposed to be used
     * @var Command::help
     *      Member 'help' gives a short description of what this command does
     */
    struct Command
    {
      bs::String name;
      commandCallback callback;
      bs::Vector<TokenType> args = {};
      bs::String usage;  // TODO: Maybe I can generate this from the args?
      bs::String help;
      bs::PrerequisiteFlags prerequisites = bs::PrerequisiteFlag::Nothing;
    };

    /**
     * This class is a simple buildern pattern implementation for the Command struct.
     * Makes the instantiation a lot more readable, since designated initializers are not available.
     */
    class CommandBuilder
    {
    public:
      CommandBuilder& name(const bs::String& name)
      {
        bs::String sanitized_name = name;
        bs::StringUtil::trim(sanitized_name);

        cmd.name = sanitized_name;
        return *this;
      }
      CommandBuilder& callback(commandCallback callback)
      {
        cmd.callback = callback;
        return *this;
      }
      CommandBuilder& arg(TokenType arg)
      {
        cmd.args.push_back(arg);
        return *this;
      }
      CommandBuilder& usage(bs::String usage)
      {
        cmd.usage = usage;
        return *this;
      }
      CommandBuilder& help(bs::String help)
      {
        cmd.help = help;
        return *this;
      }
      CommandBuilder& prerequisites(bs::PrerequisiteFlags prerequisites)
      {
        cmd.prerequisites = prerequisites;
        return *this;
      }
      Command build()
      {
        return cmd;
      }

    private:
      Command cmd = {};
    };

  };
}  // namespace REGoth
