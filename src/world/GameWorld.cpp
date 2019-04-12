#include "GameWorld.hpp"
#include <components/Character.hpp>
#include "components/Waypoint.hpp"
#include "internals/ConstructFromZEN.hpp"
#include <Scene/BsSceneObject.h>
#include <components/Item.hpp>
#include <components/Waynet.hpp>
#include <excepction/Throw.hpp>
#include <scripting/ScriptVMInterface.hpp>

namespace REGoth
{
  namespace World
  {
    class GameWorldInternal
    {
    public:
      GameWorldInternal()
      {
      }

      bool loadFromZEN(const bs::String& zenFile)
      {
        assignWorldNameFromZEN(zenFile);

        mSceneRoot = constructFromZEN(zenFile);

        if (!mSceneRoot) return false;

        findWaynet();

        return true;
      }

      void assignWorldNameFromZEN(const bs::String& zenFile)
      {
        mWorldName = zenFile.substr(0, zenFile.find_first_of('.'));
      }

      void findWaynet()
      {
        bs::HSceneObject waynetSO = gWorld().sceneRoot()->findChild("Waynet");

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

      bs::String mWorldName;
      bs::HSceneObject mSceneRoot;
      HWaynet mWaynet;
    };

    bool GameWorld::_loadZen(const bs::String& zenFile, Init init)
    {
      mInternal = bs::bs_shared_ptr_new<GameWorldInternal>();

      bool hasLoaded = mInternal->loadFromZEN(zenFile);

      if (!hasLoaded) return false;

      if (init != Init::NoInitScripts)
      {
        gGameScript().initializeWorld(worldName());
      }

      return true;
    }

    const bs::String& GameWorld::worldName()
    {
      return mInternal->mWorldName;
    }

    bs::HSceneObject GameWorld::sceneRoot()
    {
      return mInternal->mSceneRoot;
    }

    HWaynet GameWorld::waynet()
    {
      return mInternal->mWaynet;
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
      // TODO: Freepoints - just dump them at world center for now
      if (bs::StringUtil::startsWith(spawnPoint, "fp_"))
      {
        return insertItem(instance, bs::Transform::IDENTITY);
      }

      HWaypoint waypoint = waynet()->findWaypoint(spawnPoint);

      if (!waypoint)
      {
        REGOTH_THROW(
            InvalidParametersException,
            bs::StringUtil::format("Spawnpoint {0} for item of instance {1} does not exist!",
                                   spawnPoint, instance));
      }

      return insertItem(instance, waypoint->SO()->getTransform());
    }

    HCharacter GameWorld::insertCharacter(const bs::String& instance, const bs::String& waypoint)
    {
      bs::HSceneObject characterSO = bs::SceneObject::create(instance);

      HWaypoint waypointHandle = waynet()->findWaypoint(waypoint);

      if (!waypointHandle)
      {
        REGOTH_THROW(
            InvalidParametersException,
            bs::StringUtil::format("Spawnpoint {0} for character of instance {1} does not exist!",
                                   waypoint, instance));
      }

      const bs::Transform& transform = waypointHandle->SO()->getTransform();

      characterSO->setPosition(transform.pos());
      characterSO->setRotation(transform.rot());

      return characterSO->addComponent<Character>(instance);
    }

    static bs::SPtr<World::GameWorld> s_World;

    bool loadWorldFromZEN(const bs::String& zenFile, GameWorld::Init init)
    {
      s_World = bs::bs_shared_ptr_new<World::GameWorld>();
      s_World->_loadZen(zenFile, init);
      return true;
    }
  }  // namespace World

  World::GameWorld& gWorld()
  {
    if (!World::s_World)
    {
      REGOTH_THROW(InvalidStateException, "No world has been loaded yet!");
    }

    return *World::s_World;
  }
}  // namespace REGoth
