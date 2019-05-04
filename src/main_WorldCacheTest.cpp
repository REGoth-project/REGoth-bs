#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include "components/AnchoredTextLabels.h"
#include <BsZenLib/ImportPath.hpp>
#include <Components/BsCCamera.h>
#include <FileSystem/BsFileSystem.h>
#include <GUI/BsCGUIWidget.h>
#include <Resources/BsResources.h>
#include <Scene/BsPrefab.h>
#include <Scene/BsSceneObject.h>
#include <Utility/BsTimer.h>
#include <components/Item.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <daedalus/DATFile.h>
#include <exception/Throw.hpp>
#include <original-content/VirtualFileSystem.hpp>
#include <components/GameWorld.hpp>

class REGothWorldCacheTest : public REGoth::REGothEngine
{
public:
  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();

    mMainCamera->SO()->addComponent<bs::FPSCamera>();
  }

  void setupScene() override
  {
    using namespace REGoth;

    const bs::String world = "OLDWORLD.ZEN";
    const bs::String saveGame = "MySafeGame";

    if (!bs::FileSystem::exists(BsZenLib::GothicPathToCachedWorld(saveGame)))
    {
      bs::gDebug().logDebug("[REGothWorldCacheTest] Importing and caching ZEN: " + world);

      HGameWorld gameWorld = GameWorld::importZEN(world);

      bs::gDebug().logDebug("[REGothWorldCacheTest] Saving world...");

      gameWorld->save(saveGame);
    }
    else
    {
      bs::gDebug().logDebug("[REGothWorldCacheTest] Loading world from SaveGame: " + world);

      bs::HPrefab world = GameWorld::load(saveGame);
      world->instantiate();
    }
  }
};

int main(int argc, char** argv)
{
  REGothWorldCacheTest regoth;

  return REGoth::main(regoth, argc, argv);
}
