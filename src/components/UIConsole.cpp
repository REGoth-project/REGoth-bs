#include "UIConsole.hpp"
#include <GUI/BsGUIInputBox.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUILayoutY.h>
#include <GUI/BsGUIPanel.h>
#include <GUI/BsGUIScrollArea.h>
#include <GUI/BsGUITexture.h>
#include <RTTI/RTTI_UIConsole.hpp>
#include <String/BsString.h>
#include <components/GameWorld.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameResources.hpp>

namespace REGoth
{
  UIConsole::UIConsole(const bs::HSceneObject& parent, HUIElement parentUIElement)
      : UIElement(parent, parentUIElement, new bs::GUIPanel())
  {
    setName("UIConsole");

    bs::GUILayoutY* layoutY = layout().addNewElement<bs::GUILayoutY>();

    bs::GUIPanel* topPanel        = layoutY->addNewElement<bs::GUIPanel>();
    bs::GUIPanel* backgroundPanel = topPanel->addNewElement<bs::GUIPanel>(1);
    mBackground = backgroundPanel->addNewElement<bs::GUITexture>("GothicConsoleBackground");

    bs::GUIPanel* foregroundPanel = topPanel->addNewElement<bs::GUIPanel>(0);
    mScrollArea                   = foregroundPanel->addNewElement<bs::GUIScrollArea>();

    // FIXME: Input box does not appear properly ontop of the texture so i moved it below everything
    mInputBox = layoutY->addNewElement<bs::GUIInputBox>(false, "GothicConsoleInputBox");
  }

  UIConsole::~UIConsole()
  {
  }

  void UIConsole::onInitialized()
  {
    mOnConfirm      = mInputBox->onConfirm;
    mOnInputChanged = mInputBox->onValueChanged;
    mToggleConsole  = bs::VirtualButton("ToggleConsole");

    setOutput("Welcome to the REGoth Console :)");
  }

  void UIConsole::update()
  {
    UIElement::update();

    bs::Rect2I parentBounds = parentLayout().getBounds();

    layout().setPosition(0, 0);
    // TODO: Consider whole screen for this layout because of suggestion box and edit box
    layout().setWidth(parentBounds.width);
    layout().setHeight(parentBounds.height * 0.2);

    // Activation Input Handling
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

    if (mState == State::Open)
    {
      // Input Handling
    }
  }

  const bs::String& UIConsole::getInput()
  {
    return mInputBox->getText();
  }

  void UIConsole::clearInput()
  {
    mInputBox->setText("");
  }

  void UIConsole::setOutput(const bs::Vector<bs::String>& outputs)
  {
    for (auto output : outputs)
    {
      setOutput(output);
    }
  }

  void UIConsole::setOutput(const bs::String& output)
  {
    auto label = mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(output));
    label->setHeight(20);
    mOutputLabels.push_back(label);

    mScrollArea->_updateLayout(mScrollArea->_getLayoutData());
    mScrollArea->scrollDownPct(1.0);  // Move scrollbar to the very bottom ; FIXME: Does not work
                                      // correctly, call above is workaround
  }

  void UIConsole::clearOutput()
  {
    // TODO: does not work correctly
    for (bs::GUILabel* pLabel : mOutputLabels)
    {
      mScrollArea->getLayout().removeElement(pLabel);
    }

    mOutputLabels.clear();
  }

  REGOTH_DEFINE_RTTI(UIConsole)
}  // namespace REGoth
