#pragma once
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

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
    Sky(const bs::HSceneObject& parent, HGameWorld gameWorld);
    virtual ~Sky();

    void onInitialized() override;

    void update() override;

  private:

    void applySkySettingsToCamera() const;

    bs::SPtr<SkyColoring> mSkyColoring;
    HGameWorld mGameWorld;

  public:
    REGOTH_DECLARE_RTTI(Sky)

  protected:
    Sky() = default;  // For RTTI
  };
}  // namespace REGoth
