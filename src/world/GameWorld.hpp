#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  class Waynet;
  using HWaynet = bs::GameObjectHandle<Waynet>;

  namespace World
  {
    class GameWorldInternal;
    class GameWorld
    {
    public:
      enum class Init
      {
        Everything,
        NoInitScripts,
      };

      GameWorld(const bs::String& zenFile, Init init);

      /**
       * Returns the currently loaded worlds name.
       *
       * The world name is parsed from the ZEN-files name. So, a world
       * loaded from `NEWWORLD.ZEN` will have a worldname of `NEWWORLD`.
       *
       * @return  The currently loaded worlds name.
       */
      const bs::String& worldName();

      /**
       * @return  The Root-Object of the scene. If invalid, the world was not loaded correctly.
       */
      bs::HSceneObject sceneRoot();

      /**
       * @return  Handle to the Waynet of the world.
       */
      HWaynet waynet();

      /**
       * Inserts an item at the given location.
       *
       * Throws if the instance does not exist.
       *
       * @param  instance   Script instance of the item, e.g. `ITFO_APPLE`.
       * @param  transform  Where the item should be inserted into the world.
       *
       * @return Handle to Scene-Object of the item.
       */
      bs::HSceneObject insertItem(const bs::String& instance, const bs::Transform& transform);

    private:
      bs::SPtr<GameWorldInternal> mInternal;
    };

    /**
     * Loads the game world from the given zenFile from the VDFS.
     * After this succeeded, gWorld() is available.
     *
     * The scripting system is expected to be initialized before calling this.
     *
     * @note    This only handles world loading. Other subsystems like scripting
     *          need to be initialized seperately.
     *
     * @param  zenFile  Name of the Zen-file to load, e.g. `NEWWORLD.ZEN`.
     * @param  init     (Optional) Different kinds of init-styles, e.g. without
     *                  executing the worlds init-script.
     *
     * @return  Whether loading was successful.
     */
    bool loadWorldFromZEN(const bs::String& zenFile, GameWorld::Init init = GameWorld::Init::Everything);

  }  // namespace World

  /**
   * Global access to the current game world. Only valid after a world has been loaded via
   * loadWorldFromZEN().
   */
  World::GameWorld& gWorld();

}  // namespace REGoth
