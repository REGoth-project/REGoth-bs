#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include <Components/BsCCamera.h>
#include <Scene/BsPrefab.h>
#include <Scene/BsSceneObject.h>
#include <components/Character.hpp>
#include <components/CharacterKeyboardInput.hpp>
#include <components/GameWorld.hpp>
#include <exception/Throw.hpp>
#include <original-content/OriginalGameFiles.hpp>
#include <original-content/VirtualFileSystem.hpp>

class REGothWorldViewer : public REGoth::REGothEngine
{
public:
  void loadModPackages(const REGoth::OriginalGameFiles& files) override
  {
    using namespace REGoth;

    for (auto p : files.allModPackages())
    {
      bs::gDebug().logDebug("[WorldViewer] Loading Mod: " + p.toString());
      gVirtualFileSystem().loadPackage(p);
    }

    for (auto zen : gVirtualFileSystem().listByExtension(".ZEN"))
    {
      bs::gDebug().logDebug("[WorldViewer] Found ZEN: " + zen);
    }
  }

  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();

    // mFPSCamera = mMainCamera->SO()->addComponent<bs::FPSCamera>();
  }

  void setupScene() override
  {
    using namespace REGoth;

    const bs::String WORLD    = "ANFANG.ZEN";
    const bs::String SAVEGAME = "WorldViewer-" + WORLD;

    bs::HPrefab worldPrefab = GameWorld::load(SAVEGAME);
    HGameWorld world;

    if (!worldPrefab)
    {
      world = GameWorld::importZEN(WORLD);

      HCharacter hero = world->insertCharacter("PC_HERO", "START_DRAGONISLAND");
      hero->useAsHero();
      hero->SO()->addComponent<CharacterKeyboardInput>();

      world->runInitScripts();

      world->save(SAVEGAME);
    }
    else
    {
      bs::HSceneObject worldSO = worldPrefab->instantiate();

      world = worldSO->getComponent<GameWorld>();
    }

    bs::HSceneObject heroSO = world->SO()->findChild("PC_HERO");

    if (!heroSO)
    {
      REGOTH_THROW(InvalidStateException, "Expected PC_HERO in world");
    }

    HCharacter hero = heroSO->getComponent<Character>();

    mMainCamera->SO()->setPosition(bs::Vector3(2, 0.5f, 0));
    mMainCamera->SO()->lookAt(bs::Vector3(bs::BsZero));
    mMainCamera->SO()->setParent(hero->SO(), false);
  }

protected:
  bs::HFPSCamera mFPSCamera;
};

int main(int argc, char** argv)
{
  REGothWorldViewer regoth;

  return REGoth::main(regoth, argc, argv);
}
