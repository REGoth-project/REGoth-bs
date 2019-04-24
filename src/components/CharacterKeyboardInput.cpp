#include "CharacterKeyboardInput.hpp"
#include <RTTI/RTTI_CharacterKeyboardInput.hpp>
#include <components/CharacterAI.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  CharacterKeyboardInput::CharacterKeyboardInput(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("CharacterKeyboardInput");

    mMoveForward = bs::VirtualButton("Forward");
    mMoveBack    = bs::VirtualButton("Back");
    mMoveLeft    = bs::VirtualButton("Left");
    mMoveRight   = bs::VirtualButton("Right");
    mFastMove    = bs::VirtualButton("FastMove");

    mCharacterAI = SO()->getComponent<CharacterAI>();

    if (mCharacterAI.isDestroyed())
    {
      REGOTH_THROW(InvalidStateException,
                   bs::StringUtil::format("Scene Object {0} does not have a CharacterAI component!",
                                          SO()->getName()));
    }
  }

  CharacterKeyboardInput::~CharacterKeyboardInput()
  {
  }

  void CharacterKeyboardInput::fixedUpdate()
  {
    bool goingForward = bs::gVirtualInput().isButtonHeld(mMoveForward);
    bool goingBack    = bs::gVirtualInput().isButtonHeld(mMoveBack);
    bool goingLeft    = bs::gVirtualInput().isButtonHeld(mMoveLeft);
    bool goingRight   = bs::gVirtualInput().isButtonHeld(mMoveRight);
    bool fastMove     = bs::gVirtualInput().isButtonHeld(mFastMove);

    if (goingForward)
    {
      mCharacterAI->goForward();
    }
    else if (goingBack)
    {
      mCharacterAI->goBackward();
    }
    else
    {
      mCharacterAI->stopMoving();
    }

    if (goingLeft)
    {
      mCharacterAI->turnLeft();
    }
    else if (goingRight)
    {
      mCharacterAI->turnRight();
    }
    else
    {
      mCharacterAI->stopTurning();
    }
  }

  bs::RTTITypeBase* CharacterKeyboardInput::getRTTIStatic()
  {
    return RTTI_CharacterKeyboardInput::instance();
  }

  bs::RTTITypeBase* CharacterKeyboardInput::getRTTI() const
  {
    return CharacterKeyboardInput::getRTTIStatic();
  }
}  // namespace REGoth
