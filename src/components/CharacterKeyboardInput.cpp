#include "CharacterKeyboardInput.hpp"
#include <components/Character.hpp>
#include <RTTI/RTTI_CharacterKeyboardInput.hpp>
#include <components/CharacterEventQueue.hpp>
#include <components/CharacterAI.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  CharacterKeyboardInput::CharacterKeyboardInput(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("CharacterKeyboardInput");
  }

  CharacterKeyboardInput::~CharacterKeyboardInput()
  {
  }

  void CharacterKeyboardInput::onInitialized()
  {
    bs::Component::onInitialized();

    mMoveForward = bs::VirtualButton("Forward");
    mMoveBack    = bs::VirtualButton("Back");
    mMoveLeft    = bs::VirtualButton("Left");
    mMoveRight   = bs::VirtualButton("Right");
    mFastMove    = bs::VirtualButton("FastMove");
    mAction      = bs::VirtualButton("Action");

    mCharacter   = SO()->getComponent<Character>();

    if (mCharacter.isDestroyed())
    {
      REGOTH_THROW(InvalidStateException,
                   bs::StringUtil::format("Scene Object {0} does not have a Character component!",
                                          SO()->getName()));
    }

    mCharacterAI = SO()->getComponent<CharacterAI>();

    if (mCharacterAI.isDestroyed())
    {
      REGOTH_THROW(InvalidStateException,
                   bs::StringUtil::format("Scene Object {0} does not have a CharacterAI component!",
                                          SO()->getName()));
    }

    mEventQueue = SO()->getComponent<CharacterEventQueue>();

    if (mEventQueue.isDestroyed())
    {
      REGOTH_THROW(
          InvalidStateException,
          bs::StringUtil::format("Scene Object {0} does not have a CharacterEventQueue component!",
                                 SO()->getName()));
    }
  }

  void CharacterKeyboardInput::update()
  {
    bool action = bs::gVirtualInput().isButtonDown(mAction);

    if (action)
    {
      auto thisCharacter = SO()->getComponent<Character>();

      // TODO: Proper implementation of using focusable things
      auto characters = mCharacter->findCharactersInRange(2.0f);

        for (HCharacter c : characters)
        {
          // Skip self
          if (c->SO() == SO()) continue;

          auto eventQueue = c->SO()->getComponent<CharacterEventQueue>();

          bs::gDebug().logDebug("[CharacterKeyboardInput] Talk to: " + c->SO()->getName());
          eventQueue->clear(); // FIXME: Find out what's blocking the new message
          eventQueue->pushTalkToCharacter(thisCharacter);
          break;
        }
    }
  }

  void CharacterKeyboardInput::fixedUpdate()
  {
    bool goingForward = bs::gVirtualInput().isButtonHeld(mMoveForward);
    bool goingBack    = bs::gVirtualInput().isButtonHeld(mMoveBack);
    bool goingLeft    = bs::gVirtualInput().isButtonHeld(mMoveLeft);
    bool goingRight   = bs::gVirtualInput().isButtonHeld(mMoveRight);
    bool fastMove     = bs::gVirtualInput().isButtonHeld(mFastMove);

    // Always keep the user controllers physics active
    mCharacterAI->activatePhysics();

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

    if (fastMove)
    {
      mCharacterAI->fastMove(4.0f);
    }
    else
    {
      mCharacterAI->fastMove(1.0f);
    }
  }

  REGOTH_DEFINE_RTTI(CharacterKeyboardInput)
}  // namespace REGoth
