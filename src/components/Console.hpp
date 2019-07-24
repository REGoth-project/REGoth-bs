#pragma once
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class Console;
  using HConsole = bs::GameObjectHandle<Console>;

  class Console : public bs::Component
  {
  public:
    Console(const bs::HSceneObject& parent);
    virtual ~Console();

    void onInitialized() override;
    void onInputChanged(const bs::String& input);
    bs::Vector<bs::String> onCommandConfirmed(bs::String input);

  private:
    /* Command stuff */
    typedef bs::Vector<bs::String> (Console::*commandCallback)(bs::Vector<bs::String>);
    struct Command
    {
      commandCallback callback;
      size_t num_of_args;
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
      CommandBuilder& num_of_args(size_t num_of_args)
      {
        cmd.num_of_args = num_of_args;
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
      Command cmd;
    };
    bs::Map<bs::String, Command> mCommands;
    bs::Vector<bs::String> command_List(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_Help(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_CheatFull(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_CheatGod(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_Insert(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_Spawnmass(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_Kill(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_EditAbilities(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_EditFocus(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_SetTime(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_GotoWaypoint(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_GotoCamera(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_GotoPos(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_AIGoto(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_SetClippingfactor(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_ZFogZone(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_ToggleConsole(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_ToggleFrame(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_ToggleWaynet(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_Firstperson(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_HeroExport(bs::Vector<bs::String> args);
    bs::Vector<bs::String> command_HeroImport(bs::Vector<bs::String> args);
    void registerCommand(const bs::String& name, Command command);
    void registerAllCommand();

    /* Trie stuff */
  private:
    class Trie
    {
    public:
      Trie()
      {
        root.endOfWord = false;
        root.subWord   = "";
      }

      void insert(const bs::String& word)
      {
        Node curr = root;

        for (int i = 0; i < word.size(); i++)
        {
          char c = word.at(i);
          if (curr.children.find(c) == curr.children.end())
          {
            Node newNode;
            newNode.endOfWord = false;
            newNode.subWord   = curr.subWord + c;
            curr.children[c]  = newNode;
          }
          curr = curr.children[c];
        }
        curr.endOfWord = true;
      }

    private:
      struct Node
      {
        bs::Map<char, Node> children;
        bool endOfWord;
        bs::String subWord;
      };
      Node root;
    };
    Trie mAutotrie;

  public:
    REGOTH_DECLARE_RTTI(Console)

  protected:
    Console() = default;  // For RTTI
  };
}  // namespace REGoth
