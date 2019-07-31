#include <core/EmptyEngine.hpp>

#include <memory>

#include <core/EngineConfig.hpp>

using namespace REGoth;

EmptyEngine::EmptyEngine(std::unique_ptr<const EngineConfig>&& config)
    : mConfig{std::move(config)}
{
  // pass
}

const EngineConfig* EmptyEngine::config() const
{
  return mConfig.get();
}
