#include "GameWorld.hpp"

#include <BsZenLib/ImportPath.hpp>
#include <daedalus/DATFile.h>

#include <Resources/BsResources.h>
#include <Scene/BsPrefab.h>
#include <Scene/BsSceneManager.h>
#include <Threading/BsTaskScheduler.h>

#include <components/Character.hpp>
#include <components/Focusable.hpp>
#include <components/GameClock.hpp>
#include <components/Item.hpp>
#include <components/VisualCharacter.hpp>
#include <components/Waynet.hpp>

#include <RTTI/RTTI_GameWorld.hpp>
#include <exception/Assert.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/VirtualFileSystem.hpp>
#include <scripting/ScriptVMForGameWorld.hpp>

namespace REGoth
{
  GameWorld::GameWorld(const bs::HSceneObject& parent, const bs::String& zenFile)
      : bs::Component(parent)
      , mZenFile(zenFile)
  {
    setName("GameWorld");
  }

  GameWorld::GameWorld(const bs::HSceneObject& parent, Empty /* empty */)
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
    HGameWorld thisWorld = bs::static_object_cast<GameWorld>(getHandle());

    // Always do this after importing or deserializing
    fillFindByNameCache();

    // FIXME: Enable these again if BsSceneManager::findComponents works at this point.
    //        It seems to be too early for the components to be found when deserializing the world...
    //        At the moment, these lists are stored inside the save game, which is not optimal.
    // findAllCharacters();
    // findAllItems();
    // findAllFocusables();

    // If this is true here, we're being de-serialized
    if (mIsInitialized) return;

    initScriptVM();

    if (!mZenFile.empty())
    {
      // Import the ZEN and add all scene objects as children to this SO.
      bs::HSceneObject so = mZenImporter.constructFromZEN(thisWorld, mZenFile);

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
    mGameClock->setTime(8, 0);

    mIsInitialized = true;
  }

  void GameWorld::findAllCharacters()
  {
    mAllCharacters = bs::gSceneManager().findComponents<Character>(false);
  }

  void GameWorld::findAllItems()
  {
    mAllItems = bs::gSceneManager().findComponents<Item>(false);
  }

  void GameWorld::findAllFocusables()
  {
    mAllFocusables = bs::gSceneManager().findComponents<Focusable>(false);
  }

  HItem GameWorld::insertItem(const bs::String& instance, const bs::Transform& transform)
  {
    HGameWorld thisWorld = bs::static_object_cast<GameWorld>(getHandle());

    bs::HSceneObject itemSO = bs::SceneObject::create(instance);
    itemSO->setParent(SO());

    itemSO->setPosition(transform.pos());
    itemSO->setRotation(transform.rot());

    auto item = itemSO->addComponent<Item>(instance, thisWorld);

    auto focusable = itemSO->addComponent<Focusable>();

    // TODO: Figure out the correct name to use for the focus text
    focusable->setText(instance);

    mAllItems.push_back(item);

    REGOTH_ASSERT(!!focusable, "Item {0} must be Focusable", itemSO->getName());

    mAllFocusables.push_back(focusable);

    return item;
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

    auto character = characterSO->addComponent<Character>(instance, thisWorld);

    mAllCharacters.push_back(character);

    auto focusable = characterSO->getComponent<Focusable>();
    REGOTH_ASSERT(!!focusable, "Character {0} must be Focusable", characterSO->getName());

    mAllFocusables.push_back(focusable);

    return character;
  }

  //FIXME: deprecate String as spawnpoint, use Startpoint or Waypoint directly
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

    REGOTH_LOG(Info, Uncategorized, "[GameWorld] Insert Character {0} at {1}", instance, spawnPoint);

