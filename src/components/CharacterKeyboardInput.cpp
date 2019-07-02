#include "CharacterKeyboardInput.hpp"
#include <RTTI/RTTI_CharacterKeyboardInput.hpp>
#include <components/Character.hpp>
#include <components/CharacterAI.hpp>
#include <components/CharacterEventQueue.hpp>
#include <components/GameWorld.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  CharacterKeyboardInput::CharacterKeyboardInput(const bs::HSceneObject& parent, HGameWorld world)
      : bs::Component(parent)
      , mWorld(world)
  {
    setName("CharacterKeyboardInput");
  }

  CharacterKeyboardInput::~CharacterKeyboardInput()
  {
  }

  void CharacterKeyboardInput::onInitialized()
  {
    bs::Component::onInitialized();

    mMoveForward = bs::VirtualButton("MoveForward");
    mMoveBack    = bs::VirtualButton("MoveBack");
    mStrafeLeft  = bs::VirtualButton("StrafeLeft");
    mStrafeRight = bs::VirtualButton("StrafeRight");
    mTurnLeft    = bs::VirtualButton("TurnLeft");
    mTurnRight   = bs::VirtualButton("TurnRight");
    mFastMove    = bs::VirtualButton("FastMove");
    mAction      = bs::VirtualButton("Action");
    mQuickSave   = bs::VirtualButton("QuickSave");

    mCharacter = SO()->getComponent<Character>();

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
    if (bs::gVirtualInput().isButtonDown(mAction))
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
        eventQueue->clear();  // FIXME: Find out what's blocking the new message
        eventQueue->pushTalkToCharacter(thisCharacter);
        break;
      }
    }

    if (bs::gVirtualInput().isButtonDown(mQuickSave))
    {
      mWorld->save("WorldViewer-" + mWorld->worldName() + ".ZEN");
    }
  }

  void CharacterKeyboardInput::fixedUpdate()
  {
    // Always keep the user controllers physics active
    mCharacterAI->activatePhysics();

    if (bs::gVirtualInput().isButtonHeld(mMoveForward))
    {
      mCharacterAI->goForward();
    }
    else if (bs::gVirtualInput().isButtonHeld(mMoveBack))
    {
      mCharacterAI->goBackward();
    }
    else
    {
      mCharacterAI->stopMoving();
    }

    if (bs::gVirtualInput().isButtonHeld(mTurnLeft))
    {
      mCharacterAI->turnLeft();
    }
    else if (bs::gVirtualInput().isButtonHeld(mTurnRight))
    {
      mCharacterAI->turnRight();
    }
    else
    {
      mCharacterAI->stopTurning();
    }

    if (bs::gVirtualInput().isButtonHeld(mFastMove))
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
