#pragma once

#include <FileSystem/BsPath.h>

#include <cxxopts.hpp>

namespace bs
{
  /**
   * Allows using the `bs::Path` data type together with `cxxopts`.
   *
   * @param str Input stringstream.
   * @param path Path to write data to.
   * @return The original stringstream.
   */
  std::stringstream& operator>>(std::stringstream& str, bs::Path& path);
}  // namespace bs

namespace REGoth
{
  /**
   * @brief The base configuration for an `Engine`.
   *
   * Configuration objects are required to construct an engine.  An `EngineConfig` is the default
   * configuration object, which is used in the definition of `Engine::config()`, and which
   * is also the configuration object for the default engine implementation `EmptyGame`.
   *
   * If a specialised `EngineConfig` is needed, `Engine` must be extended and
   * `Engine::config()` overridden accordingly.
   *
   * @ingroup core
   * @headerfile core/EngineConfig.hpp
   */
  struct EngineConfig
  {
    /**
     * Frees acquired resources.
     */
    virtual ~EngineConfig();

    /**
     * Registers CLI engine options via the given `cxxopts` options parameter.
     *
     * @param options A `cxxopts` options configurations object.
     */
    void registerCLIEngineOptions(cxxopts::Options& options);

    /**
     * Verifies CLI engine options after parsing.
     */
    void verifyCLIEngineOptions();

    /**
     * Registers CLI options via the given `cxxopts` options parameter.
     *
     * @param options A `cxxopts` options configurations object.
     */
    virtual void registerCLIOptions(cxxopts::Options& /* options */);

    /**
     * Verifies CLI options after parsing.
     */
    virtual void verifyCLIOptions();

    /**
     * Verbosity level. 0 = quiet. The higher, the more verbose.
     */
    unsigned int verbosity = 0;

    /**
     * Path to the executable of REGoth.
     */
    bs::Path engineExecutablePath;

    /**
     * Path to the game assets of Gothic or Gothic II.
     */
    bs::Path originalAssetsPath;

    /**
     * X resolution of the game.
     */
    unsigned int resolutionX = 1280;

    /**
     * Y resolution of the game.
     */
    unsigned int resolutionY = 720;

    /**
     * Whether the game should be run in fullscreen.
     */
    bool isFullscreen = false;
  };
}  // namespace REGoth