    return insertCharacter(instance, transform);
  }

  template <typename T>
  static void removeHandleFromVector(T handle, bs::Vector<T>& vector)
  {
    for (auto it = vector.begin(); it != vector.end(); it++)
    {
      if (*it == handle)
      {
        *it = vector.back();
        vector.pop_back();
      }
    }
  }

  void GameWorld::removeItem(HItem item)
  {
    auto focusable = item->SO()->getComponent<Focusable>();

    removeHandleFromVector(item, mAllItems);
    removeHandleFromVector(focusable, mAllFocusables);

    item->SO()->destroy();
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

    visit(SO());
  }

  template <typename T>
  static bs::Vector<GameWorld::FoundInRange<T>> findComponentsInRange(
      const bs::Vector<T>& allComponents, float rangeInMeters, const bs::Vector3& around)
  {
    // Squared distances are faster to compute in the tight loop later as they
    // don't need the expensive sqrt().
    float rangeSq = rangeInMeters * rangeInMeters;

    bs::Vector<GameWorld::FoundInRange<T>> found;

    for (T focusable : allComponents)
    {
      const auto& transform = focusable->SO()->getTransform();

      float distanceSq = transform.pos().squaredDistance(around);

      if (distanceSq < rangeSq)
      {
        GameWorld::FoundInRange<T> result;
        result.distanceSq = distanceSq;
        result.thing      = focusable;

        found.emplace_back(result);
      }
    }

    std::sort(found.begin(), found.end(), [](const auto& left, const auto& right) {
      return left.distanceSq < right.distanceSq;
    });

    return found;
  }

  bs::Vector<GameWorld::FoundInRange<HCharacter>> GameWorld::findCharactersInRange(
      float rangeInMeters, const bs::Vector3& around) const
  {
    return findComponentsInRange(mAllCharacters, rangeInMeters, around);
  }

  bs::Vector<GameWorld::FoundInRange<HItem>> GameWorld::findItemsInRange(
      float rangeInMeters, const bs::Vector3& around) const
  {
    return findComponentsInRange(mAllItems, rangeInMeters, around);
  }

  bs::Vector<GameWorld::FoundInRange<HFocusable>> GameWorld::findFocusablesInRange(
      float rangeInMeters, const bs::Vector3& around) const
  {
    return findComponentsInRange(mAllFocusables, rangeInMeters, around);
  }

  bs::Vector<HWaypoint> GameWorld::findWay(const bs::Vector3& from, const bs::Vector3& to)
  {
    HWaypoint waypointFrom = waynet()->findClosestWaypointTo(from).closest;
    HWaypoint waypointTo   = waynet()->findClosestWaypointTo(to).closest;

    return waynet()->findWay(waypointFrom, waypointTo);
  }

  bs::Vector<HWaypoint> GameWorld::findWay(const bs::String& from, const bs::String& to)
  {
    HWaypoint waypointFrom = waynet()->findWaypoint(from);
    HWaypoint waypointTo   = waynet()->findWaypoint(to);

    if (!waypointFrom)
    {
      auto offWaynetFrom = findObjectByName(from);

      if (!offWaynetFrom) return {};

      const auto& positionFrom = offWaynetFrom->getTransform().pos();

      waypointFrom = waynet()->findClosestWaypointTo(positionFrom).closest;
    }

    if (!waypointTo)
    {
      auto offWaynetTo = findObjectByName(to);

      if (!offWaynetTo) return {};

      const auto& positionTo = offWaynetTo->getTransform().pos();

      waypointTo = waynet()->findClosestWaypointTo(positionTo).closest;
    }

    if (!waypointFrom) return {};
    if (!waypointTo) return {};

    return waynet()->findWay(waypointFrom, waypointTo);
  }

  bs::SPtr<bs::Task> GameWorld::save(const bs::String& saveName)
  {
    bs::HPrefab cached = bs::Prefab::create(SO());

    enum
    {
      Overwrite    = true,
      KeepExisting = false,
    };

    auto saveTask = bs::Task::create("Save:" + saveName, [cached, saveName]() {
      // TODO: Should store at savegame location
      bs::Path path = BsZenLib::GothicPathToCachedWorld(saveName);

      // FIXME: This call can take quite long when running under a debugger
      bs::gResources().save(cached, path, Overwrite);
    });

    bs::TaskScheduler::instance().addTask(saveTask);

    return saveTask;
  }

  bs::HPrefab GameWorld::load(const bs::String& saveName)
  {
    // TODO: Should load at savegame location
    bs::Path path = BsZenLib::GothicPathToCachedWorld(saveName);

    return bs::gResources().load<bs::Prefab>(path);
  }

  REGOTH_DEFINE_RTTI(GameWorld)
}  // namespace REGoth
