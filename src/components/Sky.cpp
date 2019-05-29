#include "Sky.hpp"
#include <RenderAPI/BsViewport.h>
#include <Renderer/BsCamera.h>
#include <RTTI/RTTI_Sky.hpp>
#include <Scene/BsSceneManager.h>
#include <components/GameClock.hpp>
#include <components/GameWorld.hpp>
#include <sky/SkyColoring.hpp>

namespace REGoth
{
  Sky::Sky(const bs::HSceneObject& parent, HGameWorld gameWorld)
      : bs::Component(parent)
      , mGameWorld(gameWorld)
  {
    setName("Sky");
  }

  Sky::~Sky()
  {
  }

  void Sky::onInitialized()
  {
    if (!mSkyColoring)
    {
      mSkyColoring = bs::bs_shared_ptr_new<SkyColoring>();
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

    // FIXME: Fog color can actually be a little bit different. Get the correct color from master state.
    camera->getViewport()->setClearColorValue(fogColor);
  }

  REGOTH_DEFINE_RTTI(Sky)

}  // namespace REGoth
