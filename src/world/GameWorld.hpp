#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  class Waynet;
  using HWaynet = bs::GameObjectHandle<Waynet>;

  class Item;
  using HItem = bs::GameObjectHandle<Item>;

  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  namespace World
  {
    /**
     * This class handles the game world.
     *
     * It can create Items, Characters and other objects, and gives access
     * to certain important scene objects and components like the Waynet.
     *
     * This is also the place where the WLD_*-externals should be implemented.
     *
     *
     * # Loading a world
     *
     * To load a world, the global function `REGoth::World::loadWorldFromZEN()` has to
     * be used. This function will replace any worlds loaded before with a new instance.
     * However, it will not reset the scripting engine, so if this is not the first load,
     * make sure to do that before loading another world.
     *
     * Example:
     *
     *     Daedalus::DATFile dat("path/to/GOTHIC.DAT");
     *
     *     Scripting::loadGothicDAT(dat);
     *
     *     World::loadWorldFromZEN("OLDWORLD.ZEN");
     *
     */
    class GameWorldInternal;
    class GameWorld
    {
    public:
      enum class Init
      {
        Everything,
        NoInitScripts,
      };

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
       * @return Handle to the item.
       */
      HItem insertItem(const bs::String& instance, const bs::Transform& transform);

      /**
       * Inserts an item at the given spawnpoint.
       *
       * A spawnpoint can be either a waypoint or a freepoint.
       *
       * Throws if instance or spawnpoint does not exist.
       *
       * @param  instance    Script instance of the item, e.g. `ITFO_APPLE`.
       * @param  spawnPoint  Where the item should be inserted into the world (Waypoint or
       *                     Freepoint).
       *
       * @return Handle to the item.
       */
      HItem insertItem(const bs::String& instance, const bs::String& spawnPoint);

      /**
       * Inserts a character at the given waypoint.
       *
       * Throws if instance or waypoint does not exist.
       *
       * @param  instance    Script instanc og the character, e.g. `PC_HERO`.
       * @param  spawnPoint  Name of the Scene Object where the character should be inserted at.
       *                     This includes the names of Waypoints, Freepoints and other spots. An
       *                     example for a waypoint would be `OC1`.
       *
       * @return Handle of the character.
       */
      HCharacter insertCharacter(const bs::String& instance, const bs::String& spawnPoint);

      /**
       * Internal method to load the world. Do not call directly.
       */
      bool _loadZen(const bs::String& zenFile, Init init);

      /**
       * Internal method to create an empty world. Do not call directly.
       */
      void _loadEmpty();

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
    bool loadWorldFromZEN(const bs::String& zenFile,
                          GameWorld::Init init = GameWorld::Init::Everything);

    /**
     * Creates an empty game world. After this succeeded, gWorld() is available.
     *
     * It will not do a script-init and will not create a hero-character.
     *
     * The scripting system does not need to be initialized before calling this.
     */
    void loadWorldEmpty();

  }  // namespace World

  /**
   * Global access to the current game world. Only valid after a world has been loaded via
   * loadWorldFromZEN().
   */
  World::GameWorld& gWorld();

}  // namespace REGoth
