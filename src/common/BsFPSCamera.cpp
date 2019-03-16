#include "BsFPSCamera.h"
#include "Math/BsMath.h"
#include "Math/BsVector3.h"
#include "Physics/BsPhysics.h"
#include "Scene/BsSceneObject.h"
#include "Utility/BsTime.h"

namespace bs
{
  /** Determines speed of camera rotation. */
  constexpr float ROTATION_SPEED = 3.0f;

  /** Determines range of movement for pitch rotation, in either direction. */
  constexpr Degree PITCH_RANGE = Degree(45.0f);

  /** Initial movement speed. */
  constexpr float START_SPEED = 10.0f;  // m/s

  /** Maximum movement speed. */
  constexpr float TOP_SPEED = 16.0f;  // m/s

  /** Acceleration that determines how quickly to go from starting to top speed. */
  constexpr float ACCELERATION = 1.5f;

  /** Multiplier applied to the speed when the fast move button is held. */
  constexpr float FAST_MODE_MULTIPLIER = 2.0f;

  FPSCamera::FPSCamera(const HSceneObject& parent)
      : Component(parent)
  {
    // Set a name for the component, so we can find it later if needed
    setName("FPSCamera");

    // Get handles for key bindings. Actual keys attached to these bindings will be registered during
    // app start-up.
    mHorizontalAxis = VirtualAxis("Horizontal");
    mVerticalAxis = VirtualAxis("Vertical");
	mRotate = VirtualButton("Rotate");

    // Get handles for key bindings. Actual keys attached to these bindings will be
    // registered during app start-up.
    mMoveForward = VirtualButton("Forward");
    mMoveBack = VirtualButton("Back");
    mMoveLeft = VirtualButton("Left");
    mMoveRight = VirtualButton("Right");
    mFastMove = VirtualButton("FastMove");

    // Determine initial yaw and pitch
    Quaternion rotation = SO()->getTransform().getRotation();

    Radian pitch, yaw, roll;
    (void)rotation.toEulerAngles(pitch, yaw, roll);

    mPitch = pitch;
    mYaw = yaw;

    applyAngles();
  }

  void FPSCamera::fixedUpdate()
  {
    // Check if any movement keys are being held
    bool goingForward = gVirtualInput().isButtonHeld(mMoveForward);
    bool goingBack = gVirtualInput().isButtonHeld(mMoveBack);
    bool goingLeft = gVirtualInput().isButtonHeld(mMoveLeft);
    bool goingRight = gVirtualInput().isButtonHeld(mMoveRight);
    bool fastMove = gVirtualInput().isButtonHeld(mFastMove);

    const Transform& tfrm = SO()->getTransform();

    // If the movement button is pressed, determine direction to move in
    Vector3 direction = Vector3::ZERO;
    if (goingForward) direction += tfrm.getForward();
    if (goingBack) direction -= tfrm.getForward();
    if (goingRight) direction += tfrm.getRight();
    if (goingLeft) direction -= tfrm.getRight();

    const float frameDelta = gTime().getFixedFrameDelta();

    // If a direction is chosen, normalize it to determine final direction.
    if (direction.squaredLength() != 0)
    {
      direction.normalize();

      // Apply fast move multiplier if the fast move button is held.
      float multiplier = 1.0f;
      if (fastMove) multiplier = FAST_MODE_MULTIPLIER;

      // Calculate current speed of the camera
      mCurrentSpeed = Math::clamp(mCurrentSpeed + ACCELERATION * frameDelta, START_SPEED, TOP_SPEED);
      mCurrentSpeed *= multiplier;
    }
    else
    {
      mCurrentSpeed = 0.0f;
    }

    // If the current speed isn't too small, move the camera in the wanted direction
    Vector3 velocity(BsZero);
    float tooSmall = std::numeric_limits<float>::epsilon();
    if (mCurrentSpeed > tooSmall) velocity = direction * mCurrentSpeed;

    SO()->move(velocity * frameDelta);
  }

  void FPSCamera::update()
  {
	  bool shouldRotate = gVirtualInput().isButtonHeld(mRotate);

	  if (shouldRotate)
	  {
		  // If camera is rotating, apply new pitch/yaw rotation values depending on the amount of rotation
		  // from the vertical/horizontal axes.
		  mYaw += Degree(gVirtualInput().getAxisValue(mHorizontalAxis) * ROTATION_SPEED);
		  mPitch += Degree(gVirtualInput().getAxisValue(mVerticalAxis) * ROTATION_SPEED);

		  applyAngles();
	  }
  }

  void FPSCamera::applyAngles()
  {
    mYaw.wrap();
    mPitch.wrap();

    const Degree pitchMax = PITCH_RANGE;
    const Degree pitchMin = Degree(360.0f) - PITCH_RANGE;

    if (mPitch > pitchMax && mPitch < pitchMin)
    {
      if ((mPitch - pitchMax) > (pitchMin - mPitch))
        mPitch = pitchMin;
      else
        mPitch = pitchMax;
    }

    Quaternion yRot(Vector3::UNIT_Y, Radian(mYaw));
    Quaternion xRot(Vector3::UNIT_X, Radian(mPitch));

    if (!mCharacterSO)
    {
      Quaternion camRot = yRot * xRot;
      camRot.normalize();

      SO()->setRotation(camRot);
    }
    else
    {
      mCharacterSO->setRotation(yRot);
      SO()->setRotation(xRot);
    }
  }

}  // namespace bs
