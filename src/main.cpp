/** \file
 * Entry point for REGoth
 */

#include <core/Engine.hpp>
#include <core/EngineConfig.hpp>

int main(int argc, char** argv)
{
  std::unique_ptr<const REGoth::EngineConfig> config
      = REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
  REGoth::Engine engine{std::move(config)};

  return REGoth::runEngine(engine);
}
