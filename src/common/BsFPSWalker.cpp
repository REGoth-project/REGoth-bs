#include "BsFPSWalker.h"
#include "Math/BsVector3.h"
#include "Math/BsMath.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCCamera.h"
#include "Components/BsCCharacterController.h"
#include "BsApplication.h"
#include "Physics/BsPhysics.h"
#include "Utility/BsTime.h"

namespace bs
{
	/** Initial movement speed. */
	constexpr float START_SPEED = 4.0f; // m/s

	/** Maximum movement speed. */
	constexpr float TOP_SPEED = 7.0f; // m/s

	/** Acceleration that determines how quickly to go from starting to top speed. */
	constexpr float ACCELERATION = 1.5f;

	/** Multiplier applied to the speed when the fast move button is held. */
	constexpr float FAST_MODE_MULTIPLIER = 2.0f;

	FPSWalker::FPSWalker(const HSceneObject& parent)
		:Component(parent)
	{
		// Set a name for the component, so we can find it later if needed
		setName("FPSWalker");

		// Find the CharacterController we'll be using for movement
		mController = SO()->getComponent<CCharacterController>();

		// Get handles for key bindings. Actual keys attached to these bindings will be registered during app start-up.
		mMoveForward = VirtualButton("Forward");
		mMoveBack = VirtualButton("Back");
		mMoveLeft = VirtualButton("Left");
		mMoveRight = VirtualButton("Right");
		mFastMove = VirtualButton("FastMove");
	}

	void FPSWalker::fixedUpdate()
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

		// Eliminate vertical movement
		direction.y = 0.0f;
		direction.normalize();

		const float frameDelta = gTime().getFixedFrameDelta();

		// If a direction is chosen, normalize it to determine final direction.
		if (direction.squaredLength() != 0)
		{
			direction.normalize();

			// Apply fast move multiplier if the fast move button is held.
			float multiplier = 1.0f;
			if (fastMove)
				multiplier = FAST_MODE_MULTIPLIER;

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
		if (mCurrentSpeed > tooSmall)
			velocity = direction * mCurrentSpeed;

		// Note: Gravity is acceleration, but since the walker doesn't support falling, just apply it as a velocity
		Vector3 gravity = gPhysics().getGravity();
		mController->move((velocity + gravity) * frameDelta);
	}
}
