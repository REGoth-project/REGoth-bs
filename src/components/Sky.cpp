#include <components/Sky.hpp>

#include <functional>
#include <memory>

#include <Components/BsCRenderable.h>
#include <Material/BsMaterial.h>
#include <RenderAPI/BsViewport.h>
#include <Renderer/BsCamera.h>
#include <Resources/BsBuiltinResources.h>
#include <Scene/BsSceneManager.h>

#include <BsZenLib/ImportMaterial.hpp>
#include <BsZenLib/ImportTexture.hpp>

#include <RTTI/RTTI_Sky.hpp>
#include <components/GameClock.hpp>
#include <components/GameWorld.hpp>
#include <components/SkyStateGenerator.hpp>
#include <exception/Assert.hpp>
#include <exception/Throw.hpp>
#include <original-content/VirtualFileSystem.hpp>

using namespace REGoth;

Sky::Sky(const bs::HSceneObject& parent, HGameWorld gameWorld, const RenderMode& renderMode,
         const bs::Color& skyColor)
    : bs::Component{parent}
    , mSkyStateGen{skyColor, gameWorld->worldName()}
    , mGameWorld{gameWorld}
    , mRenderMode{renderMode}
    , mSkyRenderer{
          std::bind(renderMode == RenderMode::Plane ? &Sky::renderSkyPlane : &Sky::renderSkyDome,
                    this, std::placeholders::_1)}
{
  setName("Sky");
}

Sky::~Sky()
{
  // pass
}

void Sky::onInitialized()
{
  switch (mRenderMode)
  {
    case RenderMode::Plane:
    {
      const bs::HShader shader =
          bs::BuiltinResources::instance().getBuiltinShader(bs::BuiltinShader::Transparent);
      mSkyMaterial = bs::Material::create(shader);

      bs::HSceneObject so = bs::SceneObject::create("SkyPlane");
      so->setPosition(bs::Vector3{0, 200, 0});
      mSkyRenderable = so->addComponent<bs::CRenderable>();

      const bs::HMesh mesh = bs::gBuiltinResources().getMesh(bs::BuiltinMesh::Quad);
      mSkyRenderable->setMesh(mesh);
      mSkyRenderable->setMaterial(mSkyMaterial);

      const float scale = 20000.0f;
      so->setScale(bs::Vector3{scale, 1.0f, scale});
      break;
    }

    case RenderMode::Dome:
    {
      // TODO: Implement sky dome rendering.
      REGOTH_THROW(NotImplementedException, "Dome sky not yet implemented.");
      break;
    }
  }
}

void Sky::update()
{
  // Update the sky state.
  const float dayRatio = mGameWorld->gameclock()->getDayRatio();
  REGOTH_ASSERT_VAL(dayRatio, 0.f <= dayRatio && dayRatio < 1.f, "Must be in [0, 1)");
  std::shared_ptr<const SkyState> skyState = mSkyStateGen.update(dayRatio);

  // Render fog and sky.
  renderFog(skyState->fogColor, skyState->fogNear, skyState->fogFar);
  mSkyRenderer(skyState);
}

void Sky::renderFog(const bs::Color& fogColor, float fogNear, float fogFar) const
{
  REGOTH_ASSERT(fogNear < fogFar, "fogNear must be smaller than fogFar");

  // TODO: Account for distances using fogNear and fogFar.

  const auto& camera = bs::gSceneManager().getMainCamera();
  camera->getViewport()->setClearColorValue(fogColor);
}

void Sky::renderSkyPlane(std::shared_ptr<const SkyState> skyState) const
{
  mSkyMaterial->setTexture("gAlbedoTex", skyState->skyLayer.texture);
}

void Sky::renderSkyDome(std::shared_ptr<const SkyState> /* skyState */) const
{
  // TODO: Implement sky dome rendering.
  REGOTH_THROW(NotImplementedException, "Dome sky not yet implemented.");
}

REGOTH_DEFINE_RTTI(Sky)
