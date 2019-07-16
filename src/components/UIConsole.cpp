#include "UIConsole.hpp"
#include <GUI/BsGUIInputBox.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUILayoutY.h>
#include <GUI/BsGUIPanel.h>
#include <GUI/BsGUIScrollArea.h>
#include <GUI/BsGUITexture.h>
#include <RTTI/RTTI_UIConsole.hpp>
#include <String/BsString.h>
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
        bs::Vector<bs::String> args = bs::StringUtil::split(input, bs::HString(" "));

        bs::String command = args.at(0);
        args.erase(args.begin());

        // TODO:
        // We can now call command(args I guess?)
        // maybe instead of this lets try startswith/match +replaceAll to match to the commands
        // instead?
        // command,callback map
        // loop through keys and try starts with on trimmed string
        // remove that key from string and do the callback

        auto it = mCommands.find(command);  // TODO: This always returns end for some reason
        if (it != mCommands.end())
        {
          bs::gDebug().logDebug("[Console] " + command + " triggered");
          bs::String output = (this->*it->second)(args);
          mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString("test"));
        }

        // Clear
        mInputBox->setText("");
      }
    }
  }

  bs::String UIConsole::command_Dummy(bs::Vector<bs::String> args)
  {
    return bs::String("lol");
  }

  void UIConsole::registerCommand(const bs::String& name, commandCallback callback)
  {
    mCommands[name] = callback;
  }

  void UIConsole::registerAllCommand()
  {
    using This = UIConsole;

    registerCommand("cheat full", (commandCallback)&This::command_Dummy);
    registerCommand("cheat god", (commandCallback)&This::command_Dummy);
    registerCommand("insert", (commandCallback)&This::command_Dummy);
    registerCommand("spawnmass", (commandCallback)&This::command_Dummy); /* giga? */
    registerCommand("kill", (commandCallback)&This::command_Dummy);
    registerCommand("edit abilities", (commandCallback)&This::command_Dummy);
    registerCommand("edit focus", (commandCallback)&This::command_Dummy);
    registerCommand("set time", (commandCallback)&This::command_Dummy);
    registerCommand("goto waypoint", (commandCallback)&This::command_Dummy);
    registerCommand("aigoto", (commandCallback)&This::command_Dummy);
    registerCommand("goto camera", (commandCallback)&This::command_Dummy);
    registerCommand("goto pos", (commandCallback)&This::command_Dummy);
    registerCommand("set clippingfactor", (commandCallback)&This::command_Dummy);
    registerCommand("zfogzone", (commandCallback)&This::command_Dummy);
    registerCommand("toggle console", (commandCallback)&This::command_Dummy);
    registerCommand("toggle frame", (commandCallback)&This::command_Dummy);
    registerCommand("toggle waynet", (commandCallback)&This::command_Dummy);
    registerCommand("firstperson", (commandCallback)&This::command_Dummy);
    registerCommand("hero export", (commandCallback)&This::command_Dummy);
    registerCommand("hero import", (commandCallback)&This::command_Dummy);
  }

  REGOTH_DEFINE_RTTI(UIConsole)
}  // namespace REGoth
