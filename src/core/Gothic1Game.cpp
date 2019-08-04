#include <core/Gothic1Game.hpp>

#include <Components/BsCCamera.h>
#include <Image/BsColor.h>
#include <Scene/BsPrefab.h>

#include <components/Character.hpp>
#include <components/CharacterKeyboardInput.hpp>
#include <components/GameWorld.hpp>
#include <components/GameplayUI.hpp>
#include <components/Sky.hpp>

using namespace REGoth;

void Gothic1Game::setupMainCamera()
{
  Engine::setupMainCamera();

  mThirdPersonCamera = mMainCamera->SO()->addComponent<ThirdPersonCamera>();
}

void Gothic1Game::setupScene()
{
  const bs::String WORLD    = "WORLD.ZEN";
  const bs::String SAVEGAME = "WorldViewer-" + WORLD;

  bs::HPrefab worldPrefab = GameWorld::load(SAVEGAME);
  HGameWorld world;

  if (!worldPrefab)
  {
    world = GameWorld::importZEN(WORLD);

    HCharacter hero = world->insertCharacter("PC_HERO", WORLD_STARTPOINT);
    hero->useAsHero();
    hero->SO()->addComponent<CharacterKeyboardInput>(world);

    world->runInitScripts();

    world->save(SAVEGAME);
  }
  else
  {
    bs::HSceneObject worldSO = worldPrefab->instantiate();

    world = worldSO->getComponent<GameWorld>();
  }

  const bs::Color skyColor = bs::Color{114, 93, 82} / 255.0f;
  world->SO()->addComponent<Sky>(world, skyColor);

  bs::HSceneObject heroSO = world->SO()->findChild("PC_HERO");

  if (!heroSO)
  {
    REGOTH_THROW(InvalidStateException, "Expected PC_HERO in world");
  }

  HCharacter hero = heroSO->getComponent<Character>();

  mThirdPersonCamera->follow(hero);

  GameplayUI::createGlobal(mMainCamera);
}
