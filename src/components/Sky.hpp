#pragma once

#include <Image/BsColor.h>
#include <Scene/BsComponent.h>

#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class SkyColoring;

  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  /**
   * TODO: Documentation of Sky
   */
  class Sky : public bs::Component
  {
  public:
    Sky(const bs::HSceneObject& parent, HGameWorld gameWorld, const bs::Color& skyColor);
    virtual ~Sky() override;

    void onInitialized() override;

    void update() override;

  private:
    void applySkySettingsToCamera() const;

    bs::SPtr<SkyColoring> mSkyColoring;
    HGameWorld mGameWorld;
    const bs::Color mSkyColor;

  public:
    REGOTH_DECLARE_RTTI(Sky)

  protected:
    Sky() = default;  // For RTTI
  };
}  // namespace REGoth
