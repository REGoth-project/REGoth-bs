#include "CharacterAI.hpp"
#include <Components/BsCCharacterController.h>
#include <RTTI/RTTI_CharacterAI.hpp>
#include <Scene/BsSceneObject.h>
#include <animation/StateNaming.hpp>
#include <components/VisualCharacter.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  /** How fast the character can turn when not holding a weapon. (Radians/Second) */
  constexpr float TURN_SPEED_NORMAL = 0.05f;

  /** Multiplicator of how fast the character can turn while holding a weapon. */
  constexpr float TURN_SPEED_MULTIPLICATOR_WITH_WEAPON = 2.0f;

  CharacterAI::CharacterAI(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    mVisual = SO()->getComponent<VisualCharacter>();

    if (mVisual.isDestroyed())
    {
      REGOTH_THROW(
          InvalidStateException,
          bs::StringUtil::format("Scene Object {0} does not have a VisualCharacter component!",
                                 SO()->getName()));
    }

    mCharacterController = SO()->getComponent<bs::CCharacterController>();

    if (mCharacterController.isDestroyed())
    {
      REGOTH_THROW(
          InvalidStateException,
          bs::StringUtil::format("Scene Object {0} does not have a CCharacterController component!",
                                 SO()->getName()));
    }
  }

  bool CharacterAI::goForward()
  {
    if (!isStateSwitchAllowed()) return false;

    return mVisual->tryPlayTransitionAnimationTo("S_RUNL");
  }

  bool CharacterAI::goBackward()
  {
    if (!isStateSwitchAllowed()) return false;

    return mVisual->tryPlayTransitionAnimationTo("T_JUMPB");
  }

  bool CharacterAI::strafeLeft()
  {
    if (!isStateSwitchAllowed()) return false;

    // TODO: Implement

    return true;
  }

  bool CharacterAI::strafeRight()
  {
    if (!isStateSwitchAllowed()) return false;

    // TODO: Implement

    return true;
  }

  bool CharacterAI::turnLeft()
  {
    if (!isTurningAllowed()) return false;

    mTurnDirection = TurnDirection::Left;

    return true;
  }

  bool CharacterAI::turnRight()
  {
    if (!isTurningAllowed()) return false;

    mTurnDirection = TurnDirection::Right;

    return true;
  }

  bool CharacterAI::stopMoving()
  {
    if (!isStateSwitchAllowed()) return false;

    return mVisual->tryPlayTransitionAnimationTo("S_RUN");
  }

  bool CharacterAI::stopTurning()
  {
    mTurnDirection = TurnDirection::None;

    return true;
  }

  bool CharacterAI::isStateSwitchAllowed()
  {
    mVisual                = SO()->getComponent<VisualCharacter>();
    bs::String playingAnim = mVisual->getPlayingAnimationName();

    if (playingAnim.empty()) return true;

    bs::String state = Animation::getStateName(playingAnim);

    // Playing some weird animation we don't know the naming scheme for?
    if (state.empty()) return false;

    if (!mVisual->isPlayingAnimationInterruptable()) return false;

    return true;
  }

  bool CharacterAI::isTurningAllowed()
  {
    // TODO: Find out when turning is allowed and implement this
    return true;
  }

  void CharacterAI::fixedUpdate()
  {
    if (isTurningAllowed())
    {
      handleTurning();
    }

    bs::Vector3 rootMotion = mVisual->resolveFrameRootMotion();

    // FIXME: Find out why the root motion is rotated 90 degrees?
    bs::Vector3 rootMotionFixed(-rootMotion.z, rootMotion.y, rootMotion.x);

    bs::Vector3 rootMotionRotated = SO()->getTransform().getRotation().rotate(rootMotionFixed);

    // No need to multiply rootMotion by the frame delta since it is the actual movement since
    // last time we queried it.
    mCharacterController->move(rootMotionRotated);

    // Note: Gravity is acceleration, but since the walker doesn't support falling, just apply it as
    // a velocity
    // FIXME: Actual gravity!
    const float frameDelta = bs::gTime().getFixedFrameDelta();
    bs::Vector3 gravity    = bs::Vector3(0, -9.81, 0);  // gPhysics().getGravity();

    mCharacterController->move(gravity * frameDelta);
  }

  void CharacterAI::handleTurning()
  {
    float frameTurn = 0.0f;

    switch (mTurnDirection)
    {
      case TurnDirection::None:
        frameTurn = 0.0f;
        break;

      case TurnDirection::Left:
        frameTurn = TURN_SPEED_NORMAL;
        break;

      case TurnDirection::Right:
        frameTurn = -TURN_SPEED_NORMAL;
        break;
    }

    SO()->rotate(bs::Vector3::UNIT_Y, bs::Radian(frameTurn));
  }

  void CharacterAI::teleport(const bs::String& waypoint)
  {
  }

  void CharacterAI::turnToNPC(bs::HSceneObject targetSO)
  {
  }

  void CharacterAI::standUp()
  {
  }

  void CharacterAI::standUpQuick()
  {
  }

  void CharacterAI::gotoWaypoint(const bs::String& waypoint)
  {
  }

  void CharacterAI::gotoFreePoint(const bs::String& freepoint)
  {
  }

  void CharacterAI::gotoNextFreePoint(const bs::String& freepoint)
  {
  }

  void CharacterAI::gotoNpc(bs::HSceneObject targetCharacterSO)
  {
  }

  void CharacterAI::startState(const bs::String& stateFunction, bs::INT32 stateBehavior,
                               const bs::String& waypoint)
  {
  }

  void CharacterAI::wait(float seconds)
  {
  }

  void CharacterAI::playAni(const bs::String& animation)
  {
  }

  void CharacterAI::setWalkMode(bs::UINT32 walkMode)
  {
  }

  void CharacterAI::stopProcessingInfos()
  {
  }

  void CharacterAI::output(bs::HSceneObject characterSO, const bs::String& svmName)
  {
  }

  void CharacterAI::processInfos()
  {
  }

  bs::RTTITypeBase* CharacterAI::getRTTIStatic()
  {
    return RTTI_CharacterAI::instance();
  }

  bs::RTTITypeBase* CharacterAI::getRTTI() const
  {
    return CharacterAI::getRTTIStatic();
  }
}  // namespace REGoth
