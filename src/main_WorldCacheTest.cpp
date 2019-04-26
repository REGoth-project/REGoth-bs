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
#include <scripting/ScriptVMInterface.hpp>
#include <world/GameWorld.hpp>

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

    Scripting::loadGothicDAT(gVirtualFileSystem().readFile("GOTHIC.DAT"));

    const bs::String world = "NEWWORLD.ZEN";

    bs::Timer timer;

    if (!bs::FileSystem::exists(BsZenLib::GothicPathToCachedWorld(world)))
    {
      bs::gDebug().logDebug("[REGothWorldCacheTest] Caching world: " + world);
      bs::gDebug().logDebug("[REGothWorldCacheTest]   - Loading...");

      timer.reset();
      World::loadWorldFromZEN(world, World::GameWorld::Init::NoInitScripts);

      bs::gDebug().logDebug("[REGothWorldCacheTest]   -  ^--> " +
                            bs::toString(timer.getMicroseconds() * bs::Time::MICROSEC_TO_SEC) + "s");

      bs::gDebug().logDebug("[REGothWorldCacheTest]   - Creating prefab...");

      timer.reset();
      bs::HPrefab cached = bs::Prefab::create(gWorld().sceneRoot());

      bs::gDebug().logDebug("[REGothWorldCacheTest]   -  ^--> " +
                            bs::toString(timer.getMicroseconds() * bs::Time::MICROSEC_TO_SEC) + "s");

      bs::gDebug().logDebug("[REGothWorldCacheTest]   - Saving prefab...");

      timer.reset();
      bs::gResources().save(cached, BsZenLib::GothicPathToCachedWorld(world), true);

      bs::gDebug().logDebug("[REGothWorldCacheTest]   -  ^--> " +
                            bs::toString(timer.getMicroseconds() * bs::Time::MICROSEC_TO_SEC) + "s");

      bs::gDebug().logDebug("[REGothWorldCacheTest]   - Done!");
    }
    else
    {
      bs::gDebug().logDebug("[REGothWorldCacheTest] Loading world from Cache: " + world);

      bs::gDebug().logDebug("[REGothWorldCacheTest]   - Loading prefab...");

      timer.reset();
      bs::HPrefab prefab =
          bs::gResources().load<bs::Prefab>(BsZenLib::GothicPathToCachedWorld(world));

      bs::gDebug().logDebug("[REGothWorldCacheTest]   -  ^--> " +
                            bs::toString(timer.getMicroseconds() * bs::Time::MICROSEC_TO_SEC) + "s");

      bs::gDebug().logDebug("[REGothWorldCacheTest]   - Instantiating prefab...");

      timer.reset();
      prefab->instantiate();

      bs::gDebug().logDebug("[REGothWorldCacheTest]   -  ^--> " +
                            bs::toString(timer.getMicroseconds() * bs::Time::MICROSEC_TO_SEC) + "s");

      bs::gDebug().logDebug("[REGothWorldCacheTest]   - Done!");
    }
  }
};

int main(int argc, char** argv)
{
  REGothWorldCacheTest regoth;

  return REGoth::main(regoth, argc, argv);
}
