#include <components/SkyStateGenerator.hpp>

#include <memory>
#include <tuple>

#include <Image/BsColor.h>
#include <Math/BsMath.h>
#include <Renderer/BsCamera.h>
#include <Scene/BsSceneManager.h>

#include <original-content/OriginalGameResources.hpp>

using namespace REGoth;

std::vector<SkyState> initPresets(const bs::Color& skyColor, const bs::String& worldName)
{
  SkyState day1;
  day1.time                     = 0.00f;
  day1.baseColor                = bs::Color(255, 250, 235) / 255.0f;
  day1.fogColor                 = skyColor;
  day1.domeColorUpper           = bs::Color(255, 255, 255) / 255.0f;
  day1.fogDistance              = 0.05f;
  day1.isSunActive              = true;
  day1.cloudsLayer.textureName  = "SKYDAY_" + worldName + "_LAYER1_A0.TGA";
  day1.cloudsLayer.textureAlpha = 215.0f / 255.0f;
  day1.cloudsLayer.textureSpeed = bs::Vector2(1.1f, 0);
  day1.cloudsLayer.textureScale = 1.0f;
  day1.skyLayer.textureName     = "SKYDAY_" + worldName + "_LAYER0_A0.TGA";
  day1.skyLayer.textureAlpha    = 1.0f;
  day1.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
  day1.skyLayer.textureScale    = 1.0f;

  SkyState day2;
  day2.time                     = 0.25f;
  day2.baseColor                = bs::Color(255, 250, 235) / 255.0f;
  day2.fogColor                 = skyColor;
  day2.domeColorUpper           = bs::Color(255, 255, 255) / 255.0f;
  day2.fogDistance              = 0.05f;
  day2.isSunActive              = true;
  day2.cloudsLayer.textureName  = "SKYDAY_" + worldName + "_LAYER1_A0.TGA";
  day2.cloudsLayer.textureAlpha = 0.0f;
  day2.cloudsLayer.textureSpeed = bs::Vector2(1.1f, 0);
  day2.cloudsLayer.textureScale = 1.0f;
  day2.skyLayer.textureName     = "SKYDAY_" + worldName + "_LAYER0_A0.TGA";
  day2.skyLayer.textureAlpha    = 1.0f;
  day2.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
  day2.skyLayer.textureScale    = 1.0f;

  SkyState dusk;
  dusk.time                     = 0.30f;
  dusk.baseColor                = bs::Color(255, 185, 170) / 255.0f;
  dusk.fogColor                 = bs::Color(170, 70, 50) / 255.0f;
  dusk.domeColorUpper           = bs::Color(255, 255, 255) / 255.0f;
  dusk.fogDistance              = 0.2f;
  dusk.isSunActive              = true;
  dusk.cloudsLayer.textureName  = "SKYDAY_" + worldName + "_LAYER1_A0.TGA";
  dusk.cloudsLayer.textureAlpha = 0.5f;
  dusk.cloudsLayer.textureSpeed = bs::Vector2(1.1f, 0);
  dusk.cloudsLayer.textureScale = 1.0f;
  dusk.skyLayer.textureName     = "SKYDAY_" + worldName + "_LAYER0_A0.TGA";
  dusk.skyLayer.textureAlpha    = 0.5f;
  dusk.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
  dusk.skyLayer.textureScale    = 1.0f;

  SkyState night0;
  night0.time                     = 0.35f;
  night0.baseColor                = bs::Color(105, 105, 195) / 255.0f;
  night0.fogColor                 = bs::Color(20, 20, 60) / 255.0f;
  night0.domeColorUpper           = bs::Color(55, 55, 155) / 255.0f;
  night0.fogDistance              = 0.1f;
  night0.isSunActive              = false;
  night0.cloudsLayer.textureName  = "SKYNIGHT_" + worldName + "_LAYER0_A0.TGA";
  night0.cloudsLayer.textureAlpha = 1.0f;
  night0.cloudsLayer.textureSpeed = bs::Vector2(0.0f, 0);
  night0.cloudsLayer.textureScale = 4.0f;
  night0.skyLayer.textureName     = "SKYNIGHT_" + worldName + "_LAYER1_A0.TGA";
  night0.skyLayer.textureAlpha    = 0.0f;
  night0.skyLayer.textureSpeed    = bs::Vector2(0.0f, 0);
  night0.skyLayer.textureScale    = 1.0f;

  SkyState night1;
  night1.time                     = 0.50f;
  night1.baseColor                = bs::Color(40, 60, 210) / 255.0f;
  night1.fogColor                 = bs::Color(5, 5, 20) / 255.0f;
  night1.domeColorUpper           = bs::Color(55, 55, 155) / 255.0f;
  night1.fogDistance              = 0.1f;
  night1.isSunActive              = false;
  night1.cloudsLayer.textureName  = "SKYNIGHT_" + worldName + "_LAYER0_A0.TGA";
  night1.cloudsLayer.textureAlpha = 1.0f;
  night1.cloudsLayer.textureSpeed = bs::Vector2(0.0f, 0);
  night1.cloudsLayer.textureScale = 4.0f;
  night1.skyLayer.textureName     = "SKYNIGHT_" + worldName + "_LAYER1_A0.TGA";
  night1.skyLayer.textureAlpha    = 215.0f / 255.0f;
  night1.skyLayer.textureSpeed    = bs::Vector2(0.0f, 0);
  night1.skyLayer.textureScale    = 1.0f;

  SkyState night2;
  night2.time                     = 0.65f;
  night2.baseColor                = bs::Color(40, 60, 210) / 255.0f;
  night2.fogColor                 = bs::Color(5, 5, 20) / 255.0f;
  night2.domeColorUpper           = bs::Color(55, 55, 155) / 255.0f;
  night2.fogDistance              = 0.1f;
  night2.isSunActive              = false;
  night2.cloudsLayer.textureName  = "SKYNIGHT_" + worldName + "_LAYER0_A0.TGA";
  night2.cloudsLayer.textureAlpha = 1.0f;
  night2.cloudsLayer.textureSpeed = bs::Vector2(0.0f, 0);
  night2.cloudsLayer.textureScale = 4.0f;
  night2.skyLayer.textureName     = "SKYNIGHT_" + worldName + "_LAYER1_A0.TGA";
  night2.skyLayer.textureAlpha    = 0.0f;
  night2.skyLayer.textureSpeed    = bs::Vector2(0.0f, 0);
  night2.skyLayer.textureScale    = 1.0f;

  SkyState dawn;
  dawn.time                     = 0.70f;
  dawn.baseColor                = bs::Color(190, 160, 255) / 255.0f;
  dawn.fogColor                 = bs::Color(80, 60, 105) / 255.0f;
  dawn.domeColorUpper           = bs::Color(255, 255, 255) / 255.0f;
  dawn.fogDistance              = 0.5f;
  dawn.isSunActive              = true;
  dawn.cloudsLayer.textureName  = "SKYNIGHT_" + worldName + "_LAYER0_A0.TGA";
  dawn.cloudsLayer.textureAlpha = 0.5f;
  dawn.cloudsLayer.textureSpeed = bs::Vector2(1.1f, 0);
  dawn.cloudsLayer.textureScale = 1.0f;
  dawn.skyLayer.textureName     = "SKYNIGHT_" + worldName + "_LAYER1_A0.TGA";
  dawn.skyLayer.textureAlpha    = 0.5f;
  dawn.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
  dawn.skyLayer.textureScale    = 1.0f;

  SkyState day0;
  day0.time                     = 0.75f;
  day0.baseColor                = bs::Color(255, 250, 235) / 255.0f;
  day0.fogColor                 = skyColor;
  day0.domeColorUpper           = bs::Color(255, 255, 255) / 255.0f;
  day0.fogDistance              = 0.2f;
  day0.isSunActive              = true;
  day0.cloudsLayer.textureName  = "SKYDAY_" + worldName + "_LAYER1_A0.TGA";
  day0.cloudsLayer.textureAlpha = 0.0f;
  day0.cloudsLayer.textureSpeed = bs::Vector2(1.1f, 0);
  day0.cloudsLayer.textureScale = 1.0f;
  day0.skyLayer.textureName     = "SKYDAY_" + worldName + "_LAYER0_A0.TGA";
  day0.skyLayer.textureAlpha    = 1.0f;
  day0.skyLayer.textureSpeed    = bs::Vector2(0.3f, 0);
  day0.skyLayer.textureScale    = 1.0f;

  std::vector<SkyState> presets{day1, day2, dusk, night0, night1, night2, dawn, day0};

  for (SkyState& preset : presets)
  {
    preset.cloudsLayer.texture = gOriginalGameResources().texture(preset.cloudsLayer.textureName);
    preset.skyLayer.texture    = gOriginalGameResources().texture(preset.skyLayer.textureName);
  }

  return presets;
}

