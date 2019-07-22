#pragma once
#include "UIElement.hpp"
#include <GUI/BsGUIInputBox.h>
#include <Input/BsVirtualInput.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class UIConsole;
  using HUIConsole = bs::GameObjectHandle<UIConsole>;

  class UIConsole : public UIElement
  {
  public:
    UIConsole(const bs::HSceneObject& parent, HUIElement parentUiElement);
    virtual ~UIConsole();
    void update() override;

    bs::String getInput();
    void clearInput();
    void setOutput(bs::Vector<bs::String> outputs);
    void setOutput(bs::String output);

    bs::Event<void()>& onConfirmEvent()
    {
      return mInputBox->onConfirm;
    }
    bs::Event<void(const bs::String&)>& onValueChangedEvent()
    {
      return mInputBox->onValueChanged;
    }

  protected:
    void onInitialized() override;
    enum class State
    {
      Closed,
      Open,
    };

  private:
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
