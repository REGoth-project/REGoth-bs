#include "GameWorld.hpp"
#include <BsZenLib/ImportPath.hpp>
#include <Components/BsCCharacterController.h>
#include <RTTI/RTTI_GameWorld.hpp>
#include <Resources/BsResources.h>
#include <Scene/BsPrefab.h>
#include <components/Character.hpp>
#include <components/CharacterAI.hpp>
#include <components/Item.hpp>
#include <components/VisualCharacter.hpp>
#include <components/Waynet.hpp>
#include <daedalus/DATFile.h>
#include <exception/Throw.hpp>
#include <original-content/VirtualFileSystem.hpp>
#include <scripting/ScriptVMForGameWorld.hpp>
#include <world/internals/ConstructFromZEN.hpp>

namespace REGoth
{
  GameWorld::GameWorld(const bs::HSceneObject& parent, const bs::String& zenFile)
      : bs::Component(parent)
      , mZenFile(zenFile)
  {
    setName("GameWorld");
  }

  GameWorld::GameWorld(const bs::HSceneObject& parent, Empty empty)
      : bs::Component(parent)
      , mZenFile("")
  {
    setName("GameWorld");
  }

  GameWorld::~GameWorld()
  {
  }

  void GameWorld::onInitialized()
  {
    // If this is true here, we're being de-serialized
    if (mIsInitialized) return;

    initScriptVM();

    if (!mZenFile.empty())
    {
      // Import the ZEN and add all scene objects as children to this SO.
      Internals::constructFromZEN(SO(), mZenFile);

      findWaynet();
    }
    else
    {
      // Need to fill in some dummy data on empty worlds
      mWaynet = SO()->addComponent<Waynet>();
    }

    onImportedZEN();

    mIsInitialized = true;
  }

  HItem GameWorld::insertItem(const bs::String& instance, const bs::Transform& transform)
  {
    bs::HSceneObject itemSO = bs::SceneObject::create(instance);
    itemSO->setPosition(transform.pos());
    itemSO->setRotation(transform.rot());

    return itemSO->addComponent<Item>(instance);
  }

  HItem GameWorld::insertItem(const bs::String& instance, const bs::String& spawnPoint)
  {
    bs::HSceneObject spawnPointSO = SO()->findChild(spawnPoint);
    bs::Transform transform;

    if (spawnPointSO)
    {
      transform = spawnPointSO->getTransform();
    }
    else
    {
      // FIXME: What to do on invalid spawnpoints?
      // REGOTH_THROW(InvalidParametersException,
      //     bs::StringUtil::format("Spawnpoint {0} for item of instance {1} does not exist!",
      //                            spawnPoint, instance));
    }

    return insertItem(instance, transform);
  }

  HCharacter GameWorld::insertCharacter(const bs::String& instance, const bs::Transform& transform)
  {
    bs::HSceneObject characterSO = bs::SceneObject::create(instance);
    characterSO->setParent(SO());

    characterSO->setPosition(transform.pos());
    characterSO->setRotation(transform.rot());

    characterSO->addComponent<VisualCharacter>();
    characterSO->addComponent<bs::CCharacterController>();
    characterSO->addComponent<CharacterAI>();

    return characterSO->addComponent<Character>(instance);
  }

  HCharacter GameWorld::insertCharacter(const bs::String& instance, const bs::String& spawnPoint)
  {
    bs::HSceneObject spawnPointSO = SO()->findChild(spawnPoint);
    bs::Transform transform;

    if (spawnPointSO)
    {
      transform = spawnPointSO->getTransform();
    }
    else
    {
      // FIXME: What to do on invalid spawnpoints?
      // REGOTH_THROW(
      //     InvalidParametersException,
      //     bs::StringUtil::format("Spawnpoint '{0}' for character of instance {1} does not
      //     exist!",
      //                            spawnPoint, instance));
    }

    transform.move(
        bs::Vector3(0, 0.5f, 0));  // FIXME: Can we move the center to the feet somehow instead?

    return insertCharacter(instance, transform);
  }

  void GameWorld::initScriptVM()
  {
    bs::Vector<bs::UINT8> data = gVirtualFileSystem().readFile("GOTHIC.DAT");
    Daedalus::DATFile dat(data.data(), data.size());

    mScriptVM = bs::bs_shared_ptr_new<Scripting::ScriptVMForGameWorld>(
        bs::static_object_cast<GameWorld>(getHandle()), dat);

    mScriptVM->initialize();
  }

  HGameWorld GameWorld::importZEN(const bs::String& zenFile)
  {
    bs::HSceneObject rootSO = bs::SceneObject::create("root");

    return rootSO->addComponent<GameWorld>(zenFile);
  }

  void GameWorld::onImportedZEN()
  {
  }

  void GameWorld::runInitScripts()
  {
    mScriptVM->initializeWorld(worldName());
  }

  HGameWorld GameWorld::createEmpty()
  {
    bs::HSceneObject rootSO = bs::SceneObject::create("root");

    return rootSO->addComponent<GameWorld>(EmptyWorld);
  }

  void GameWorld::findWaynet()
  {
    bs::HSceneObject waynetSO = SO()->findChild("Waynet");

    if (!waynetSO)
    {
      REGOTH_THROW(InvalidStateException, "No waynet found in this world?");
    }

    HWaynet waynet = waynetSO->getComponent<Waynet>();

    if (!waynet)
    {
      REGOTH_THROW(InvalidStateException, "Waynet does not have Waynet component?");
    }

    mWaynet = waynet;
  }

  bs::String GameWorld::worldName() const
  {
    return mZenFile.substr(0, mZenFile.find_first_of('.'));
  }

  HCharacter GameWorld::hero() const
  {
    auto scriptObject = mScriptVM->getHero();

    if (scriptObject == Scripting::SCRIPT_OBJECT_HANDLE_INVALID)
      return {};

    bs::HSceneObject heroSO = mScriptVM->mapping().getMappedSceneObject(scriptObject);

    return heroSO->getComponent<Character>();
  }

  void GameWorld::save(const bs::String& saveName)
  {
    bs::HPrefab cached = bs::Prefab::create(SO());

    enum
    {
      Overwrite    = true,
      KeepExisting = false,
    };

    // TODO: Should store at savegame location
    bs::Path path = BsZenLib::GothicPathToCachedWorld(saveName);
    bs::gResources().save(cached, path, Overwrite);
  }

  bs::HPrefab GameWorld::load(const bs::String& saveName)
  {
    // TODO: Should load at savegame location
    bs::Path path = BsZenLib::GothicPathToCachedWorld(saveName);

    return bs::gResources().load<bs::Prefab>(path);
  }

  REGOTH_DEFINE_RTTI(GameWorld)
}  // namespace REGoth
