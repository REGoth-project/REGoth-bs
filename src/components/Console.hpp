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
   * Executes correct command based on textual input.
   */
  class Console : public bs::Component
  {
  public:
    Console(const bs::HSceneObject& parent);
    virtual ~Console();

    /**
     * Keeps track of where we are in the current command to offer 'content-aware' autocomplete
     * suggestions.
     *
     * @note   To be used in the callback of the inputChanged event of the inputBox of the ConsoleUI
     *         component.
     *
     * @param  input
     *           Current input - usually provided by the inputChanged event of the inputBox of the
     *           ConsoleUI component.
     */
    void onInputChanged(const bs::String& input);

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

  protected:
    void onInitialized() override;

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
     * Restores health and mana of the currently controlled(?) character.
     * TODO: currently controlled or PC_HERO only?
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_CheatFull(bs::Vector<bs::String>& args);

    /**
     * Turns on god mode for the currently controlled(?) character.
     * TODO: currently controlled or PC_HERO only?
     * TODO: Is god mode only invulnerability?
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_CheatGod(bs::Vector<bs::String>& args);

    /**
     * Insert the given Item/NPC infront of the currently controlled character.
     *
     * @param  args
     *           Name of an item or NPC.
     * TODO: Anything else you can insert with this?
     */
    void command_Insert(bs::Vector<bs::String>& args);

    /**
     * Spawns (???) around(?) the currently controlled character-
     *
     * @param  args
     *           Amount of entities to spawn.
     */
    void command_SpawnMass(bs::Vector<bs::String>& args);

    /**
     * Kills target currently in focus.
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_Kill(bs::Vector<bs::String>& args);

    /**
     * Allows editing various attributes and abilities of the currently controlled character.
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_EditAbilities(bs::Vector<bs::String>& args);

    /**
     * Allows editing various attributes and abilities of the character currently in focus.
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_EditFocus(bs::Vector<bs::String>& args);

    /**
     * Sets the current time.
     *
     * @param  args
     *           Two literal arguments in the form of [hh] [mm].
     */
    void command_SetTime(bs::Vector<bs::String>& args);

    /**
     * Teleport the currently controlled character to the given waypoint.
     *
     * @param  args
     *           Waypoint to teleport to.
     */
    void command_GotoWaypoint(bs::Vector<bs::String>& args);

    /**
     * Teleports PC_HERO(?) to the free floating camera.
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_GotoCamera(bs::Vector<bs::String>& args);

    /**
     * Teleport the currently controlled character to the given position in the world.
     *
     * @param  args
     *           Three literal arguments in the form of [x] [y] [z] to repesent a position.
     */
    void command_GotoPos(bs::Vector<bs::String>& args);

    /**
     * ???
     *
     * @param  args
     *           Waypoint.
     */
    void command_AIGoto(bs::Vector<bs::String>& args);

    /**
     * ???
     *
     * @param  args
     *           Literal float argument.
     */
    void command_SetClippingfactor(bs::Vector<bs::String>& args);

    /**
     * ???
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_ZFogZone(bs::Vector<bs::String>& args);

    /**
     * Toggles the visuals of the console.
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_ToggleConsole(bs::Vector<bs::String>& args);

    /**
     * Toggles the visuals of the frame (whole gameui).
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_ToggleFrame(bs::Vector<bs::String>& args);

    /**
     * Toggles the visuals of the waynet (debug).
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_ToggleWaynet(bs::Vector<bs::String>& args);

    /**
     * Toggles firstperson mode.
     *
     * @param  args
     *           Does not use any arguments.
     */
    void command_Firstperson(bs::Vector<bs::String>& args);

    /**
     * Exports attributes and abilities of currently controlled characters to a file with the given
     * name in the save folder.
     *
     * @param  args
     *           Name of the file.
     */
    void command_HeroExport(bs::Vector<bs::String>& args);

    /**
     * Import attributes and abilities from the file with the given name in the save folder and apply
     * them to the currently controlled character.
     *
     * @param  args
     *           Name of the file.
     */
    void command_HeroImport(bs::Vector<bs::String>& args);

    struct Command;  // forward declaration
    /**
     * Register a command by
     *
     * @param  name
     *           Name of the command to identify it in the ingame console.
     *
     * @param  command
     *           The command itself with its reference to the callback and other useful information.
     */
    void registerCommand(const Command& command);

    /**
     * Registers all commands.
     */
    void registerAllCommand();

  private:
    HUIConsole mConsoleUI;
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
      bs::UINT32 num_of_tokens;
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
      CommandBuilder& name(const bs::String& name)
      {
        // sanitize name and determine num of tokens aka how many words in the command
        bs::String sanitized_name = name;
        bs::StringUtil::trim(sanitized_name);
        bs::UINT32 num_of_tokens = bs::StringUtil::split(sanitized_name, " ").size();

        cmd.name          = sanitized_name;
        cmd.num_of_tokens = num_of_tokens;
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
