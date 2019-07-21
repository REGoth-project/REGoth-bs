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

    auto handleValueChanged = [&](const bs::String& change) {
      BS_LOG(Info, Uncategorized, "[Console] Input changed! Current input: {0}!", change);
    };
    mInputBox->onValueChanged.connect(handleValueChanged);

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
        mInputBox->setFocus(false);
        break;

      case State::Open:
        mBackground->setVisible(true);
        mScrollArea->setVisible(true);
        mInputBox->setVisible(true);
        mInputBox->setFocus(true);
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

#if 0
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
              (this->*it->second.callback)(args);;
            }
            else
            {
              bs::String usage = it->second.usage;
              mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(usage));
            }
            break;
          }
        }
#endif

#if 1
        // 2*O(1) solution, I dont know
        bs::Vector<bs::String> args = bs::StringUtil::split(input, bs::HString(" "));
        bs::String command          = args.at(0);

        // first pass for single word commands
        auto it = mCommands.find(command);
        if (it != mCommands.end())
        {
          args.erase(args.begin());
          (this->*it->second.callback)(args);
        }
        // second pass for two word commands
        else if (args.size() >= 2)
        {
          command = command + " " + args.at(1);
          args.erase(args.begin(), args.begin() + 2);

          it = mCommands.find(command);
          if (it != mCommands.end())
          {
            (this->*it->second.callback)(args);
          }
        }
#endif

        // Clear
        mInputBox->setText("");
      }
    }
  }

  void UIConsole::command_List(bs::Vector<bs::String> args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'list' executed!");

    mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString("List of all commands:"));
    for (auto it = mCommands.begin(); it != mCommands.end(); it++)
    {
      bs::String command = it->first;

      mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(command));
    }
  }

  void UIConsole::command_Help(bs::Vector<bs::String> args)
  {
    REGOTH_LOG(Info, Uncategorized, "[Console] Command 'help' executed!");

    bs::String& command = args.front();
    auto it             = mCommands.find(command);
    if (it == mCommands.end())
    {
      mScrollArea->getLayout().addNewElement<bs::GUILabel>(
          bs::HString("Unkown command: " + command));
    }
    else
    {
      bs::String usage = it->second.usage;
      mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(usage));
      bs::String help = it->second.help;
      mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(help));
    }
  }

  void UIConsole::command_CheatFull(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'cheat full' is not implemented yet!");
  }

  void UIConsole::command_CheatGod(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'cheat god' is not implemented yet!");
  }

  void UIConsole::command_Insert(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'insert' is not implemented yet!");
  }

  void UIConsole::command_Spawnmass(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'spawnmass' is not implemented yet!");
  }

  void UIConsole::command_Kill(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'kill' is not implemented yet!");
  }

  void UIConsole::command_EditAbilities(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'edit abilities' is not implemented yet!");
  }

  void UIConsole::command_EditFocus(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'edit focus' is not implemented yet!");
  }

  void UIConsole::command_SetTime(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'set time' is not implemented yet!");
  }

  void UIConsole::command_GotoWaypoint(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'goto waypoint' is not implemented yet!");
  }

  void UIConsole::command_GotoCamera(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'goto camera' is not implemented yet!");
  }

  void UIConsole::command_GotoPos(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'goto pos' is not implemented yet!");
  }

  void UIConsole::command_AIGoto(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'aigoto' is not implemented yet!");
  }

  void UIConsole::command_SetClippingfactor(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized,
               "[Console] Command 'set clippingfactor' is not implemented yet!");
  }

  void UIConsole::command_ZFogZone(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'zfogzone' is not implemented yet!");
  }

  void UIConsole::command_ToggleConsole(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'toggle console' is not implemented yet!");
  }

  void UIConsole::command_ToggleFrame(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'toggle frame' is not implemented yet!");
  }

  void UIConsole::command_ToggleWaynet(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'toggle waynet' is not implemented yet!");
  }

  void UIConsole::command_Firstperson(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'firstperson' is not implemented yet!");
  }

  void UIConsole::command_HeroExport(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'hero export' is not implemented yet!");
  }

  void UIConsole::command_HeroImport(bs::Vector<bs::String> args)
  {
    // TODO: implement
    REGOTH_LOG(Warning, Uncategorized, "[Console] Command 'hero import' is not implemented yet!");
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

  REGOTH_DEFINE_RTTI(UIConsole)
}  // namespace REGoth
