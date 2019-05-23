#include "GameWorld.hpp"
#include <BsZenLib/ImportPath.hpp>
#include <Components/BsCCharacterController.h>
#include <RTTI/RTTI_GameWorld.hpp>
#include <Resources/BsResources.h>
#include <Scene/BsPrefab.h>
#include <components/Character.hpp>
#include <components/CharacterAI.hpp>
#include <components/CharacterEventQueue.hpp>
#include <components/GameClock.hpp>
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
      bs::HSceneObject so = Internals::constructFromZEN(SO(), mZenFile);

      if (!so)
      {
        REGOTH_THROW(InvalidParametersException, "Failed to import ZEN-file: " + mZenFile);
      }

      findWaynet();
    }
    else
    {
      // Need to fill in some dummy data on empty worlds
      mWaynet = SO()->addComponent<Waynet>();
    }

    onImportedZEN();

    mGameClock = SO()->addComponent<GameClock>();

    mIsInitialized = true;

    // Always do this after importing or deserializing
    fillFindByNameCache();
  }

  HItem GameWorld::insertItem(const bs::String& instance, const bs::Transform& transform)
  {
    bs::HSceneObject itemSO = bs::SceneObject::create(instance);
    itemSO->setParent(SO());

    itemSO->setPosition(transform.pos());
    itemSO->setRotation(transform.rot());

    return itemSO->addComponent<Item>(instance);
  }

  HItem GameWorld::insertItem(const bs::String& instance, const bs::String& spawnPoint)
  {
    bs::HSceneObject spawnPointSO = findObjectByName(spawnPoint);
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
    HGameWorld thisWorld = bs::static_object_cast<GameWorld>(getHandle());

    bs::HSceneObject characterSO = bs::SceneObject::create(instance);
    characterSO->setParent(SO());

    characterSO->setPosition(transform.pos());
    characterSO->setRotation(transform.rot());

    characterSO->addComponent<VisualCharacter>();

    auto controller = characterSO->addComponent<bs::CCharacterController>();

    // FIXME: Assign the radius and height set via the visuals bounding box
    controller->setRadius(0.35f);
    controller->setHeight(0.5f);

    auto ai = characterSO->addComponent<CharacterAI>(thisWorld);

    // All script-inserted characters will be disabled right after inserting them so they
    // don't cause the game to slow down. If they are all active, physics will be calculated
    // even for those out of reach which takes a huge hit on performance.
    // The character-controller will be enabled by the AI or user input.
    // ai->deactivatePhysics(); // Commented out for testing

    auto character = characterSO->addComponent<Character>(instance);

    // Must be added after the Character and CharacterAI components
    auto eventQueue = characterSO->addComponent<CharacterEventQueue>(thisWorld);

    // If we don't init the routine now, the character won't have its default routine
    eventQueue->reinitRoutine();

    return character;
  }

  HCharacter GameWorld::insertCharacter(const bs::String& instance, const bs::String& spawnPoint)
  {
    bs::HSceneObject spawnPointSO = findObjectByName(spawnPoint);
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

    bs::gDebug().logDebug("[GameWorld] Insert Character " + instance + " at " + spawnPoint);

    return insertCharacter(instance, transform);
  }

  void GameWorld::initScriptVM()
  {
    bs::Vector<bs::UINT8> data = gVirtualFileSystem().readFile("GOTHIC.DAT");

    mScriptVM = bs::bs_shared_ptr_new<Scripting::ScriptVMForGameWorld>(
        bs::static_object_cast<GameWorld>(getHandle()), data);

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
    auto scriptObject = mScriptVM->heroInstance();

    if (scriptObject == Scripting::SCRIPT_OBJECT_HANDLE_INVALID) return {};

    bs::HSceneObject heroSO = mScriptVM->mapping().getMappedSceneObject(scriptObject);

    return heroSO->getComponent<Character>();
  }

  bs::HSceneObject GameWorld::findObjectByName(const bs::String& name)
  {
    auto it = mSceneObjectsByNameCached.find(name);

    if (it != mSceneObjectsByNameCached.end())
    {
      // Found it in cache!
      if (!it->second.isDestroyed())
      {
        return it->second;
      }
      else
      {
        // If the object was destroyed, act like the object wasn't found, the cache might be outdated
        // and there is such an object now.
      }
    }

    bs::HSceneObject so = SO()->findChild(name);

    if (so)
    {
      mSceneObjectsByNameCached[name] = so;
    }

    return so;
  }

  void GameWorld::fillFindByNameCache()
  {
    mSceneObjectsByNameCached.clear();

    std::function<void(bs::HSceneObject)> visit = [&](bs::HSceneObject parent) {
      for (bs::UINT32 i = 0; i < parent->getNumChildren(); i++)
      {
        bs::HSceneObject child = parent->getChild(i);

        bs::String name = child->getName();

        if (!name.empty())
        {
          mSceneObjectsByNameCached[name] = child;
        }

        visit(child);
      }
    };
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
