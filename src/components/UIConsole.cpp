#include "UIConsole.hpp"
#include <GUI/BsGUIInputBox.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUILayoutY.h>
#include <GUI/BsGUIPanel.h>
#include <GUI/BsGUIScrollArea.h>
#include <GUI/BsGUITexture.h>
#include <RTTI/RTTI_UIConsole.hpp>
#include <String/BsString.h>
#include <log/logging.hpp>
#include <original-content/OriginalGameResources.hpp>

namespace REGoth
{
  UIConsole::UIConsole(const bs::HSceneObject& parent, HUIElement parentUiElement)
      : UIElement(parent, parentUiElement, new bs::GUIPanel())
  {
    setName("UIConsole");

    // TODO: Proper back/foreground, does not seem to work for mInputBox at all
    bs::GUIPanel* backgroundPanel = layout().addNewElement<bs::GUIPanel>(1);
    mBackground = backgroundPanel->addNewElement<bs::GUITexture>("GothicConsoleBackground");

    bs::GUIPanel* foregroundPanel = layout().addNewElement<bs::GUIPanel>(0);
    bs::GUILayoutY* layoutY       = foregroundPanel->addNewElement<bs::GUILayoutY>();

    mScrollArea = layoutY->addNewElement<bs::GUIScrollArea>();
    mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString("test"));

    mInputBox = layoutY->addNewElement<bs::GUIInputBox>(false, "GothicConsoleInputBox");
    mInputBox->setText("I am a console!");

    registerAllCommand();
  }

  UIConsole::~UIConsole()
  {
  }

  void UIConsole::onInitialized()
  {
    mToggleConsole = bs::VirtualButton("ToggleConsole");
    mConfirm       = bs::VirtualButton("Confirm");
  }

  void UIConsole::update()
  {
    UIElement::update();

    bs::Rect2I parentBounds = parentLayout().getBounds();

    layout().setPosition(0, 0);
    layout().setWidth(parentBounds.width);
    layout().setHeight(parentBounds.height * 0.2);

    // Activation Handling
    if (bs::gVirtualInput().isButtonDown(mToggleConsole))
    {
      mState = (mState == State::Closed) ? State::Open : State::Closed;
    }

    // State handling
    switch (mState)
    {
      case State::Closed:
        mBackground->setVisible(false);
        mScrollArea->setVisible(false);
        mInputBox->setVisible(false);
        break;

      case State::Open:
        mBackground->setVisible(true);
        mScrollArea->setVisible(true);
        mInputBox->setVisible(true);
        mInputBox->setFocus(true);  // TODO: Keep this?
        break;
    }

    // Input Handling
    if (mState == State::Open)
    {
      if (bs::gVirtualInput().isButtonDown(
              mConfirm))  // TODO: Actually might want to use the GUIInputBox callback? -> will still
                          // need this for autocompletion etc. though
      {
        // Process
        bs::String input = mInputBox->getText();
        bs::StringUtil::trim(input);
        // bs::Vector<bs::String> args = bs::StringUtil::split(input, bs::HString(" "));

        // bs::String command = args.at(0);
        // args.erase(args.begin());

        // TODO:
        // We can now call command(args I guess?)
        // maybe instead of this lets try startswith/match +replaceAll to match to the commands
        // instead?
        // command,callback map
        // loop through keys and try starts with on trimmed string
        // remove that key from string and do the callback

        for (auto it = mCommands.begin(); it != mCommands.end(); it++)
        {
          bs::String command = it->first;
          if (bs::StringUtil::startsWith(input, command))
          {
            BS_LOG(Info, Uncategorized, "[Console] {0} triggered!", command);
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
              bs::String output = (this->*it->second.callback)(args);
              mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(output));
            }
            else
            {
              bs::String usage = it->second.usage;
              mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(usage));
            }
            break;
          }
        }

        /*
              auto it = mCommands.find(command);
              if (it != mCommands.end())
              {
                bs::gDebug().logDebug("[Console] " + command + " triggered");
                bs::String output = (this->*it->second)(args);
                mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(output));
              }
      */
        // Clear
        mInputBox->setText("");
      }
    }
  }

  bs::String UIConsole::command_Dummy(bs::Vector<bs::String> args)
  {
    return bs::String("lol");
  }

  bs::String UIConsole::command_List(bs::Vector<bs::String> args)
  {
    mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString("List of all commands:"));
    for (auto it = mCommands.begin(); it != mCommands.end(); it++)
    {
      bs::String command = it->first;

      mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(command));
    }
    return bs::String("lol");
  }

  bs::String UIConsole::command_Help(bs::Vector<bs::String> args)
  {
    bs::String& command = args.front();
    auto it             = mCommands.find(command);
    if (it == mCommands.end())
    {
      return "Unkown command: " + command;
    }
    else
    {
      bs::String usage = it->second.usage;
      mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(usage));
      bs::String help = it->second.help;
      mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(help));
      return "lol";
    }
  }

  void UIConsole::registerCommand(const bs::String& name, Command command)
  {
    mCommands[name] = command;
  }

  void UIConsole::registerAllCommand()
  {
    using This = UIConsole;
    Command command;

    command = {(commandCallback)&This::command_List, 0, "Usage: list", "Lists all commands."};
    registerCommand("list", command);
    command = {(commandCallback)&This::command_Help, 1, "Usage: help [command]",
               "Prints out helpful information about the given command."};
    registerCommand("help", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: cheat full", ""};
    registerCommand("cheat full", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: cheat god", ""};
    registerCommand("cheat god", command);
    command = {(commandCallback)&This::command_Dummy, 1, "Usage: insert [name]", ""};
    registerCommand("insert", command);
    command = {(commandCallback)&This::command_Dummy, 1, "Usage: spawnmass {giga} [amount]", ""};
    registerCommand("spawnmass", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: kill", ""};
    registerCommand("kill", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: edit abilities", ""};
    registerCommand("edit abilities", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: edit focus", ""};
    registerCommand("edit focus", command);
    command = {(commandCallback)&This::command_Dummy, 2, "Usage: set time [hh] [mm]", ""};
    registerCommand("set time", command);
    command = {(commandCallback)&This::command_Dummy, 1, "Usage: goto waypoint [waypoint]", ""};
    registerCommand("goto waypoint", command);
    command = {(commandCallback)&This::command_Dummy, 1, "Usage: aigoto [waypoint]", ""};
    registerCommand("aigoto", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: goto camera", ""};
    registerCommand("goto camera", command);
    command = {(commandCallback)&This::command_Dummy, 3, "Usage: goto pos [x] [y] [z]", ""};
    registerCommand("goto pos", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: set clippingfactor [f]", ""};
    registerCommand("set clippingfactor", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: zfogzone", ""};
    registerCommand("zfogzone", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: toggle console", ""};
    registerCommand("toggle console", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: toggle frame", ""};
    registerCommand("toggle frame", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: toggle waynet", ""};
    registerCommand("toggle waynet", command);
    command = {(commandCallback)&This::command_Dummy, 0, "Usage: firstperson", ""};
    registerCommand("firstperson", command);
    command = {(commandCallback)&This::command_Dummy, 1, "Usage: hero export [filename]", ""};
    registerCommand("hero export", command);
    command = {(commandCallback)&This::command_Dummy, 1, "Usage: hero import [filename]", ""};
    registerCommand("hero import", command);
  }

  REGOTH_DEFINE_RTTI(UIConsole)
}  // namespace REGoth
