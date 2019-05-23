#include "CharacterAI.hpp"
#include <Components/BsCCamera.h>
#include <Components/BsCCharacterController.h>
#include <RTTI/RTTI_CharacterAI.hpp>
#include <Scene/BsSceneManager.h>
#include <Scene/BsSceneObject.h>
#include <animation/StateNaming.hpp>
#include <components/GameWorld.hpp>
#include <components/VisualCharacter.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  /** How fast the character can turn when not holding a weapon. (Radians/Second) */
  constexpr float TURN_SPEED_NORMAL = 0.05f;

  /** Multiplicator of how fast the character can turn while holding a weapon. */
  constexpr float TURN_SPEED_MULTIPLICATOR_WITH_WEAPON = 2.0f;

  /**
   * How far away the character can be from the camera until it should disable physics.
   * Must be larger than the range which activates physics again, ACTIVATE_PHYSICS_RANGE_METERS.
   * See https://regoth-project.github.io/REGoth-bs/content/characters.html
   *
   * TODO: Make this configurable.
   */
  constexpr float DEACTIVATE_PHYSICS_RANGE_METERS = 45.0;

  /** See DEACTIVATE_PHYSICS_RANGE_METERS */
  constexpr float ACTIVATE_PHYSICS_RANGE_METERS = 40.0;

  CharacterAI::CharacterAI(const bs::HSceneObject& parent, HGameWorld world)
      : bs::Component(parent)
      , mWorld(world)
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

  void CharacterAI::deactivatePhysics()
  {
    mIsPhysicsActive = false;
  }

  void CharacterAI::activatePhysics()
  {
    mIsPhysicsActive = true;
  }

  bool CharacterAI::shouldDisablePhysics() const
  {
    const auto& mainCamera     = bs::gSceneManager().getMainCamera();
    const auto& cameraPosition = mainCamera->getTransform().pos();
    const auto& soPosition     = SO()->getTransform().pos();

    float maxRangeSq = DEACTIVATE_PHYSICS_RANGE_METERS * DEACTIVATE_PHYSICS_RANGE_METERS;

    return cameraPosition.squaredDistance(soPosition) > maxRangeSq;
  }

  bool CharacterAI::shouldEnablePhysics() const
  {
    const auto& mainCamera     = bs::gSceneManager().getMainCamera();
    const auto& cameraPosition = mainCamera->getTransform().pos();
    const auto& soPosition     = SO()->getTransform().pos();

    float minRangeSq = ACTIVATE_PHYSICS_RANGE_METERS * ACTIVATE_PHYSICS_RANGE_METERS;

    return cameraPosition.squaredDistance(soPosition) < minRangeSq;
  }

  bool CharacterAI::isPhysicsActive() const
  {
    return mIsPhysicsActive;
  }

  void CharacterAI::handlePhysicsActivation()
  {
    if (mIsPhysicsActive)
    {
      if (shouldDisablePhysics())
      {
        deactivatePhysics();
      }
    }
    else
    {
      if (shouldEnablePhysics())
      {
        activatePhysics();
      }
    }
  }

  bool CharacterAI::goForward()
  {
    if (!isStateSwitchAllowed()) return false;

    bs::String anim = AnimationState::constructStateAnimationName(AI::WeaponMode::None,
                                                                  mWalkMode, "L");

    return mVisual->tryPlayTransitionAnimationTo(anim);
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

    bs::String anim =
        AnimationState::constructStateAnimationName(AI::WeaponMode::None, mWalkMode, "");

    return mVisual->tryPlayTransitionAnimationTo(anim);
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

    bs::String state = AnimationState::getStateName(playingAnim);

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

  void CharacterAI::instantTurnToPosition(const bs::Vector3& position)
  {
    bs::Vector3 positionSameHeight = position;

    // Characters should stay upright (at least most of them),
    // thus modify the position as if it were straight ahead.
    positionSameHeight.y = SO()->getTransform().pos().y;

    SO()->lookAt(positionSameHeight);
  }

  bool CharacterAI::gotoPositionStraight(const bs::Vector3& position)
  {
    instantTurnToPosition(position);

    goForward();

    return isAtPosition(position);
  }

  bool CharacterAI::isAtPosition(const bs::Vector3& position)
  {
    return (SO()->getTransform().pos() - position).length() < 0.5f;
  }

  void CharacterAI::fastMove(float factor)
  {
    mVisual->setDebugAnimationSpeedFactor(factor);
  }

  void CharacterAI::fixedUpdate()
  {
    handlePhysicsActivation();

    if (!mIsPhysicsActive)
    {
      return;
    }

    if (isTurningAllowed())
    {
      handleTurning();
    }

    bs::Vector3 rootMotion = mVisual->resolveFrameRootMotion();

    bs::Vector3 rootMotionRotated = SO()->getTransform().getRotation().rotate(rootMotion);

    // For some reason this is inverted
    rootMotionRotated *= -1.0;

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
    bs::HSceneObject so = mWorld->findObjectByName(waypoint);

    if (!so)
    {
      // Usually we would throw here, but Gothic has some invalid waypoints inside it's scripts
      // so we would break the original games if we did that. Resort to a warning for those,
      // better than nothing, I guess.
      bs::gDebug().logWarning("[CharacterAI] Teleport failed, waypoint doesn't exist: " + waypoint);
      return;
    }

    SO()->setPosition(so->getTransform().pos());

    // Turn the same way the waypoint is oriented, but have the character keep looking forward
    bs::Vector3 forwardCenterd = so->getTransform().getForward();

    forwardCenterd.y = 0;
    forwardCenterd.normalize();

    SO()->setForward(forwardCenterd);
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

  void CharacterAI::setWalkMode(AI::WalkMode walkMode)
  {
    mWalkMode = walkMode;
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

  REGOTH_DEFINE_RTTI(CharacterAI);
}  // namespace REGoth
