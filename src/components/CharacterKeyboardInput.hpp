#pragma once
#include <BsPrerequisites.h>
#include <Input/BsVirtualInput.h>
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class CharacterAI;
  using HCharacterAI = bs::GameObjectHandle<CharacterAI>;

  /**
   * Component which queries keyboard input and makes the character it is attached to
   * move accordingly.
   */
  class CharacterKeyboardInput : public bs::Component
  {
  public:
    CharacterKeyboardInput(const bs::HSceneObject& parent);
    virtual ~CharacterKeyboardInput();


  protected:

    /** Triggered once per frame. Allows the component to handle input and move. */
    void fixedUpdate() override;
    void onInitialized() override;

  private:
    /**
     * Input key cache
     */
    bs::VirtualButton mMoveForward;
    bs::VirtualButton mMoveBack;
    bs::VirtualButton mMoveLeft;
    bs::VirtualButton mMoveRight;
    bs::VirtualButton mFastMove;
    bs::VirtualButton mAction;

    // Handle to the CharacterAI component attached to the scene object
    HCharacterAI mCharacterAI;

  public:
    REGOTH_DECLARE_RTTI(CharacterKeyboardInput);

  protected:
    CharacterKeyboardInput() = default;  // For RTTI
  };
}  // namespace REGoth