SkyStateGenerator::SkyStateGenerator(const bs::Color& skyColor, const bs::String& worldName)
    : mSkyStatePresets{initPresets(skyColor, worldName)}
{
  mCurrentSkyState = std::make_shared<SkyState>();
}

SkyStateGenerator::~SkyStateGenerator()
{
  // pass
}

const std::shared_ptr<const SkyState> SkyStateGenerator::update(float dayRatio)
{
  // Set time to current state.
  mCurrentSkyState->time = std::fmod(dayRatio + 0.5f, 1.0f);

  // Find two presets to interpolate between by index.
  bs::UINT32 p1i, p2i;
  std::tie(p1i, p2i) = findPresetIndices();

  // Interpolate presets and save to current state.
  interpolatePresets(mSkyStatePresets[p1i], mSkyStatePresets[p2i]);

  // FIXME: There is some stuff about levelchanges here. Like, "turn off rain when on dragonisland"

  // FIXME: Multiply fogColor with 0.8 when using dome!

  return mCurrentSkyState;
}

std::tuple<bs::UINT32, bs::UINT32> SkyStateGenerator::findPresetIndices() const
{
  bs::UINT32 p1i = static_cast<bs::UINT32>(mSkyStatePresets.size() - 1);
  bs::UINT32 p2i = 0;

  for (bs::UINT32 i = 0; i < mSkyStatePresets.size(); ++i)
  {
    // Since the sky states are ordered, our start state is the first one which is supposed to start
    // *before* the current time.  The target state is then the one right after.  Since it's easier
    // to search for the target state, which is the first state which has a time larger than the
    // current time, we search for that. The start state is the one right before it.
    if (mCurrentSkyState->time < mSkyStatePresets[i].time)
    {
      // Subtracting 1 will not cause a negative numbers here since the first state has a time of
      // 0.0, so the check should never pass for it.  If everything is set up correctly that is.
      p1i = (i - 1) % mSkyStatePresets.size();
      p2i = i % mSkyStatePresets.size();
      break;
    }
  }

  return {p1i, p2i};
}

