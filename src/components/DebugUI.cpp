#include "DebugUI.hpp"
#include <RTTI/RTTI_DebugUI.hpp>
#include <components/Debug.hpp>
#include <components/UIConsole.hpp>
#include <Input/BsInput.h>
#include <log/logging.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  static HDebugUI s_DebugUI;

  DebugUI::DebugUI(const bs::HSceneObject& parent, bs::HCamera camera)
      : UIElement(parent, camera)
  {
    setName("DebugUI");
  }

  DebugUI::~DebugUI()
  {
  }

  void DebugUI::onInitialized()
  {
    UIElement::onInitialized();

    mToggleConsole  = bs::VirtualButton("ToggleConsole");

    if (!mDebug)
    {
      mDebug = SO()->addComponent<Debug>();
      //mDebug->SO()->setActive(false); TODO: Set to inactive by default at some point
    }

    if (!mConsoleUI)
    {
      mConsoleUI = addChildElement<UIConsole>("UIConsole");
    }

    //Input entry event
    auto entryExitCallback = [&](const bs::TextInputEvent& eventInput) {
      //Disable when console is open
      if(mConsoleUI->isOpen()) return;

      char input = (char)eventInput.textChar;
      REGOTH_LOG(Info, Uncategorized, "[DebugUI] !EVENT! Input with: {0}!", input);
      //TODO: marvin mode -> set mDebug SO to active("marvin")/inactive("42"), see previous TODO
    };
    bs::gInput().onCharInput.connect(entryExitCallback);
  }

  void DebugUI::createGlobal(bs::HCamera camera)
  {
    if (s_DebugUI)
    {
      REGOTH_THROW(InvalidStateException, "DebugUI has already been created!");
    }

    auto so = bs::SceneObject::create("DebugUI");

    s_DebugUI = so->addComponent<DebugUI>(camera);
  }

  void DebugUI::update()
  {
    UIElement::update();

    if (mDebug->SO()->getActive() && bs::gVirtualInput().isButtonDown(mToggleConsole))
    {
      mConsoleUI->toggle();
    }
  }

  HDebugUI gDebugUI()
  {
    if (!s_DebugUI)
    {
      REGOTH_THROW(InvalidStateException, "DebugUI::createGlobal() has not been called before!");
    }

    return s_DebugUI;
  }

  REGOTH_DEFINE_RTTI(DebugUI)
}  // namespace REGoth
