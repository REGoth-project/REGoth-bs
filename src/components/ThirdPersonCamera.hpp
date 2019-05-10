#pragma once
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  class ThirdPersonCamera;
  using HThirdPersonCamera = bs::GameObjectHandle<ThirdPersonCamera>;

  /**
   * Third-Person Camera following a character. This is the camera used in normal gameplay.
   *
   * Cameras in Gothic seem to be a rather complex story. There are a number of camera
   * script-instances defined in CAMERA.DAT, for example:
   *
   *   INSTANCE CamModRun(CCamSys_Def)
   *   {
   *     bestRange 		 = 2.5 ;
   *     minRange  		 = 1.4 ;
   *     maxRange  		 = 6.0 ;
   *     bestElevation = 25.0 ;
   *     minElevation	 = 0.0;
   *     maxElevation	 = 89.0;
   *     bestAzimuth	 = 0.0;
   *     minAzimuth		 = -90.0;
   *     maxAzimuth		 = 90.0;
   *     rotOffsetX		 = 20.0;
   *     rotOffsetY		 = 0.0;
   *   };
   */
  class ThirdPersonCamera : public bs::Component
  {
  public:
    ThirdPersonCamera(const bs::HSceneObject& parent);
    virtual ~ThirdPersonCamera();

    /**
     * Lets the camera follow the given character.
     */
    void follow(HCharacter character);

    /**
     * Applies the given delta to the cameras distance. This can be used to zoom in the
     * camera using the mousewheel, for example.
     *
     * There is no direct setter for this distance, as the distance has to adapt to
     * certain situations during gameplay.
     *
     * @param  delta  Positive values will move the camera further away from the target,
     *                Negative values will move it closer.
     */
    void modifyDistance(float delta);

  protected:
    void fixedUpdate() override;
    void throwIfFollowedCharacterIsNotValid() const;

    /**
     * @return Position of the target character
     */
    bs::Vector3 characterPosition() const;

    /**
     * @return Forward-facing vector of the target character.
     */
    bs::Vector3 characterForwardDirection() const;

    /**
     * @return The actual distance from the camera to the target right now in meters.
     */
    float distanceToCharacter() const;

    /**
     * To be called from fixedUpdate(), moves the camera to the location it is aiming for.
     */
    void gracefullyMoveCamera(float dt);

    /**
     * Distance the camera should hold from the target character.
     */
    float mDistanceToAimFor = 2.5f;

    HCharacter mFollowedCharacter;

    /**
     * Position looked at in the previous update cycle, for smoothing.
     */
    bs::Vector3 mOldLookAt;
  public:
    REGOTH_DECLARE_RTTI(ThirdPersonCamera)

  protected:
    ThirdPersonCamera() = default;  // RTTI only
  };
}  // namespace REGoth
