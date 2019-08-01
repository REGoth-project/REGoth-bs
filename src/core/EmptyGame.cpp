#include <core/EmptyGame.hpp>

#include <memory>

#include <core/EngineConfig.hpp>

using namespace REGoth;

EmptyGame::EmptyGame(std::unique_ptr<const EngineConfig>&& config)
    : mConfig{std::move(config)}
{
  // pass
}

const EngineConfig* EmptyGame::config() const
{
  return mConfig.get();
}
