#include <components/Sky.hpp>

#include <RenderAPI/BsViewport.h>
#include <Renderer/BsCamera.h>
#include <Scene/BsSceneManager.h>

#include <RTTI/RTTI_Sky.hpp>
#include <components/GameClock.hpp>
#include <components/GameWorld.hpp>
#include <components/SkyColoring.hpp>

namespace REGoth
{
  Sky::Sky(const bs::HSceneObject& parent, HGameWorld gameWorld, const bs::Color& skyColor)
      : bs::Component(parent)
      , mGameWorld(gameWorld)
      , mSkyColor{skyColor}
  {
    setName("Sky");
  }

  Sky::~Sky()
  {
    // pass
  }

  void Sky::onInitialized()
  {
    if (!mSkyColoring)
    {
      mSkyColoring = bs::bs_shared_ptr_new<SkyColoring>(mSkyColor);
      mSkyColoring->fillSkyStates();
    }
  }

  void Sky::update()
  {
    mSkyColoring->interpolate(mGameWorld->gameclock()->getDayRatio());

    applySkySettingsToCamera();
  }

  void Sky::applySkySettingsToCamera() const
  {
    const auto& camera = bs::gSceneManager().getMainCamera();
    float near;
    float far;
    bs::Color fogColor;

    mSkyColoring->calculateFogDistanceAndColor(near, far, fogColor);

    // TODO: Use fog near and far
    (void)near;
    (void)far;

    // FIXME: Fog color can actually be a little bit different. Get the correct color from master
    // state.
    camera->getViewport()->setClearColorValue(fogColor);
  }

  REGOTH_DEFINE_RTTI(Sky)
}  // namespace REGoth
