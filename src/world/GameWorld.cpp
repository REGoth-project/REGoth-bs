#include "GameWorld.hpp"
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

        return true;
      }

      void assignWorldNameFromZEN(const bs::String& zenFile)
      {
        mWorldName = zenFile.substr(0, zenFile.find_first_of('.'));
      }

      bs::String mWorldName;
      bs::HSceneObject mSceneRoot;
      HWaynet mWaynet;
    };

    GameWorld::GameWorld(const bs::String& zenFile, Init init)
    {
      mInternal = bs::bs_shared_ptr_new<GameWorldInternal>();

      mInternal->loadFromZEN(zenFile);

      if (init != Init::NoInitScripts)
      {
        gGameScript().initializeWorld(worldName());
      }
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

    bs::HSceneObject GameWorld::insertItem(const bs::String& instance,
                                           const bs::Transform& transform)
    {
      bs::HSceneObject itemSO = bs::SceneObject::create(instance);
      itemSO->setPosition(transform.pos());
      itemSO->setRotation(transform.rot());

      itemSO->addComponent<Item>(instance);

      return itemSO;
    }

    static bs::SPtr<World::GameWorld> s_World;

    bool loadWorldFromZEN(const bs::String& zenFile, GameWorld::Init init)
    {
      s_World = bs::bs_shared_ptr_new<World::GameWorld>(zenFile, init);
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
