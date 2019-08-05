#pragma once
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class Console;
  using HConsole = bs::GameObjectHandle<Console>;

  class UIConsole;
  using HUIConsole = bs::GameObjectHandle<UIConsole>;

  /**
   * This class handles the logical side of the in-game Console.
   * This is where the command functions are defined and are registered to the corresponding command.
   * Handles history of executed command (TODO) and tries for autocomplete suggestions of the current
   * input supplied by the ConsoleUI component.
   * Executes correct command based on textual input.
   */
  class Console : public bs::Component
  {
    struct Command;  // forward declaration

  public:
    Console(const bs::HSceneObject& parent);
    virtual ~Console();

    /**
     * Keeps track of where we are in the current command to offer 'content-aware' autocomplete
     * suggestions.
     *
     * @return A vector of auto-complete suggestions to be displayed by the ConsoleUI component.
     *
     * @note   To be used in the callback of the inputChanged event of the inputBox of the ConsoleUI
     *         component.
     *
     * @param  input
     *           Current input - usually provided by the inputChanged event of the inputBox of the
     *           ConsoleUI component.
     */
    bs::Vector<bs::String> onInputChanged(const bs::String& input);

    /**
     * Identifies the correct command based on the input string and executes its callback.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @note   To be used in the callback of the onInputConfirmed event of the inputBox of the
     *         ConsoleUI component.
     *
     * @param  input
     *           Current input - Needs to be provided manually by the ConsoleUI component.
     */
    bs::Vector<bs::String> onCommandConfirmed(bs::String input);

  protected:
    void onInitialized() override;

  private:
    /**
     * Lists all commands that are registered to this Console.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_List(bs::Vector<bs::String> args);

    /**
     * Prints a 'helpful' help message (aka a short description) of the given command.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Name of the command.
     */
    bs::Vector<bs::String> command_Help(bs::Vector<bs::String> args);

    /**
     * Restores health and mana of the currently controlled(?) character.
     * TODO: currently controlled or PC_HERO only?
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_CheatFull(bs::Vector<bs::String> args);

    /**
     * Turns on god mode for the currently controlled(?) character.
     * TODO: currently controlled or PC_HERO only?
     * TODO: Is god mode only invulnerability?
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_CheatGod(bs::Vector<bs::String> args);

    /**
     * Insert the given Item/NPC infront of the currently controlled character.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Name of an item or NPC.
     * TODO: Anything else you can insert with this?
     */
    bs::Vector<bs::String> command_Insert(bs::Vector<bs::String> args);

    /**
     * Spawns (???) around(?) the currently controlled character-
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Amount of entities to spawn.
     */
    bs::Vector<bs::String> command_Spawnmass(bs::Vector<bs::String> args);

    /**
     * Kills target currently in focus.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_Kill(bs::Vector<bs::String> args);

    /**
     * Allows editing various attributes and abilities of the currently controlled character.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_EditAbilities(bs::Vector<bs::String> args);

    /**
     * Allows editing various attributes and abilities of the character currently in focus.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_EditFocus(bs::Vector<bs::String> args);

    /**
     * Sets the current time.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Two literal arguments in the form of [hh] [mm].
     */
    bs::Vector<bs::String> command_SetTime(bs::Vector<bs::String> args);

    /**
     * Teleport the currently controlled character to the given waypoint.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Waypoint to teleport to.
     */
    bs::Vector<bs::String> command_GotoWaypoint(bs::Vector<bs::String> args);

    /**
     * Teleports PC_HERO(?) to the free floating camera.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_GotoCamera(bs::Vector<bs::String> args);

    /**
     * Teleport the currently controlled character to the given position in the world.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Three literal arguments in the form of [x] [y] [z] to repesent a position.
     */
    bs::Vector<bs::String> command_GotoPos(bs::Vector<bs::String> args);

    /**
     * ???
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Waypoint.
     */
    bs::Vector<bs::String> command_AIGoto(bs::Vector<bs::String> args);

    /**
     * ???
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Literal float argument.
     */
    bs::Vector<bs::String> command_SetClippingfactor(bs::Vector<bs::String> args);

    /**
     * ???
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_ZFogZone(bs::Vector<bs::String> args);

    /**
     * Toggles the visuals of the console.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_ToggleConsole(bs::Vector<bs::String> args);

    /**
     * Toggles the visuals of the frame (whole gameui).
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_ToggleFrame(bs::Vector<bs::String> args);

    /**
     * Toggles the visuals of the waynet (debug).
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_ToggleWaynet(bs::Vector<bs::String> args);

    /**
     * Toggles firstperson mode.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Does not use any arguments.
     */
    bs::Vector<bs::String> command_Firstperson(bs::Vector<bs::String> args);

    /**
     * Exports attributes and abilities of currently controlled characters to a file with the given
     * name in the save folder.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Name of the file.
     */
    bs::Vector<bs::String> command_HeroExport(bs::Vector<bs::String> args);

    /**
     * Import attributes and abilities from the file with the given name in the save folder and apply
     * them to the currently controlled character.
     *
     * @return A vector of output messages to be displayed by the ConsoleUI component.
     *
     * @param  args
     *           Name of the file.
     */
    bs::Vector<bs::String> command_HeroImport(bs::Vector<bs::String> args);

    /**
     * Register a command by
     *
     * @param  name
     *           Name of the command to identify it in the ingame console.
     *
     * @param  command
     *           The command itself with its reference to the callback and other useful information.
     */
    void registerCommand(const bs::String& name, Command command);

    /**
     * Registers all commands.
     */
    void registerAllCommand();

  private:
    HUIConsole mConsoleUI;
    bs::Map<bs::String, Command> mCommands;
    using commandCallback = bs::Vector<bs::String> (Console::*)(bs::Vector<bs::String>);

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
      commandCallback callback;
      bs::Vector<TokenType> args = {};
      bs::String usage;  // TODO: Maybe I can generate this from the args?
      bs::String help;
    };

    /**
     * This class is a simple buildern pattern implementation for the Command struct.
     * Makes the instantiation a lot more readable, since designated initializers are not available.
     */
    class CommandBuilder
    {
    public:
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
      Command build()
      {
        return cmd;
      }

    private:
      Command cmd = {};
    };

  public:
    REGOTH_DECLARE_RTTI(Console)

  protected:
    Console() = default;  // For RTTI
  };
}  // namespace REGoth
