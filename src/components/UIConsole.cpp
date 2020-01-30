#include "UIConsole.hpp"
#include <debug/Console.hpp>
#include <GUI/BsGUIPanel.h>
#include <GUI/BsGUILayoutY.h>
#include <RTTI/RTTI_UIConsole.hpp>
#include <log/logging.hpp>

namespace REGoth
{
  /**
   * How fast the box should be growing/shrinking
   */
  constexpr float POSITION_CHANGE_SPEED = 10.0f;

  UIConsole::UIConsole(const bs::HSceneObject& parent, HUIElement parentUiElement)
      : UIElement(parent, parentUiElement, new bs::GUIPanel())
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
    auto confirmCallback = [&]() {
      const bs::String& input = mInputBox->getText();
      REGOTH_LOG(Info, Uncategorized, "[Console] !EVENT! Command confirmed: {0}!", input);
      mConsole.parseAndExecuteCommand(input);
      clearInput();
    };
    mInputBox->onConfirm.connect(confirmCallback);

    auto clearCallback = [&]() {
      REGOTH_LOG(Info, Uncategorized, "[Console] !EVENT! Output cleared!");
      clearOutput();
    };
    mConsole.mOnClearOutput.connect(clearCallback);

    auto outputCallback = [&](const bs::String& output) {
      REGOTH_LOG(Info, Uncategorized, "[Console] !EVENT! Output with: {0}!", output);
      setOutput(output);
    };
    mConsole.mOnOutputChanged.connect(outputCallback);
  }

  void UIConsole::update()
  {
    UIElement::update();

    bs::Rect2I parentBounds = parentLayout().getBounds();

    //TODO: Consider whole screen for this layout because of suggestion box and edit box
    float width = parentBounds.width;
    float height = parentBounds.height/2;
    layout().setWidth(width);
    layout().setHeight(height);

    //Scroll in console from offscreen
    layout().setPosition(0, (mVisibleRatio-1.0f)*height);
    
    switch (mState)
    {
      case State::Closed:
        mVisibleRatio = 0.0f;
        mBackground->setVisible(false);
        mScrollArea->setVisible(false);
        mInputBox->setVisible(false);
        mInputBox->setFocus(false);
        break;

      case State::Growing:
        mVisibleRatio =
            std::min(1.0f, mVisibleRatio + bs::gTime().getFrameDelta() * POSITION_CHANGE_SPEED);

        mBackground->setVisible(true);
        mScrollArea->setVisible(true);
        mInputBox->setVisible(true);

        if (mVisibleRatio >= 1.0f)
        {
          mState = State::Open;
        }
        break;

      case State::Open:
        mVisibleRatio = 1.0f;
        mInputBox->setFocus(true);
        break;

      case State::Shrinking:
        mVisibleRatio =
            std::max(0.0f, mVisibleRatio - bs::gTime().getFrameDelta() * POSITION_CHANGE_SPEED);

        if (mVisibleRatio <= 0.0f)
        {
          mState = State::Closed;
        }

        break;
    }
  }

  void UIConsole::toggle()
  {
    if (mState == State::Closed)
    {
      REGOTH_LOG(Info, Uncategorized, "[Console] Open Console!");
      mState = State::Growing;
    }

    if (mState == State::Open)
    {
      REGOTH_LOG(Info, Uncategorized, "[Console] Close Console!");
      mState = State::Shrinking;
    }
  }

  bool UIConsole::isOpen()
  {
    return (mState != State::Closed);
  }

  void UIConsole::clearInput()
  {
    mInputBox->setText("");
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

  void UIConsole::setOutput(const bs::String& output)
  {
    auto label = mScrollArea->getLayout().addNewElement<bs::GUILabel>(bs::HString(output));
    label->setHeight(20);
    mOutputLabels.push_back(label);

    mScrollArea->_updateLayout(mScrollArea->_getLayoutData());
    mScrollArea->scrollDownPct(1.0);  // Move scrollbar to the very bottom ; FIXME: Does not work
                                      // correctly, call above is workaround
  }

  REGOTH_DEFINE_RTTI(UIConsole)
}  // namespace REGoth
