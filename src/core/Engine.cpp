#include <core/Engine.hpp>

#include <memory>

#include <core/EngineConfig.hpp>

using namespace REGoth;

Engine::Engine(std::unique_ptr<const EngineConfig>&& config)
    : mConfig{std::move(config)}
{
  // pass
}

const EngineConfig* Engine::config() const
{
  return mConfig.get();
}
