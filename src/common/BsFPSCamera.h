#pragma once

#include "BsPrerequisites.h"
#include "Input/BsVirtualInput.h"
#include "Math/BsDegree.h"
#include "Scene/BsComponent.h"

namespace bs
{
  /**
   * Component that controls rotation of the scene objects it's attached to through mouse input. Used
   * for first person views.
   */
  class FPSCamera : public Component
  {
  public:
    FPSCamera(const HSceneObject& parent);

    /**
     * Sets the character scene object to manipulate during rotations. When set, all yaw rotations
     * will be applied to the provided scene object, otherwise they will be applied to the current
     * object.
     */
    void setCharacter(const HSceneObject& characterSO) { mCharacterSO = characterSO; }

    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;

    /** Triggered once per frame. Allows the component to handle input and move. */
    void fixedUpdate() override;

  private:
    /** Applies the current yaw and pitch angles, rotating the object. Also wraps and clamps the
     * angles as necessary. */
    void applyAngles();

    HSceneObject mCharacterSO; /**< Optional parent object to manipulate. */

    Degree mPitch = Degree(0.0f); /**< Current pitch rotation of the camera (looking up or down). */
    Degree mYaw = Degree(0.0f);   /**< Current yaw rotation of the camera (looking left or right). */

    float mCurrentSpeed = 0.0f; /**< Current speed of the camera. */

	VirtualButton mRotate;       /**< Key that allows camera to be rotated while held. */
    VirtualAxis mVerticalAxis;   /**< Input device axis used for controlling camera's pitch rotation
                                    (up/down). */
    VirtualAxis mHorizontalAxis; /**< Input device axis used for controlling camera's yaw rotation
                                    (left/right). */
    VirtualButton mMoveForward;  /**< Key binding for moving the camera forward. */
    VirtualButton mMoveBack;     /**< Key binding for moving the camera backward. */
    VirtualButton mMoveLeft;     /**< Key binding for moving the camera left. */
    VirtualButton mMoveRight;    /**< Key binding for moving the camera right. */
    VirtualButton mFastMove;     /**< Key that speeds up movement while held. */
  };

  using HFPSCamera = GameObjectHandle<FPSCamera>;
}  // namespace bs
