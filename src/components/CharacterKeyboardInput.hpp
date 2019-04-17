#pragma once
#include <BsPrerequisites.h>
#include <Input/BsVirtualInput.h>
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

    /** Triggered once per frame. Allows the component to handle input and move. */
    void fixedUpdate() override;

  private:
    /**
     * Input key cache
     */
    bs::VirtualButton mMoveForward;
    bs::VirtualButton mMoveBack;
    bs::VirtualButton mMoveLeft;
    bs::VirtualButton mMoveRight;
    bs::VirtualButton mFastMove;

    // Handle to the CharacterAI component attached to the scene object
    HCharacterAI mCharacterAI;

    /************************************************************************/
    /* RTTI                                                                 */
    /************************************************************************/
  public:
    friend class RTTI_CharacterKeyboardInput;
    static bs::RTTITypeBase* getRTTIStatic();
    bs::RTTITypeBase* getRTTI() const override;

    // protected:
  public:  // FIXME: Should be protected, it is only used by RTTI but friend doesn't seem to work?!
    CharacterKeyboardInput() = default;  // Serialization only
  };
}  // namespace REGoth
