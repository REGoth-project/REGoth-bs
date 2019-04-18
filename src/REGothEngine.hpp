/** \file
 */

#pragma once

#include <BsPrerequisites.h>

namespace REGoth
{
  /**
   * This is the REGoth-Core-Class, which initializes the engine, sets the
   * input and the scene.
   *
   * To handle more use cases, the REGothEngine-class can be extended.
   * The base class will not load any world and start on an empty scene but with
   * most of the utilities set up to load original content and game mechanics.
   *
   * Therefore, the REGothEngine-class can be used to implement viewers and other tools
   * as well as the actual reimplementation of the Gothic games.
   *
   *
   * Some important virtual functions exist which can be overridden:
   *
   *  - getVdfsPackagesToLoad()
   *  - setupInput()
   *  - setupScene()
   *
   * To actually run an instance of the engine, see the `main`-wrapper below.
   */
  class REGothEngine
  {
  public:
    REGothEngine() = default;
    virtual ~REGothEngine();

    /**
     * Construct a list of VDFS-packages to load here.
     *
     * By default, all packages within the games Data-Directory are loaded.
     *
     * @param  dataDirectory  Path of the Data-Directory of the game files (Where the .VDF-files
     * are).
     * @return List of VDFS-Packages to be loaded by the engine
     */
    virtual bs::Vector<bs::Path> getVdfsPackagesToLoad(const bs::Path& dataDirectory);

    /**
     * Load VDFS packages from the original game.
     *
     * @param          argv0  First value of the argv array passed to main (Path to executable).
     * @param  gameDirectory  Location where Gothics game files can be found.
     */
    void loadOriginalGamePackages(const bs::String& argv0, const bs::Path& gameDirectory);

    /**
     * When called after loadOriginalGamePackages(), this will check whether Gothics game files were
     * found at the location given to loadOriginalGamePackages().
     *
     * @return Whether game files were found.
     */
    bool hasFoundGameFiles();

    /**
     * Initializes bsf and opens the window
     */
    void initializeBsf();

    /**
     * Load all resource manifests written by previous runs of REGoth.
     */
    void loadCachedResourceManifests();

    /**
     * Saves all resources currently registered in the resource manager to a manifest.
     */
    void saveCachedResourceManifests();

    /**
     * Assign buttons and axis to control the game
     */
    virtual void setupInput();

    /**
     * Sets up the main camera of this engine.
     */
    virtual void setupMainCamera();

    /**
     * Load scenes and other objects and add them to the scene
     */
    virtual void setupScene();

    /**
     * Run the main-loop
     */
    void run();

    /**
     * Shutdown bsf
     */
    void shutdown();

  protected:
    /**
     * Main camera this engines renders with
     */
    bs::HCamera mMainCamera;
  };

  /**
   * Boilerplate-code to setup and run the given engine
   *
   * @param  regoth  Untouched fresh instance of the engine class you want to run
   * @param    argc  `argc` as in `main`
   * @param    argv  `argv` as in `main`
   *
   * @return Return value as in `main`.
   */
  int main(REGothEngine& regoth, int argc, char** argv);
}  // namespace REGoth
