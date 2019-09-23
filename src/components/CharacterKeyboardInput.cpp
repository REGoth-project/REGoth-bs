#include "CharacterKeyboardInput.hpp"

#include <RTTI/RTTI_CharacterKeyboardInput.hpp>

#include <components/Character.hpp>
#include <components/CharacterAI.hpp>
#include <components/CharacterEventQueue.hpp>
#include <components/Focusable.hpp>
#include <components/GameWorld.hpp>
#include <components/Inventory.hpp>
#include <components/Item.hpp>

#include <exception/Throw.hpp>
#include <log/logging.hpp>

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

    mMoveForward       = bs::VirtualButton("MoveForward");
    mMoveBack          = bs::VirtualButton("MoveBack");
    mStrafeLeft        = bs::VirtualButton("StrafeLeft");
    mStrafeRight       = bs::VirtualButton("StrafeRight");
    mTurnLeft          = bs::VirtualButton("TurnLeft");
    mTurnRight         = bs::VirtualButton("TurnRight");
    mFastMove          = bs::VirtualButton("FastMove");
    mToggleWalking     = bs::VirtualButton("ToggleWalking");
    mToggleSneaking    = bs::VirtualButton("ToggleSneaking");
    mToggleMeleeWeapon = bs::VirtualButton("ToggleMeleeWeapon");
    mJump              = bs::VirtualButton("Jump");
    mAction            = bs::VirtualButton("Action");
    mQuickSave         = bs::VirtualButton("QuickSave");

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

      auto closestFocusable = mCharacter->findClosestFocusable();

      if (closestFocusable)
      {
        auto characterEventQueue = closestFocusable->SO()->getComponent<CharacterEventQueue>();
        auto item                = closestFocusable->SO()->getComponent<Item>();

        if (characterEventQueue)
        {
          REGOTH_LOG(Info, Uncategorized, "[CharacterKeyboardInput] Talk to: {0}",
                     characterEventQueue->SO()->getName());

          characterEventQueue->clear();  // FIXME: Find out what's blocking the new message
          characterEventQueue->pushTalkToCharacter(thisCharacter);
        }
        else if (item)
        {
          auto inventory = mCharacter->SO()->getComponent<Inventory>();

          REGOTH_LOG(Info, Uncategorized, "[CharacterKeyboardInput] Pick up item: {0}",
                     item->itemInstance());

          inventory->giveItem(item->itemInstance());

          mWorld->removeItem(item);
        }
        else
        {
          REGOTH_LOG(Info, Uncategorized,
                     "[CharacterKeyboardInput] Action on unknown focus object: {0}",
                     closestFocusable->SO()->getName());
        }
      }
    }

    if (bs::gVirtualInput().isButtonDown(mToggleWalking))
    {
      mCharacterAI->tryToggleWalking();
    }

    if (bs::gVirtualInput().isButtonDown(mToggleSneaking))
    {
      mCharacterAI->tryToggleSneaking();
    }

    if (bs::gVirtualInput().isButtonDown(mToggleMeleeWeapon))
    {
      mCharacterAI->tryToggleMeleeWeapon();
    }

    if (bs::gVirtualInput().isButtonDown(mJump))
    {
      mCharacterAI->jump();
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
    else if (bs::gVirtualInput().isButtonHeld(mStrafeLeft))
    {
      // FIXME: Should be strafeLeft, but has to be strafeRight here since the world is mirrored
      mCharacterAI->strafeRight();
    }
    else if (bs::gVirtualInput().isButtonHeld(mStrafeRight))
    {
      // FIXME: Should be strafeRight, but has to be strafeLeft here since the world is mirrored
      mCharacterAI->strafeLeft();
    }
    else
    {
      mCharacterAI->stopMoving();
    }

    if (bs::gVirtualInput().isButtonHeld(mTurnLeft))
    {
      // FIXME: Should be turnLeft, but has to be turnRight here since the world is mirrored
      mCharacterAI->turnRight();
    }
    else if (bs::gVirtualInput().isButtonHeld(mTurnRight))
    {
      // FIXME: Should be turnRight, but has to be turnLeft here since the world is mirrored
      mCharacterAI->turnLeft();
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
