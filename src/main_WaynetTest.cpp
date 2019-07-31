#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include "components/AnchoredTextLabels.hpp"
#include <Components/BsCCamera.h>
#include <GUI/BsCGUIWidget.h>
#include <Scene/BsSceneObject.h>
#include <components/GameWorld.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <exception/Throw.hpp>
#include <original-content/VirtualFileSystem.hpp>

class REGothWaynetTester : public REGoth::REGothEngineDefaultConfig
{
public:
  using REGoth::REGothEngineDefaultConfig::REGothEngineDefaultConfig;

  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();

    mMainCamera->SO()->addComponent<bs::FPSCamera>();

    auto guiSO     = bs::SceneObject::create("GUI");
    auto guiWidget = guiSO->addComponent<bs::CGUIWidget>(mMainCamera);

    auto debugOverlaySO = bs::SceneObject::create("DebugOverlay");
    mTextLabels         = debugOverlaySO->addComponent<REGoth::AnchoredTextLabels>(guiWidget);
    mTextLabels->setMaximumDistance(50.f);
  }

  void setupScene() override
  {
    using namespace REGoth;

    HGameWorld world = GameWorld::importZEN("WORLD.ZEN");

    world->waynet()->debugDraw(mTextLabels);

    // Commented out: Pathfinder stresstest
    // auto from = world->waynet()->findWaypoint("SPAWN_MOLERAT02_SPAWN01");
    // auto to   = world->waynet()->findWaypoint("SPAWN_TALL_PATH_BANDITOS2_02_04");

    // while (true)
    // {
    //   world->waynet()->findWay(from, to);
    // }
  }

protected:
  REGoth::HAnchoredTextLabels mTextLabels;
};

int main(int argc, char** argv)
{
  std::unique_ptr<const REGoth::EngineConfig> config =
      REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
  REGothWaynetTester engine{std::move(config)};

  return REGoth::runEngine(engine);
}
