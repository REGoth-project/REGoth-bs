#pragma once

#include "BsPrerequisites.h"
#include "Scene/BsComponent.h"
#include "Input/BsVirtualInput.h"

namespace bs
{
	/** 
	 * Component that controls movement through a character controller, used for first-person movement. The 
	 * CharacterController component must be attached to the same SceneObject this component is on.
	 */
	class FPSWalker : public Component
	{
	public:
		FPSWalker(const HSceneObject& parent);

		/** Triggered once per frame. Allows the component to handle input and move. */
		void fixedUpdate() override;

	private:
		HCharacterController mController;

		float mCurrentSpeed = 0.0f; /**< Current speed of the camera. */

		VirtualButton mMoveForward; /**< Key binding for moving the camera forward. */
		VirtualButton mMoveBack; /**< Key binding for moving the camera backward. */
		VirtualButton mMoveLeft; /**< Key binding for moving the camera left. */
		VirtualButton mMoveRight; /**< Key binding for moving the camera right. */
		VirtualButton mFastMove; /**< Key that speeds up movement while held. */
	};

	using HFPSWalker = GameObjectHandle<FPSWalker>;
}