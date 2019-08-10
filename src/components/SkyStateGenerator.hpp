#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include <BsPrerequisites.h>
#include <Image/BsColor.h>
#include <Math/BsVector2.h>

#include <BsZenLib/ZenResources.hpp>

#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  /**
   * Describes a certain layer in the sky.
   */
  struct SkyLayer
  {
    bs::String textureName;

    bs::HTexture texture;
    float textureAlpha;
    float textureScale;
    bs::Vector2 textureSpeed;
  };

  /**
   * Describes a given sky state.
   */
  struct SkyState
  {
    // Time when this state becomes active
    // time in days since last 12:00
    float time;

    // Color values
    bs::Color baseColor;
    bs::Color fogColor;
    bs::Color domeColorUpper;

    float fogNear;
    float fogFar;

    // Base-fog distance modifier
    float fogDistance;

    // Whether the sun should be active. If false, the moon is.
    bool isSunActive;

    // Information about the cloud and sky layers
    SkyLayer cloudsLayer;
    SkyLayer skyLayer;
  };

  class SkyConfig;

  /**
   * This class will calculate the ambient light color emitted by the sky.
   *
   * As in the real world, the color of the ambient light changes depending on the daytime in the
   * original game.  To archive this effect, the game uses a list of so called `SkyState`s, which
   * describe the look of the sky at a single fixed point in time.  During the day/night cycle, the
   * game then interpolates between two of those `SkyState`s, the one closest *before* the current
   * time, and the closest next one.  The interpolated color is then applied as ambient lighting and
   * fog color.
   *
   * Fog will also be calculated by this class, since it's parameters are embedded inside the
   * `SkyState`s as well.
   *
   * Since sky rendering is a pretty complex topic in the original game, this class contains a lot of
   * magic numbers of which we are not quite sure why many of them were chosen.  Probably only
   * because it looked good.
   */
  class SkyStateGenerator
  {
  public:
    SkyStateGenerator(const bs::Color& skyColor, const bs::String& worldName);
    virtual ~SkyStateGenerator();

    /**
     * Updates the `SkyState`.
     *
     * @param  dayRatio  Value in range [0, 1) where 0 means start of day and (nearly) 1 end of day.
     *                   For example, if the clock reads 0:00, this would need to be 0.  If the clock
     *                   reads 23:59 this would need to be close to 1.
     * @return The current sky state after update.
     */
    const std::shared_ptr<const SkyState> update(float dayRatio);

  private:
    /**
     * @return Color of the coulds layer as poly (layer 1).
     */
    bs::Color getPolyCloudsLayerColor();

    /**
     * Finds the two sky states we need to interpolate between for the current time.
     *
     * For example, if the current time is 0.28, this will find the sky states for time 0.25 and the
     * one for 0.30.  The current time is read from `mCurrentSkyState`.
     *
     * @return 2-Tuple of indices of the presets to interpolate
     */
    std::tuple<unsigned int, unsigned int> findPresetIndices() const;

    /**
     * Interpolates the two given sky states and stores the result into `mCurrentSkyState`.
     */
    void interpolatePresets(const SkyState& p1, const SkyState& p2);

    /**
     * Collection of `SkyState` presets to interpolate between.
     */
    const std::vector<SkyState> mSkyStatePresets;

    /**
     * `SkyState` reflecting the current status of the sky.
     */
    std::shared_ptr<SkyState> mCurrentSkyState;
  };
}  // namespace REGoth
