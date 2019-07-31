#pragma once

#include <memory>

#include <core/AbstractEngine.hpp>
#include <core/EngineConfig.hpp>

namespace REGoth
{
  /**
   * @brief A concrete REGoth engine which uses the default configuration object EngineConfig.
   *
   * This is a convenience class which can be extended and used if the default configuration object
   * EngineConfig is sufficient, and no custom configuration is needed.
   *
   * If you wish to use a custom configuration, however, AbstractEngine should be extended, and
   * config() overridden to avoid confusion between different configuration members.
   *
   * @ingroup core
   * @headerfile core/Engine.hpp
   */
  class Engine : public AbstractEngine
  {
  public:
    /**
     * Constructs an engine object given a configuration.
     *
     * @param config Engine configuration.
     */
    Engine(std::unique_ptr<const EngineConfig>&& config);

    /**
     * Gets the engine's configuration.
     *
     * @return Engine configuration data structure.
     */
    const EngineConfig* config() const final override;

  private:
    /**
     * Engine base configuration.
     */
    std::unique_ptr<const EngineConfig> mConfig;
  };
}  // namespace REGoth