void SkyStateGenerator::interpolatePresets(const SkyState& p1, const SkyState& p2)
{
  // Handle case time >= 0.75f
  const float timeS1 = p2.time < p1.time ? p2.time + 1.0f : p2.time;

  // Scale up time difference to [0,1]
  const float t = (mCurrentSkyState->time - p1.time) / (timeS1 - p1.time);

  // Interpolate values
  mCurrentSkyState->baseColor      = p1.baseColor + t * (p2.baseColor - p1.baseColor);
  mCurrentSkyState->fogColor       = p1.fogColor + t * (p2.fogColor - p1.fogColor);
  mCurrentSkyState->fogDistance    = p1.fogDistance + t * (p2.fogDistance - p1.fogDistance);
  mCurrentSkyState->domeColorUpper = p1.domeColorUpper + t * (p2.domeColorUpper - p1.domeColorUpper);

  mCurrentSkyState->cloudsLayer = p1.cloudsLayer;
  mCurrentSkyState->cloudsLayer.textureAlpha =
      bs::Math::lerp(t, p1.cloudsLayer.textureAlpha, p2.cloudsLayer.textureAlpha);
  mCurrentSkyState->cloudsLayer.textureScale =
      bs::Math::lerp(t, p1.cloudsLayer.textureScale, p2.cloudsLayer.textureScale);

  mCurrentSkyState->skyLayer = p1.skyLayer;
  mCurrentSkyState->skyLayer.textureAlpha =
      bs::Math::lerp(t, p1.skyLayer.textureAlpha, p2.skyLayer.textureAlpha);
  mCurrentSkyState->skyLayer.textureScale =
      bs::Math::lerp(t, p1.skyLayer.textureScale, p2.skyLayer.textureScale);

  const float farPlane              = bs::gSceneManager().getMainCamera()->getFarClipDistance();
  const bs::Vector3& cameraPosition = bs::gSceneManager().getMainCamera()->getTransform().pos();

  // FIXME: Find proper bounds. Using the world mesh would be okay here.
  const bs::AABox worldBBox = bs::AABox(bs::Vector3(0, 0, 0), bs::Vector3(100, 100, 100));

  // Note: These are some magic values to match what Gothic does
  const float fogMidrange = farPlane * 0.4f;
  const float fogMidDelta = farPlane - fogMidrange;

  // Do heightfog-approximation
  const float ytotal  = worldBBox.getMax().y - worldBBox.getMin().y;
  const float fogMinY = worldBBox.getMin().y + 0.5f * ytotal;
  const float fogMaxY = worldBBox.getMin().y + 0.7f * ytotal;

  // Scale fog back depending on how high the camera is
  // TODO: Do not compare float operation with != or ==
  float fogScale = fogMaxY - fogMinY != 0 ? (cameraPosition.y - fogMinY) / (fogMaxY - fogMinY) : 0;

  fogScale = bs::Math::clamp(fogScale, 0.0f, 1.0f);

  // Fog should be at least our set distance
  fogScale = bs::Math::max(mCurrentSkyState->fogDistance, fogScale);

  mCurrentSkyState->fogFar = fogMidrange + (1.0f - fogScale) * fogMidDelta;

  // Apply some user value
  // FIXME: This should have a getter/setter and all that stuff
  const float userFogScale = 1.0f;
  mCurrentSkyState->fogFar *= userFogScale;
  mCurrentSkyState->fogNear = mCurrentSkyState->fogFar * 0.3f;

  // REGoth - specific: Let the fog be a little closer because of the long view-distances
  mCurrentSkyState->fogNear *= 0.5f;

  mCurrentSkyState->fogFar *= 0.4f;
  mCurrentSkyState->fogNear *= 0.4f;

  // Fix up the fog color. The fog should get less intense with decrasing fogFar

  // Compute intensity based on the ZenGins color-base
  const bs::Color base = bs::Color(0.299f, 0.587f, 0.114f);

  // Original engine uses only the red component here as well. Who knows why.
  const bs::Color baseColor = bs::Color(mCurrentSkyState->fogColor.r, mCurrentSkyState->fogColor.r,
                                        mCurrentSkyState->fogColor.r);

  // Calculate intensity
  const float intensityValue =
      std::min(baseColor.r * base.r + baseColor.g * base.g + baseColor.b * base.b, 120.0f / 255.0f);

  const bs::Color intensity  = bs::Color(intensityValue, intensityValue, intensityValue);
  const float intensityScale = fogScale * 0.5f;

  // Calculate actual fog color
  mCurrentSkyState->fogColor =
      (1.0f - intensityScale) * mCurrentSkyState->fogColor + intensityScale * intensity;
}

bs::Color SkyStateGenerator::getPolyCloudsLayerColor()
{
  bs::Color color;

  // At night.
  if (mCurrentSkyState->time >= 0.25f && mCurrentSkyState->time <= 0.75f)
  {
    color = bs::Color::White;
  }
  else
  {
    color = mCurrentSkyState->domeColorUpper;
  }

  // At night, we want to keep the clouds white instead of getting a blueish tint.
  if (mCurrentSkyState->time >= 0.35f && mCurrentSkyState->time <= 0.65f)
  {
    color = 0.5f * (color + bs::Color::White);
  }

  return color;
}
