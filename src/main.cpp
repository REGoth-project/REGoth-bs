#include <memory>

#include <core.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>

using namespace REGoth;

int main(int argc, char** argv)
{
  auto config         = parseArguments<EngineConfig>(argc, argv);
  const GameType game = config->gameType;
  std::unique_ptr<Engine> engine;

  if (game == GameType::Gothic1)
  {
    REGOTH_LOG(Info, Uncategorized, "Booting Gothic...");
    engine = std::make_unique<Gothic1Game>(std::move(config));
  }
  else if (game == GameType::Gothic2)
  {
    REGOTH_LOG(Info, Uncategorized, "Booting Gothic II...");
    engine = std::make_unique<Gothic2Game>(std::move(config));
  }
  else
  {
    REGOTH_THROW(InvalidStateException, "Cannot identify Gothic or Gothic II installation.");
  }

  return runEngine(*engine.get());
}
