#pragma once

#include <memory>

#include <core/Engine.hpp>
#include <core/EngineConfig.hpp>

namespace REGoth
{
  /**
   * @brief A concrete, yet empty REGoth engine which uses the default configuration object
   * `EngineConfig`.
   *
   * This is a convenience class which can be extended and used if the default configuration object
   * `EngineConfig` is sufficient, and no custom configuration is needed.
   *
   * If you wish to use a custom configuration, however, `Engine` should be extended, and
   * `Engine::config()` overridden to avoid confusion between different configuration members.
   *
   * @ingroup core
   * @headerfile core/EmptyGame.hpp
   */
  class EmptyGame : public Engine
  {
  public:
    /**
     * Constructs an engine object given a configuration.
     *
     * @param config `Engine` configuration.
     */
    EmptyGame(std::unique_ptr<const EngineConfig>&& config);

    /**
     * Gets the engine's configuration.
     *
     * @return `Engine` configuration data structure.
     */
    const EngineConfig* config() const final override;

  private:
    /**
     * Engine base configuration.
     */
    std::unique_ptr<const EngineConfig> mConfig;
  };
}  // namespace REGoth
