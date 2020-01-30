#include <core/Gothic1Game.hpp>

#include <Components/BsCCamera.h>
#include <Scene/BsPrefab.h>

#include <components/GameplayUI.hpp>
#include <components/DebugUI.hpp>

using namespace REGoth;

void Gothic1Game::setupMainCamera()
{
  Engine::setupMainCamera();;
}

void Gothic1Game::setupScene()
{
  GameplayUI::createGlobal(mMainCamera);
  DebugUI::createGlobal(mMainCamera);
}
