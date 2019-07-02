#pragma once
#include <BsPrerequisites.h>
#include <Input/BsVirtualInput.h>
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class CharacterAI;
  using HCharacterAI = bs::GameObjectHandle<CharacterAI>;

  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  class CharacterEventQueue;
  using HCharacterEventQueue = bs::GameObjectHandle<CharacterEventQueue>;

  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  /**
   * Component which queries keyboard input and makes the character it is attached to
   * move accordingly.
   */
  class CharacterKeyboardInput : public bs::Component
  {
  public:
    CharacterKeyboardInput(const bs::HSceneObject& parent, HGameWorld world);
    virtual ~CharacterKeyboardInput();


  protected:

    /** Triggered once per frame. Allows the component to handle input and move. */
    void fixedUpdate() override;
    void update() override;
    void onInitialized() override;

  private:
    /**
     * Input key cache
     */
    bs::VirtualButton mMoveForward;
    bs::VirtualButton mMoveBack;
    bs::VirtualButton mTurnLeft;
    bs::VirtualButton mTurnRight;
    bs::VirtualButton mStrafeLeft;
    bs::VirtualButton mStrafeRight;
    bs::VirtualButton mFastMove;
    bs::VirtualButton mToggleWalking;
    bs::VirtualButton mToggleSneaking;
    bs::VirtualButton mAction;
    bs::VirtualButton mQuickSave;

    // Handle to the CharacterAI component attached to the scene object
    HCharacter mCharacter;
    HCharacterAI mCharacterAI;
    HCharacterEventQueue mEventQueue;
    HGameWorld mWorld;

  public:
    REGOTH_DECLARE_RTTI(CharacterKeyboardInput);

  protected:
    CharacterKeyboardInput() = default;  // For RTTI
  };
}  // namespace REGoth
