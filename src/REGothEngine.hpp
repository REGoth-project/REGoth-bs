/** \file
 */

#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <BsPrerequisites.h>
#include <FileSystem/BsFileSystem.h>

#include <cxxopts.hpp>

#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameFiles.hpp>

/**
 * Allows using the bs::Path data type together with cxxopts.
 *
 * @param str Input stringstream.
 * @param path Path to write data to.
 * @return stringstream.
 */
std::stringstream& operator>>(std::stringstream& str, bs::Path& path);

namespace REGoth
{
  class EngineContent;
  class OriginalGameFiles;

  /**
   * The base configuration of the engine.
   */
  struct EngineConfig
  {
    virtual ~EngineConfig()
    {
      // pass
    }

    void registerCLIEngineOptions(cxxopts::Options& options)
    {
      // Configure positional handling
      options.positional_help("[GAME ASSETS PATH]");
      options.show_positional_help();

      const std::string grp = "Core engine";

      // Define engine options
      options.add_option(grp, "g", "game-assets", "Path to a Gothic or Gothic 2 installation",
                         cxxopts::value<bs::Path>(originalAssetsPath), "[PATH]");
      options.add_option(grp, "", "video-x-res", "X resolution",
                         cxxopts::value<unsigned int>(resolutionX), "[PX]");
      options.add_option(grp, "", "video-y-res", "Y resolution",
                         cxxopts::value<unsigned int>(resolutionY), "[PX]");
      options.add_option(grp, "", "video-fullscreen", "Run in fullscreen mode",
                         cxxopts::value<bool>(isFullscreen), "");

      // Allow game-assets to also be a positional
      options.parse_positional({"game-assets"});
    }

    void verifyCLIEngineOptions()
    {
      // Assert that engineExecutablePath is not empty.
      if (engineExecutablePath.isEmpty())
      {
        REGOTH_THROW(InvalidStateException, "Engine executable path could not be determined.");
      }

      // Resolve engineExecutablePath.
      engineExecutablePath.makeAbsolute(bs::FileSystem::getWorkingDirectoryPath());

      // If originalAssetsPath is unset, try to derive possible fallbacks.
      if (originalAssetsPath.isEmpty())
      {
        REGOTH_LOG(Info, Uncategorized, "No game asset path given. Trying executable path...");

        // Use engineExecutablePath as first choice.
        originalAssetsPath = OriginalGameFiles::findGameFilesRoot(engineExecutablePath);

        // If originalAssetsPath is still unset, try current working directory.
        if (originalAssetsPath.isEmpty() &&
            engineExecutablePath != bs::FileSystem::getWorkingDirectoryPath())
        {
          REGOTH_LOG(Info, Uncategorized, "Failed. Trying current working directory...");

          originalAssetsPath =
              OriginalGameFiles::findGameFilesRoot(bs::FileSystem::getWorkingDirectoryPath());

          if (originalAssetsPath.isEmpty())
          {
            REGOTH_LOG(Info, Uncategorized, "Failed. Giving up.");
            REGOTH_THROW(
                InvalidStateException,
                "Could not find a game asset folder. Try specifying it with `--game-assets`.");
          }
        }

        REGOTH_LOG(Info, Uncategorized, "Succeded");
      }
      // Otherwise verify and resolve user-supplied input.
      else
      {
        bs::String userInput = originalAssetsPath.toString();

        // Probe user-supplied directory.
        originalAssetsPath = OriginalGameFiles::findGameFilesRoot(originalAssetsPath);

        if (originalAssetsPath.isEmpty())
        {
          REGOTH_THROW(InvalidStateException,
                       "Could not find a Gothic or Gothic 2 installation at the supplied path `" +
                           userInput + "`.");
        }
      }
    }

    virtual void registerCLIOptions(cxxopts::Options& /* options */)
    {
      // pass
    }

    virtual void verifyCLIOptions()
    {
      // pass
    }

    unsigned int verbosity = 0;
    bs::Path engineExecutablePath;
    bs::Path originalAssetsPath;
    unsigned int resolutionX = 1280;
    unsigned int resolutionY = 720;
    bool isFullscreen        = false;
  };

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
    virtual ~REGothEngine();

