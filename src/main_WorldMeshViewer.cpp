#include <memory>

#include <BsFPSCamera.h>
#include <Components/BsCCamera.h>
#include <Scene/BsSceneObject.h>

#include <core/Engine.hpp>
#include <world/internals/ConstructFromZEN.hpp>

class REGothWorldMeshViewer : public REGoth::Engine
{
public:
  using REGoth::Engine::Engine;

  void setupMainCamera() override
  {
    REGoth::AbstractEngine::setupMainCamera();

    mFPSCamera = mMainCamera->SO()->addComponent<bs::FPSCamera>();
  }

  void setupScene() override
  {
    REGoth::Internals::loadWorldMeshFromZEN("ADDONWORLD.ZEN");
  }

protected:
  bs::HFPSCamera mFPSCamera;
};

int main(int argc, char** argv)
{
  std::unique_ptr<const REGoth::EngineConfig> config =
      REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
  REGothWorldMeshViewer engine{std::move(config)};

  return REGoth::runEngine(engine);
}
