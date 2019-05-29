#pragma once

#include <BsPrerequisites.h>
#include <BsZenLib/ZenResources.hpp>
#include <Image/BsColor.h>
#include <Math/BsVector2.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  /**
   * Skystates, in order of appearance
   */
  enum class SkyPresetType : bs::UINT32
  {
    Day1 = 0,
    Day2,
    Evening,
    Night0,
    Night1,
    Night2,
    Dawn,
    Day0,
    NUM_PRESETS
  };

  class SkyConfig;

  /**
   * This class will calculate the ambient light color emitted by the sky.
   *
   * As in the real world, the color of the ambient light changes depending on the
   * daytime in the original game. To archive this effect, the game uses a list of
   * so called `SkyState`s, which describe the look of the sky at a single fixed point
   * in time. During the day/night cycle, the game then interpolates between two of
   * those `SkyState`s, the one closest *before* the current time, and the closest
   * next one. The interpolated color is then applied as ambient lighting and fog
   * color.
   *
   * Fog will also be calculated by this class, since it's parameters are embedded
   * inside the `SkyState`s as well.
   *
   * Since sky rendering is a pretty complex topic in the original game, this class
   * contains a lot of magic numbers of which we are not quite sure why many of them
   * were chosen. Probably only because it looked good.
   */
  class SkyColoring
  {
  public:
    struct SkyLayer
    {
      bs::String textureNameBase;

      float textureAlpha;
      float textureScale;
      bs::Vector2 textureSpeed;
    };

    /**
     * One skystate which can be interpolated into
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

      // Base-fog distance modifier
      float fogDistance;

      // Whether the sun should be active. If false, the moon is.
      bool isSunActive;

      // Information about the cloud and sky layers
      SkyLayer cloudsLayer;
      SkyLayer skyLayer;
    };

    SkyColoring();
    virtual ~SkyColoring();

    /**
     * Updates the sky and the colors.
     *
     * @param  dayRatio  Value in range 0..1 where 0 means start of day and 1 and of day.
     *                   For example, if the clock reads 0:00, this would need to be 0.
     *                   If the clock reads 23:59 this would ne to be close to 1.
     */
    void interpolate(float dayRatio);

    /**
     * Fills the sky states with their preset values
     */
    void fillSkyStates();

    /**
     * To be called when the name of the current level changed, so the sky can adapt to
     * custom-textures the world may has.
     *
     * @param  newWorldName Name of the loaded Zen-File, e.g. `WORLD` for `WORLD.ZEN`
     */
    void onWorldNameChanged(const bs::String& newWorldName);

    /**
     * Outputs the two colors to lerp intensity values with to turn it into a sky-colored pixel
     */
    void getSkyColors(bs::Color& color0, bs::Color& color1);

    /**
     * Calculates the near- and farplanes for the fog
     *
     * @param[out]  distanceNear  Where the fog should start.
     * @param[out]  distanceEnd   Where the fog should end.
     * @param[out]  color         How the fog should be colored.
     */
    void calculateFogDistanceAndColor(float& nearFog, float& farFog, bs::Color& finalFogColor) const;

    /**
     * @return Whether it's currently nighttime
     */
    bool isNightTime() const;

    /**
     * @return Color of the coulds layer as poly (layer 1)
     */
    bs::Color getPolyCloudsLayerColor();

  private:
    /**
     * Finds the two sky states we need to interpolate between for the current time.
     * For example, if the current time is 0.28, this will find the sky states for
     * time 0.25 and the one for 0.30.
     *
     * The current time is read from the master state.
     *
     * @param[out]  s0  Index of the state to interpolate from.
     * @param[out]  s1  Index of the state to interpolate to.
     */
    void findTwoSkyStatesToInterpolateBetween(bs::UINT32& s0, bs::UINT32& s1) const;

    /**
     * Interpolates the two given sky states and stores the result into the master state.
     */
    void interpolateAndStoreIntoMasterState(const SkyState& s0, const SkyState& s1);

    /**
     * Initializes the given skystate to the given type
     */
    static void initSkyState(SkyPresetType type, SkyState& s);

    /**
     * Skystates we're interpolating
     */
    std::array<SkyState, (bs::UINT32)SkyPresetType::NUM_PRESETS> mSkyStates;

    /**
     * Interpolated skystate
     */
    SkyState mMasterState;
  };
}  // namespace REGoth
