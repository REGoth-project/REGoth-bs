#pragma once

#include "BsPrerequisites.h"
#include "Scene/BsComponent.h"
#include "Math/BsMath.h"
#include "Input/BsVirtualInput.h"

namespace bs
{
	/** Component that controls rotation of its scene object. */
	class ObjectRotator : public Component
	{
	public:
		ObjectRotator(const HSceneObject& parent);

		/** Triggered once per frame. Allows the component to handle input and move. */
		void update() override;

	private:
		Degree mPitch; /**< Current pitch rotation of the object (up or down). */
		Degree mYaw; /**< Current yar rotation of the object (left or right). */
		bool mLastButtonState; /**< Determines was the user rotating the object last frame. */

		VirtualButton mRotateObj; /**< Key that allows object to be rotated while held. */
		VirtualAxis mVerticalAxis; /**< Input device axis used for controlling object's pitch rotation (up/down). */
		VirtualAxis mHorizontalAxis; /**< Input device axis used for controlling object's yaw rotation (left/right). */

		static const float ROTATION_SPEED; /**< Determines speed for rotation, in degrees per second. */
	};
}