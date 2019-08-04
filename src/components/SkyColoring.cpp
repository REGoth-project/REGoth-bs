#include <components/SkyColoring.hpp>

#include <Image/BsColor.h>
#include <Math/BsMath.h>
#include <Renderer/BsCamera.h>
#include <Scene/BsSceneManager.h>

namespace REGoth
{
  // These must be ordered, starting at 0.00f
  constexpr float TIME_KEY_0 = 0.00f;
  constexpr float TIME_KEY_1 = 0.25f;
  constexpr float TIME_KEY_2 = 0.30f;
  constexpr float TIME_KEY_3 = 0.35f;
  constexpr float TIME_KEY_4 = 0.50f;
  constexpr float TIME_KEY_5 = 0.65f;
  constexpr float TIME_KEY_6 = 0.70f;
  constexpr float TIME_KEY_7 = 0.75f;

  static_assert(TIME_KEY_0 == 0.00f, "TIME_KEY_0 must be 0.0f!");

  SkyColoring::SkyColoring(const bs::Color& skyColor)
      : mSkyColor{skyColor}
  {
    // pass
  }

  SkyColoring::~SkyColoring()
  {
    // pass
  }

  void SkyColoring::getSkyColors(bs::Color& color0, bs::Color& color1)
  {
    color0 = bs::Color(0.0f, 0.0f, 0.0f, 0.0f);
    color1 = mMasterState.baseColor;
  }

  void SkyColoring::interpolate(float dayRatio)
  {
    mMasterState.time = std::fmod(dayRatio + 0.5f, 1.0f);

    bs::UINT32 si0;
    bs::UINT32 si1;

    findTwoSkyStatesToInterpolateBetween(si0, si1);

    SkyState& s0 = mSkyStates[si0];
    SkyState& s1 = mSkyStates[si1];

    interpolateAndStoreIntoMasterState(s0, s1);

    // FIXME: There is some stuff about levelchanges here. Like, "turn off rain when on dragonisland"

    // FIXME: Multiply fogColor with 0.8 when using dome!
  }

  void SkyColoring::findTwoSkyStatesToInterpolateBetween(bs::UINT32& s0, bs::UINT32& s1) const
  {
    // init with values for case: time >= TIME_KEY_7 (= 0.75f)
    s0 = static_cast<bs::UINT32>(mSkyStates.size()) - 1;
    s1 = 0;

    for (bs::UINT32 i = 0; i < static_cast<bs::UINT32>(mSkyStates.size()); i++)
    {
      // Since the sky states are ordered, our start state is the first one which is supposed
      // to start *before* the current time. The target state is then the one right after.
      // Since it's easier to search for the target state, which is the first state which has a
      // time larger than the current time, we search for that. The start state is the one
      // right before it.
      if (mMasterState.time < mSkyStates[i].time)
      {
        // Subtracting 1 will not cause a negative numbers here since the first state has a
        // time of 0.0, so the check should never pass for it. If everything is set up correctly
        // that is.
        s0 = (i - 1) % mSkyStates.size();
        s1 = i % mSkyStates.size();
        break;
      }
    }
  }

  void SkyColoring::interpolateAndStoreIntoMasterState(const SkyState& s0, const SkyState& s1)
  {
    // Handle case time >= TIME_KEY_7 (= 0.75f)
    float timeS1 = s1.time < s0.time ? s1.time + 1.0f : s1.time;

    // Scale up time difference to [0,1]
    float t = (mMasterState.time - s0.time) / (timeS1 - s0.time);

    // Interpolate values
    mMasterState.baseColor      = s0.baseColor + t * (s1.baseColor - s0.baseColor);
    mMasterState.fogColor       = s0.fogColor + t * (s1.fogColor - s0.fogColor);
    mMasterState.fogDistance    = s0.fogDistance + t * (s1.fogDistance - s0.fogDistance);
    mMasterState.domeColorUpper = s0.domeColorUpper + t * (s1.domeColorUpper - s0.domeColorUpper);

    mMasterState.cloudsLayer = s0.cloudsLayer;
    mMasterState.cloudsLayer.textureAlpha =
        bs::Math::lerp(t, s0.cloudsLayer.textureAlpha, s1.cloudsLayer.textureAlpha);
    mMasterState.cloudsLayer.textureScale =
        bs::Math::lerp(t, s0.cloudsLayer.textureScale, s1.cloudsLayer.textureScale);

    mMasterState.skyLayer = s0.skyLayer;
    mMasterState.skyLayer.textureAlpha =
        bs::Math::lerp(t, s0.skyLayer.textureAlpha, s1.skyLayer.textureAlpha);
    mMasterState.skyLayer.textureScale =
        bs::Math::lerp(t, s0.skyLayer.textureScale, s1.skyLayer.textureScale);
  }

