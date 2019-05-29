#include "ThirdPersonCamera.hpp"
#include <RTTI/RTTI_ThirdPersonCamera.hpp>
#include <components/Character.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  ThirdPersonCamera::ThirdPersonCamera(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("ThirdPersonCamera");
  }

  ThirdPersonCamera::~ThirdPersonCamera()
  {
  }

  void ThirdPersonCamera::follow(HCharacter character)
  {
    mFollowedCharacter = character;

    mOldLookAt = characterPosition();
    SO()->setPosition(characterPosition());
  }

  void ThirdPersonCamera::modifyDistance(float delta)
  {
    mDistanceToAimFor += delta;

    // FIXME: Make min and max distance configurable
    mDistanceToAimFor = bs::Math::clamp(mDistanceToAimFor, 1.4f, 6.0f);
  }

  float ThirdPersonCamera::distanceToCharacter() const
  {
    throwIfFollowedCharacterIsNotValid();

    const auto& posCamera = SO()->getTransform().pos();
    const auto& posTarget = mFollowedCharacter->SO()->getTransform().pos();

    return (posTarget - posCamera).length();
  }

  bs::Vector3 ThirdPersonCamera::characterPosition() const
  {
    throwIfFollowedCharacterIsNotValid();

    return mFollowedCharacter->SO()->getTransform().pos();
  }

  bs::Vector3 ThirdPersonCamera::characterForwardDirection() const
  {
    throwIfFollowedCharacterIsNotValid();

    return mFollowedCharacter->SO()->getTransform().getForward();
  }

  void ThirdPersonCamera::fixedUpdate()
  {
    bs::Component::fixedUpdate();

    gracefullyMoveCamera(bs::gTime().getFixedFrameDelta());
  }

  void ThirdPersonCamera::gracefullyMoveCamera(float dt)
  {
    // Maybe we want to be a bit more forgiving in the future...
    throwIfFollowedCharacterIsNotValid();

    bs::Vector3 newPosition = characterPosition();
    newPosition -= characterForwardDirection() * mDistanceToAimFor;
    newPosition += bs::Vector3(0, 1.6f, 0);

    bs::Vector3 newLookAt = characterPosition();
    newLookAt += bs::Vector3(0, 1.4f, 0);

    bs::Vector3 oldPosition = SO()->getTransform().pos();

    newPosition = bs::Vector3::lerp(0.05f, oldPosition, newPosition);
    newLookAt   = bs::Vector3::lerp(0.10f, mOldLookAt, newLookAt);

    SO()->setPosition(newPosition);
    SO()->lookAt(newLookAt);

    mOldLookAt = newLookAt;
  }

  void ThirdPersonCamera::throwIfFollowedCharacterIsNotValid() const
  {
    if (mFollowedCharacter.isDestroyed())
    {
      REGOTH_THROW(InvalidStateException,
                   "Character followed by ThirdPersonCamera was expected to be valid!");
    }
  }

  REGOTH_DEFINE_RTTI(ThirdPersonCamera)
}  // namespace REGoth
