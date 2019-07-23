#pragma once
#include "UIElement.hpp"
#include <GUI/BsGUIInputBox.h>
#include <Input/BsVirtualInput.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class UIConsole;
  using HUIConsole = bs::GameObjectHandle<UIConsole>;

  class Console;
  using HConsole = bs::GameObjectHandle<Console>;

  class UIConsole : public UIElement
  {
  public:
    UIConsole(const bs::HSceneObject& parent, HUIElement parentUiElement);
    virtual ~UIConsole();
    void update() override;

    void clearInput();
    void setOutput(bs::Vector<bs::String> outputs);
    void setOutput(bs::String output);

  protected:
    void onInitialized() override;
    enum class State
    {
      Closed,
      Open,
    };

  private:
    HConsole mConsole;

    bs::GUITexture* mBackground    = nullptr;
    bs::GUIScrollArea* mScrollArea = nullptr;
    bs::GUIInputBox* mInputBox     = nullptr;
    bs::VirtualButton mToggleConsole;
    bs::VirtualButton mConfirm;
    State mState = State::Closed;

  public:
    REGOTH_DECLARE_RTTI(UIConsole)

  protected:
    UIConsole() = default;  // For RTTI
  };
}  // namespace REGoth
