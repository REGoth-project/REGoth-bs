#pragma once

#include <memory>

#include <components/GameWorld.hpp>
#include <components/ThirdPersonCamera.hpp>
#include <core/EmptyGame.hpp>
#include <log/logging.hpp>

namespace REGoth
{
  class Gothic2Game : public REGoth::EmptyGame
  {
  public:
    using REGoth::EmptyGame::EmptyGame;

    void setupMainCamera() override;

    void setupScene() override;

    void setupGothic2(HGameWorld world);

  private:
    REGoth::HThirdPersonCamera mThirdPersonCamera;
  };
}  // namespace REGoth