  void SkyColoring::initSkyState(SkyPresetType type, SkyColoring::SkyState& s)
  {
    switch (type)
    {
      case SkyPresetType::Day0:
        s.time = TIME_KEY_7;

        s.baseColor      = bs::Color(255, 250, 235) / 255.0f;
        s.fogColor       = mSkyColor;
        s.domeColorUpper = bs::Color(255, 255, 255) / 255.0f;

        s.fogDistance = 0.2f;
        s.isSunActive = true;

        s.cloudsLayer.textureNameBase = "SKYDAY_LAYER1_A0.TGA";
        s.cloudsLayer.textureAlpha    = 0.0f;
        s.cloudsLayer.textureSpeed    = bs::Vector2(1.1f, 0);
        s.cloudsLayer.textureScale    = 1.0f;

        s.skyLayer.textureNameBase = "SKYDAY_LAYER0_A0.TGA";
        s.skyLayer.textureAlpha    = 1.0f;
        s.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
        s.skyLayer.textureScale    = 1.0f;
        break;

      case SkyPresetType::Day1:
        s.time = TIME_KEY_0;

        s.baseColor      = bs::Color(255, 250, 235) / 255.0f;
        s.fogColor       = mSkyColor;
        s.domeColorUpper = bs::Color(255, 255, 255) / 255.0f;

        s.fogDistance = 0.05f;
        s.isSunActive = true;

        s.cloudsLayer.textureNameBase = "SKYDAY_LAYER1_A0.TGA";
        s.cloudsLayer.textureAlpha    = 215.0f / 255.0f;
        s.cloudsLayer.textureSpeed    = bs::Vector2(1.1f, 0);
        s.cloudsLayer.textureScale    = 1.0f;

        s.skyLayer.textureNameBase = "SKYDAY_LAYER0_A0.TGA";
        s.skyLayer.textureAlpha    = 1.0f;
        s.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
        s.skyLayer.textureScale    = 1.0f;
        break;

      case SkyPresetType::Day2:
        s.time = TIME_KEY_1;

        s.baseColor      = bs::Color(255, 250, 235) / 255.0f;
        s.fogColor       = mSkyColor;
        s.domeColorUpper = bs::Color(255, 255, 255) / 255.0f;

        s.fogDistance = 0.05f;
        s.isSunActive = true;

        s.cloudsLayer.textureNameBase = "SKYDAY_LAYER1_A0.TGA";
        s.cloudsLayer.textureAlpha    = 0.0f;
        s.cloudsLayer.textureSpeed    = bs::Vector2(1.1f, 0);
        s.cloudsLayer.textureScale    = 1.0f;

        s.skyLayer.textureNameBase = "SKYDAY_LAYER0_A0.TGA";
        s.skyLayer.textureAlpha    = 1.0f;
        s.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
        s.skyLayer.textureScale    = 1.0f;
        break;

      case SkyPresetType::Evening:
        s.time = TIME_KEY_2;

        s.baseColor      = bs::Color(255, 185, 170) / 255.0f;
        s.fogColor       = bs::Color(170, 70, 50) / 255.0f;
        s.domeColorUpper = bs::Color(255, 255, 255) / 255.0f;

        s.fogDistance = 0.2f;
        s.isSunActive = true;

        s.cloudsLayer.textureNameBase = "SKYDAY_LAYER1_A0.TGA";
        s.cloudsLayer.textureAlpha    = 0.5f;
        s.cloudsLayer.textureSpeed    = bs::Vector2(1.1f, 0);
        s.cloudsLayer.textureScale    = 1.0f;

        s.skyLayer.textureNameBase = "SKYDAY_LAYER0_A0.TGA";
        s.skyLayer.textureAlpha    = 0.5f;
        s.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
        s.skyLayer.textureScale    = 1.0f;
        break;

      case SkyPresetType::Night0:
        s.time = TIME_KEY_3;

        s.baseColor      = bs::Color(105, 105, 195) / 255.0f;
        s.fogColor       = bs::Color(20, 20, 60) / 255.0f;
        s.domeColorUpper = bs::Color(55, 55, 155) / 255.0f;

        s.fogDistance = 0.1f;
        s.isSunActive = false;

        s.cloudsLayer.textureNameBase = "SKYNIGHT_LAYER0_A0.TGA";
        s.cloudsLayer.textureAlpha    = 1.0f;
        s.cloudsLayer.textureSpeed    = bs::Vector2(0.0f, 0);
        s.cloudsLayer.textureScale    = 4.0f;

        s.skyLayer.textureNameBase = "SKYNIGHT_LAYER1_A0.TGA";
        s.skyLayer.textureAlpha    = 0.0f;
        s.skyLayer.textureSpeed    = bs::Vector2(0.0f, 0);
        s.skyLayer.textureScale    = 1.0f;
        break;

      case SkyPresetType::Night1:
        s.time = TIME_KEY_4;

        s.baseColor      = bs::Color(40, 60, 210) / 255.0f;
        s.fogColor       = bs::Color(5, 5, 20) / 255.0f;
        s.domeColorUpper = bs::Color(55, 55, 155) / 255.0f;

        s.fogDistance = 0.1f;
        s.isSunActive = false;

        s.cloudsLayer.textureNameBase = "SKYNIGHT_LAYER0_A0.TGA";
        s.cloudsLayer.textureAlpha    = 1.0f;
        s.cloudsLayer.textureSpeed    = bs::Vector2(0.0f, 0);
        s.cloudsLayer.textureScale    = 4.0f;

        s.skyLayer.textureNameBase = "SKYNIGHT_LAYER1_A0.TGA";
        s.skyLayer.textureAlpha    = 215.0f / 255.0f;
        s.skyLayer.textureSpeed    = bs::Vector2(0.0f, 0);
        s.skyLayer.textureScale    = 1.0f;
        break;

      case SkyPresetType::Night2:
        s.time = TIME_KEY_5;

        s.baseColor      = bs::Color(40, 60, 210) / 255.0f;
        s.fogColor       = bs::Color(5, 5, 20) / 255.0f;
        s.domeColorUpper = bs::Color(55, 55, 155) / 255.0f;

        s.fogDistance = 0.1f;
        s.isSunActive = false;

        s.cloudsLayer.textureNameBase = "SKYNIGHT_LAYER0_A0.TGA";
        s.cloudsLayer.textureAlpha    = 1.0f;
        s.cloudsLayer.textureSpeed    = bs::Vector2(0.0f, 0);
        s.cloudsLayer.textureScale    = 4.0f;

        s.skyLayer.textureNameBase = "SKYNIGHT_LAYER1_A0.TGA";
        s.skyLayer.textureAlpha    = 0.0f;
        s.skyLayer.textureSpeed    = bs::Vector2(0.0f, 0);
        s.skyLayer.textureScale    = 1.0f;
        break;

      case SkyPresetType::Dawn:
        s.time = TIME_KEY_6;

        s.baseColor      = bs::Color(190, 160, 255) / 255.0f;
        s.fogColor       = bs::Color(80, 60, 105) / 255.0f;
        s.domeColorUpper = bs::Color(255, 255, 255) / 255.0f;

        s.fogDistance = 0.5f;
        s.isSunActive = true;

        s.cloudsLayer.textureNameBase = "SKYNIGHT_LAYER0_A0.TGA";
        s.cloudsLayer.textureAlpha    = 0.5f;
        s.cloudsLayer.textureSpeed    = bs::Vector2(1.1f, 0);
        s.cloudsLayer.textureScale    = 1.0f;

        s.skyLayer.textureNameBase = "SKYNIGHT_LAYER1_A0.TGA";
        s.skyLayer.textureAlpha    = 0.5f;
        s.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
        s.skyLayer.textureScale    = 1.0f;
        break;

      default:
        break;
    }
  }

