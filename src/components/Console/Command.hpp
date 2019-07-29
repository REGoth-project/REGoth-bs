#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  class Console;
  typedef bs::Vector<bs::String> (Console::*commandCallback)(bs::Vector<bs::String>);

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
   *      Member 'args' is a Vector of TokenTypes that specifies how many arguments and of which type
   *      the corresponding function in 'callback' takes
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
}  // namespace REGoth