    /**
     * Load VDFS packages from the original game. Will load data in the following order:
     *
     *  1. Data/ (*.vdf)
     *  2. _world/ (Recursive)
     *  3. Data/modvdf/ (*.mod, recursive)
     */
    void loadGamePackages();

    /**
     * Called by loadOriginalGamePackages(). Can be overriden by the user to load specific
     * MOD-packages.
     *
     * To load a MOD-package, use `gVirtualFileSystem().loadPackage(p)`.
     *
     * @param  files  Reference to OriginalGameFiles-object, which has some utility methods to
     *                access files in the original game directory.
     */
    virtual void loadModPackages(const OriginalGameFiles& files);

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
     * Save resource manifests containing resources loaded during this run.
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
     * Set shaders to be used when re-caching the materials.
     */
    virtual void setShaders();

    /**
     * Find the location of REGoths own `content`-directory.
     */
    void findEngineContent();

    /**
     * Run the main-loop
     */
    void run();

    /**
     * Shutdown bsf
     */
    void shutdown();

    /**
     * Gets the engine's configuration.
     *
     * @return Engine configuration data structure.
     */
    virtual const EngineConfig* config() const = 0;

  protected:
    /**
     * Main camera this engines renders with
     */
    bs::HCamera mMainCamera;

    /**
     * Path to REGoth's own `content`-directory and resource loader.
     */
    bs::SPtr<EngineContent> mEngineContent;
  };

  /**
   * A concrete `REGothEngine` which uses the default configuration.
   */
  class REGothEngineDefaultConfig : public REGothEngine
  {
  public:
    /**
     * Constructs an engine object given a configuration.
     * @param config Engine configuration.
     */
    REGothEngineDefaultConfig(std::unique_ptr<const EngineConfig>&& config);

    /**
     * Gets the engine's configuration.
     *
     * @return Engine configuration data structure.
     */
    const EngineConfig* config() const override;

  private:
    /**
     * Engine base configuration.
     */
    std::unique_ptr<const EngineConfig> mConfig;
  };

  /**
   * Parses the given command line arguments in `argv` to populate the given configuration object
   * config.
   *
   * @param argc Main's `argc`.
   * @param argv Main's `argv`.
   * @param config Output parameter to populate with parsed options.
   */
  template <class T>
  std::unique_ptr<const T> parseArguments(int argc, char** argv)
  {
    static_assert(std::is_base_of<EngineConfig, T>(),
                  "Template class must have "
                  "`REGoth::EngineConfig` as base class");

    std::unique_ptr<T> config = std::make_unique<T>();

    // Some utility functions involving bs::Path fail if this was not called prior.
    bs::MemStack::beginThread();

    bool help;
    bool version;

    cxxopts::Options options{argv[0], "REGoth - zEngine Reimplementation."};

    // Add general options.
    const std::string grp = "General";
    options.add_option(grp, "h", "help", "Print this help message", cxxopts::value<bool>(help), "");
    options.add_option(grp, "", "version", "Print the REGoth version", cxxopts::value<bool>(version),
                       "");
    options.add_option(grp, "v", "verbosity", "Verbosity level", cxxopts::value<bool>(), "");

    // Add options (engine options and specialised ones).
    config->registerCLIEngineOptions(options);
    config->registerCLIOptions(options);

    // Parse argv.
    cxxopts::ParseResult result = options.parse(argc, argv);

    // Print help if `-h` or `--help` is passed and exit.
    if (help)
    {
      std::cout << options.help() << std::endl;
      std::exit(EXIT_SUCCESS);
    }

    // Print REGoth version if `--version` is passed and exit.
    if (version)
    {
      std::cout << "Not yet implemented" << std::endl;
      std::exit(EXIT_SUCCESS);
    }

    // Set verbosity level.
    config->verbosity = static_cast<unsigned int>(result.count("verbosity"));

    // Game executable path must be set manually here.
    config->engineExecutablePath = bs::Path{argv[0]};

    // Verify configuration.
    config->verifyCLIEngineOptions();
    config->verifyCLIOptions();

    // Clean up.
    bs::MemStack::endThread();

    return config;
  }

  /**
   * Bootstrap and run the given engine.
   *
   * @param Engine object to run.
   * @return Windows errorlevel / POSIX status code.
   */
  int runEngine(REGothEngine& engine);

}  // namespace REGoth
