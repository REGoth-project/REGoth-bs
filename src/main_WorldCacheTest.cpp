#include <memory>

#include <BsFPSCamera.h>
#include <Components/BsCCamera.h>
#include <FileSystem/BsFileSystem.h>
#include <GUI/BsCGUIWidget.h>
#include <Resources/BsResources.h>
#include <Scene/BsPrefab.h>
#include <Scene/BsSceneObject.h>
#include <Utility/BsTimer.h>

#include <BsZenLib/ImportPath.hpp>
#include <daedalus/DATFile.h>

#include <core.hpp>
#include <components/AnchoredTextLabels.hpp>
#include <components/GameWorld.hpp>
#include <components/Item.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/VirtualFileSystem.hpp>

class REGothWorldCacheTest : public REGoth::Engine
{
public:
  using REGoth::Engine::Engine;

  void setupMainCamera() override
  {
    REGoth::AbstractEngine::setupMainCamera();

    mMainCamera->SO()->addComponent<bs::FPSCamera>();
  }

  void setupScene() override
  {
    using namespace REGoth;

    const bs::String world    = "OLDWORLD.ZEN";
    const bs::String saveGame = "MySafeGame";

    if (!bs::FileSystem::exists(BsZenLib::GothicPathToCachedWorld(saveGame)))
    {
      REGOTH_LOG(Info, Uncategorized, "[REGothWorldCacheTest] Importing and caching ZEN: {0}",
                 world);

      HGameWorld gameWorld = GameWorld::importZEN(world);

      REGOTH_LOG(Info, Uncategorized, "[REGothWorldCacheTest] Saving world...");

      gameWorld->save(saveGame);
    }
    else
    {
      REGOTH_LOG(Info, Uncategorized, "[REGothWorldCacheTest] Loading world from SaveGame: {0}",
                 world);

      bs::HPrefab world = GameWorld::load(saveGame);
      world->instantiate();
    }
  }
};

int main(int argc, char** argv)
{
  std::unique_ptr<const REGoth::EngineConfig> config =
      REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
  REGothWorldCacheTest engine{std::move(config)};

  return REGoth::runEngine(engine);
}
