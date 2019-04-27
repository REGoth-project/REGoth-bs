#pragma once
#include <BsPrerequisites.h>
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class Item;
  using HItem = bs::GameObjectHandle<Item>;

  class Waynet;
  using HWaynet = bs::GameObjectHandle<Waynet>;

  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  namespace Scripting
  {
    class ScriptVMForGameWorld;
  }

  /**
   * Component to be attached to the scene root which manages the game world.
   *
   * This is the place where ZEN-files are imported, Characters placed into the
   * world and where the worlds script engine is at home.
   *
   *
   * Loading and Saving Worlds
   * =========================
   *
   * When a world shall be loaded, the GameWorld component needs to be created
   * and attached to a Scene Object, which we call *Root SO*. By manually adding
   * the GameWorld component via `addComponent()`, you can specify the name of a
   * original ZEN-file. This ZEN-file will be parsed and imported into the
   * scene, where all created scene objects will be a (direct or indirect) child
   * of the *Root SO*.
   *
   * Once a world is imported it will function as a clean starting point. When
   * running the game, at that point you would want do run the script init
   * routine, but you don't have to. What happens after the import is done can
   * be controlled by overriding the `onImportedZEN()` method.
   *
   * At any time after the world has been created, it can be saved to a file,
   * similar to a save-game in the original game. This save game will store the
   * complete state of the world, including script state.
   *
   * Example to create an empty world:
   *
   *    HGameWorld gameWorld = GameWorld::createEmpty();
   *
   *
   * Example to import a ZEN:
   *
   *    HGameWorld gameWorld = GameWorld::importZEN("OLDWORLD.ZEN");
   *
   *
   * Example to save a World:
   *
   *    gameWorld->save("MySavegame");
   *
   *
   * Example to load a saved World:
   *
   *    bs::HPrefab prefab = GameWorld::load("MySavegame");
   *    prefab->instantiate();
   *
   *
   * World Script Engine
   * ===================
   *
   * In Gothic there is only one Script-VM which can be fed a generic program
   * file once instantiated. In REGoth, we have one more layer of abstraction
   * around it: For each kind of program file, there has to be a matching
   * interfacing script VM. For example, the script program used for most
   * gameplay related actions is called `GOTHIC.DAT`. This one needs completely
   * different externals and interface-functions than the one for the menus.
   * Therefore, the GameWorld component also instantiates the correct Script VM
   * for executing world related script code, see `ScriptVMForGameWorld`.
   *
   *
   * Script Externals
   * ================
   *
   * This component is also the place where world related script externals are
   * implemented, such as `WLD_CreateNPC()`. They might be named differently to
   * make them fit more with the other naming conventions, but you'll quickly
   * find out what is what. The externals themselves are to be implemented
   * without too many dependencies to the script VM, so it's better to use real
   * scene object or component handles as parameters rather than script objects.
   * This improves reusability of those script externals.
   */
  class GameWorld : public bs::Component
  {
  public:

    enum Empty
    {
      EmptyWorld,
    };

    /**
     * Imports a world from ZEN. See importZEN().
     */
    GameWorld(const bs::HSceneObject& parent, const bs::String& zenFile);
    GameWorld(const bs::HSceneObject& parent, Empty empty);

    virtual ~GameWorld();

    /**
     * Called when a ZEN-file has been successfully imported.
     * This is where further initializations can take place, such as
     * running the worlds script init routine.
     */
    virtual void onImportedZEN()
    {
    }

    /**
     * Returns the currently loaded worlds name.
     *
     * The world name is parsed from the ZEN-files name. So, a world
     * loaded from `NEWWORLD.ZEN` will have a worldname of `NEWWORLD`.
     *
     * @return  The currently loaded worlds name.
     */
    bs::String worldName() const;

    /**
     * @return  Handle to the Waynet of the world.
     */
    HWaynet waynet() const
    {
      return mWaynet;
    }

    /**
     * Access to the worlds ScriptVM with GOTHIC.DAT loaded.
     */
    Scripting::ScriptVMForGameWorld& scriptVM()
    {
      return *mScriptVM;
    }

    /**
     * Constructs a GameWorld-Component from an original ZEN-file.
     *
     * The given ZEN-file will be imported into the scene, which may take
     * a while. It will try to import all objects stored there including
     * Items, which need script constructors to run. However, it will
     * not run the worlds init-script routine as that could be unwanted
     * for some samples or tools. To run the init-script routine,
     * the virtual method `onImportedZEN()` should be overridden.
     *
     * Since importing a ZEN can take a while, you can `save()` the
     * world afterwards and load from the save, which is much quicker.
     *
     * @return Handle to the imported GameWorld.
     */
    static HGameWorld importZEN(const bs::String& zenFile);

    /**
     * Creates an empty world.
     */
    static HGameWorld createEmpty();

    /**
     * Saves the current world and everything that goes with it to a
     * savegame with the given name.
     */
    void save(const bs::String& saveName);

    /**
     * Loads the world with the given name previously saved via save().
     *
     * Throws if the save does not exist.
     *
     * @note   This is static since it makes no sense to create an empty
     *         world first since the prefab will already contain a properly
     *         set up scene object anyways.
     *
     * @return Prefab of the saved world.
     */
    static bs::HPrefab load(const bs::String& saveName);

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
     * Inserts a character at the given waypoint.
     *
     * Throws if instance or waypoint does not exist.
     *
     * @param  instance   Script instanc og the character, e.g. `PC_HERO`.
     * @param  transform  Where the character should be inserted into the world.
     *
     * @return Handle of the character.
     */
    HCharacter insertCharacter(const bs::String& instance, const bs::Transform& transform);

  protected:
    void onInitialized() override;

  private:
    /**
     * Initializes the worlds script VM with GOTHIC.DAT.
     */
    void initScriptVM();

    /**
     * Goes through the Children of this SO and looks for a Waynet-component.
     * When it found one, it stores the handle to it in `mWaynet`.
     * Throws, if no Waynet can be found.
     */
    void findWaynet();

    /**
     * ZEN-File this world was created from, e.g. `NEWWORLD.ZEN`.
     */
    bs::String mZenFile;

    /**
     * Access to the Waynet of this world.
     */
    HWaynet mWaynet;

    /**
     * Script-VM with GOTHIC.DAT loaded.
     */
    bs::SPtr<Scripting::ScriptVMForGameWorld> mScriptVM;

    /**
     * Used to skip onInitialized() when loading via RTTI.
     */
    bool mIsInitialized = false;

  public:
    REGOTH_DECLARE_RTTI(GameWorld)

  public:                   // FIXME: RTTI, make protected
    GameWorld() = default;  // Serialization only
  };

}  // namespace REGoth
