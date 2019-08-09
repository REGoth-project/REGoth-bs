#pragma once

#include <Image/BsColor.h>
#include <Scene/BsComponent.h>

#include <RTTI/RTTIUtil.hpp>
#include <components/SkyStateGenerator.hpp>

namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  /**
   * TODO: Documentation of Sky
   */
  class Sky : public bs::Component
  {
  public:
    /**
     * The utilised render mode, i.e. render the sky as a textured dome or a plane.
     */
    enum class RenderMode
    {
      Dome,
      Plane
    };

    Sky(const bs::HSceneObject& parent, HGameWorld gameWorld, const RenderMode& renderMode,
        const bs::Color& skyColor);
    virtual ~Sky() override;

    void update() override;

  private:
    void renderFog(const bs::Color& color, float fogNear, float fogFar) const;
    void renderSky() const;

    SkyStateGenerator mSkyStateGen{bs::Color{}, bs::String{}};
    HGameWorld mGameWorld;
    const RenderMode mRenderMode = RenderMode::Plane;

  public:
    REGOTH_DECLARE_RTTI(Sky)

  protected:
    Sky() = default;  // For RTTI
  };
}  // namespace REGoth
