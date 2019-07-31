/** \file
 * Entry point for REGoth
 */

#include "REGothEngine.hpp"

int main(int argc, char** argv)
{
  std::unique_ptr<const REGoth::EngineConfig> config
      = REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
  REGoth::REGothEngine engine{std::move(config)};

  return REGoth::runEngine(engine);
}
