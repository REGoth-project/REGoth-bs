#include <components/Sky.hpp>

#include <memory>

#include <RenderAPI/BsViewport.h>
#include <Renderer/BsCamera.h>
#include <Scene/BsSceneManager.h>

#include <RTTI/RTTI_Sky.hpp>
#include <components/GameClock.hpp>
#include <components/GameWorld.hpp>
#include <components/SkyStateGenerator.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  Sky::Sky(const bs::HSceneObject& parent, HGameWorld gameWorld, const RenderMode& renderMode,
           const bs::Color& skyColor)
      : bs::Component{parent}
      , mSkyStateGen{skyColor, gameWorld->worldName()}
      , mGameWorld{gameWorld}
      , mRenderMode{renderMode}
  {
    setName("Sky");

    // TODO: Implement sky dome support for Gothic II.
    if (renderMode == RenderMode::Dome)
    {
      REGOTH_THROW(NotImplementedException, "Dome sky not yet implemented.");
    }
  }

  Sky::~Sky()
  {
    // pass
  }

  void Sky::update()
  {
    // Update the sky state.
    const float dayRatio                     = mGameWorld->gameclock()->getDayRatio();
    std::shared_ptr<const SkyState> skyState = mSkyStateGen.update(dayRatio);

    // Render fog and sky.
    renderFog(skyState->fogColor, skyState->fogNear, skyState->fogFar);
    renderSky();
  }

  void Sky::renderFog(const bs::Color& fogColor, float fogNear, float fogFar) const
  {
    // TODO: Account for distances.
    (void)fogNear;
    (void)fogFar;

    const auto& camera = bs::gSceneManager().getMainCamera();
    camera->getViewport()->setClearColorValue(fogColor);
  }

  void Sky::renderSky() const
  {
    // TODO: Render sky.
  }

  REGOTH_DEFINE_RTTI(Sky)
}  // namespace REGoth
