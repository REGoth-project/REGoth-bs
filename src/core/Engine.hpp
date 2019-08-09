#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <cxxopts.hpp>

#include <core/EngineConfig.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameFiles.hpp>

namespace REGoth
{
  class EngineContent;
  class OriginalGameFiles;

  /**
   * @brief The core class of REGoth, which offers a lot of default implementations to initialize
   * the engine, setup the input, the scene, and others.
   *
   * To handle more use cases, and if a custom `EngineConfig` is desired `Engine` can be extended
   * (otherwise, see `EmptyGame`).  The base implementations provided here will not load
   * any world and start on an empty scene, but with most of the utilities set up to load original
   * content and game mechanics.
   *
   * Therefore, the `Engine` can be used to implement viewers and other tools, as well as the
   * actual reimplementations of the Gothic games.
   *
   * Some important virtual functions exist which can be overridden:
   *
   * - `setupInput()`
   *
   * Some pure virtual functions must be implemented:
   *
   * - `setupScene()`: Set up the scene this engine should handle.
   * - `config()`: Return the engine's configuration. To use a default implementation, see
   *   `EmptyGame`.
   *
   * To actually run an instance of the engine, see `runEngine()`.
   *
   * @ingroup core
   * @headerfile core/Engine.hpp
   */
  class Engine
  {
  public:
    virtual ~Engine();

    /**
     * Load VDFS packages from the original game. Will load data in the following order:
     *
     *  1. `Data/` (`*.vdf`)
     *  2. `_world/` (Recursive)
     *  3. `Data/modvdf/` (`*.mod`, recursive)
     */
    void loadGamePackages();

    /**
     * Called by `loadOriginalGamePackages()`. Can be overriden by the user to load specific
     * MOD-packages.
     *
     * To load a MOD-package, use `gVirtualFileSystem().loadPackage(p)`.
     *
     * @param  files  Reference to `OriginalGameFiles` object, which has some utility methods to
     *                access files in the original game directory.
     */
    virtual void loadModPackages(const OriginalGameFiles& files);

    /**
     * When called after `loadOriginalGamePackages()`, this will check whether Gothics game files
     * were found at the location given to `loadOriginalGamePackages()`.
     *
     * @return Whether game files were found.
     */
    bool hasFoundGameFiles();

    /**
     * Initializes `bsf` and opens the window.
     */
    void initializeBsf();

    /**
     * Load all resource manifests written by previous runs of REGoth.
     */
    void loadCachedResourceManifests();

    /**
     * Goes through the loaded VDFS packages and imports the original games resources
     * which were not already cached.
     */
    void populateResourceCache();

    /**
     * Save resource manifests containing resources loaded during this run.
     */
    void saveCachedResourceManifests();

    /**
     * Assign buttons and axis to control the game.
     */
    virtual void setupInput();

    /**
     * Sets up the main camera of this engine.
     */
    virtual void setupMainCamera();

    /**
     * Load scenes and other objects and add them to the scene.
     */
    virtual void setupScene() = 0;

    /**
     * Set shaders to be used when re-caching the materials.
     */
    virtual void setShaders();

    /**
     * Find the location of REGoths own `content` directory.
     */
    void findEngineContent();

    /**
     * Run the main loop.
     */
    void run();

    /**
     * Shutdown `bsf`.
     */
    void shutdown();

    /**
     * Grants access to the engine's configuration.
     *
     * If you do not need a custom engine configuration, and `EngineConfig` is enough, consider
     * extending `EmptyGame` instead of `Engine`, which features a default implementation using
     * `EngineConfig`.
     *
     * @return The engine configuration data structure.
     */
    virtual const EngineConfig* config() const = 0;

  protected:
    /**
     * Main camera this engines renders with.
     */
    bs::HCamera mMainCamera;

    /**
     * Path to REGoth's own `content` directory and resource loader.
     */
    bs::SPtr<EngineContent> mEngineContent;
  };
}  // namespace REGoth
