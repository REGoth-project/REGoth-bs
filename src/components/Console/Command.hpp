#pragma once
#include "TokenType.hpp"
#include <BsPrerequisites.h>

namespace REGoth
{
  class Console;
  typedef bs::Vector<bs::String> (Console::*commandCallback)(bs::Vector<bs::String>);

  struct Command
  {
    commandCallback callback;
    bs::Vector<TokenType> args = {};
    bs::String usage;
    bs::String help;
  };

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