  void SkyColoring::fillSkyStates()
  {
    // Fill all sky-states, in order
    for (unsigned int i = 0; i < mSkyStates.size(); i++)
    {
      initSkyState(static_cast<SkyPresetType>(i), mSkyStates[i]);
    }
  }

  void SkyColoring::calculateFogDistanceAndColor(float& nearFog, float& farFog,
                                                 bs::Color& finalFogColor) const
  {
    float farPlane             = bs::gSceneManager().getMainCamera()->getFarClipDistance();
    bs::Vector3 cameraPosition = bs::gSceneManager().getMainCamera()->getTransform().pos();

    // FIXME: Find proper bounds. Using the world mesh would be okay here.
    bs::AABox worldBBox = bs::AABox(bs::Vector3(0, 0, 0), bs::Vector3(100, 100, 100));

    // Note: These are some magic values to match what gothic does
    float fogMidrange = farPlane * 0.4f;
    float fogMidDelta = farPlane - fogMidrange;

    // Do heightfog-approximation
    float ytotal  = worldBBox.getMax().y - worldBBox.getMin().y;
    float fogMinY = worldBBox.getMin().y + 0.5f * ytotal;
    float fogMaxY = worldBBox.getMin().y + 0.7f * ytotal;

    // Scale fog back depending on how high the camera is
    float fogScale = fogMaxY - fogMinY != 0 ? (cameraPosition.y - fogMinY) / (fogMaxY - fogMinY) : 0;

    fogScale = bs::Math::clamp(fogScale, 0.0f, 1.0f);

    // Fog should be at least our set distance
    fogScale = bs::Math::max(mMasterState.fogDistance, fogScale);

    farFog = fogMidrange + (1.0f - fogScale) * fogMidDelta;

    // Apply some user value // FIXME: This should have a getter/setter and all that stuff
    const float userFogScale = 1.0f;
    farFog *= userFogScale;
    nearFog = farFog * 0.3f;

    // REGoth - specific: Let the fog be a little closer because of the long view-distances
    nearFog *= 0.5f;

    farFog *= 0.4f;
    nearFog *= 0.4f;

    // Fix up the fog color. The fog should get less intense with decrasing fogFar

    // Compute intensity based on the ZenGins color-base
    bs::Color base = bs::Color(0.299f, 0.587f, 0.114f);

    // Original engine uses only the red component here as well. Who knows why.
    bs::Color baseColor =
        bs::Color(mMasterState.fogColor.r, mMasterState.fogColor.r, mMasterState.fogColor.r);

    // Calculate intensity
    float intensityValue = std::min(
        baseColor.r * base.r + baseColor.g * base.g + baseColor.b * base.b, 120.0f / 255.0f);

    bs::Color intensity  = bs::Color(intensityValue, intensityValue, intensityValue);
    float intensityScale = fogScale * 0.5f;

    // Calculate actual fog color
    finalFogColor = (1.0f - intensityScale) * mMasterState.fogColor + intensityScale * intensity;
  }

  void SkyColoring::onWorldNameChanged(const bs::String& /* newWorldName */)
  {
    // Must come before loading the config since fillSkyStates initializes everything with defaults
    fillSkyStates();

    // reloadAllSkyTextures(newWorldName); // TODO: Needs to be reimplemented, thus commented out
  }

  bool SkyColoring::isNightTime() const
  {
    return mMasterState.time >= TIME_KEY_1 && mMasterState.time <= TIME_KEY_7;
  }

  bs::Color SkyColoring::getPolyCloudsLayerColor()
  {
    bs::Color color;

    if (isNightTime())
    {
      color = bs::Color::White;
    }
    else
    {
      color = mMasterState.domeColorUpper;
    }

    // At night, we want to keep the clouds white instead of getting a blueish tint
    if (mMasterState.time >= TIME_KEY_3 && mMasterState.time <= TIME_KEY_5)
    {
      color = 0.5f * (color + bs::Color::White);
    }

    return color;
  }
}  // namespace REGoth
