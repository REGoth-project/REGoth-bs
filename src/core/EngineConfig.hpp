#pragma once

#include <BsPrerequisites.h>
#include <FileSystem/BsPath.h>

#include <core/GameType.hpp>

#include <cxxopts.hpp>

#include <components/Sky.hpp>

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
   * Allows using the `REGoth::Sky::RenderMode` data type together with `cxxopts`.
   *
   * @param str Input stringstream.
   * @param path Path to write data to.
   * @return The original stringstream.
   */
  std::stringstream& operator>>(std::stringstream& str, REGoth::Sky::RenderMode& renderMode);

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
     * The current game type (i.e. Gothic vs Gothic II).
     */
    GameType gameType = GameType::Unidentified;

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

    /**
     * The sky render mode of the game.
     */
    Sky::RenderMode skyRenderMode = Sky::RenderMode::Plane;
  };
}  // namespace REGoth
