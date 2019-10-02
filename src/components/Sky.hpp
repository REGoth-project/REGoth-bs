#pragma once

#include <memory>

#include <Image/BsColor.h>
#include <Scene/BsComponent.h>

#include <RTTI/RTTIUtil.hpp>
#include <components/SkyStateGenerator.hpp>

namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  /**
   * @brief The `Sky` component renders the sky, either as plane or dome.
   *
   * The render mode can be specified using the `renderMode` construction parameter.
   */
  class Sky : public bs::Component
  {
  public:
    /**
     * @brief The utilized render mode, i.e., whether to render the sky as a textured dome or a
     *        plane.
     */
    enum class RenderMode
    {
      Dome,
      Plane
    };

    /**
     * @brief Constructs a `Sky` component.
     * @param parent Parent scene object.
     * @param gameWorld Instance to the `GameWorld` component.
     * @param renderMode Render mode to be used, i.e., `RenderMode::Plane` or `RenderMode::Dome`.
     * @param skyColor Base sky color.
     */
    Sky(const bs::HSceneObject& parent, HGameWorld gameWorld, const RenderMode& renderMode,
        const bs::Color& skyColor);

    /**
     * @brief Destructs the `Sky` component.
     */
    virtual ~Sky() override;

    /**
     * @see bs::Component::onInitialized
     */
    void onInitialized() override;

    /**
     * @brief bs::Component::update
     */
    void update() override;

  private:
    /**
     * @brief Renders the fog.
     * @param color Fog color.
     * @param fogNear Near distance.
     * @param fogFar Far distance.
     */
    void renderFog(const bs::Color& color, const float fogNear, const float fogFar) const;

    /**
     * @brief Renders the sky depending on the render mode.
     *
     * @note Delegates to `renderSkyPlane` or `renderSkyDome`, depending on `mRenderMode`.
     *
     * @param skyState Current interpolated sky state.
     */
    void renderSky(const std::shared_ptr<const SkyState> skyState) const;

    /**
     * @brief Renders the sky plane.
     * @param skyState Current interpolated sky state.
     */
    void renderSkyPlane(std::shared_ptr<const SkyState> skyState) const;

    /**
     * @brief Renders the sky dome.
     * @param skyState Current interpolated sky state.
     *
     * @note Not yet implemented, therefore tagged as [[noreturn]].
     * @throw NotImplementedException.
     */
    [[noreturn]] void renderSkyDome(std::shared_ptr<const SkyState> skyState) const;

    /**
     * @brief Instance of a `SkyStateGenerator` used to interoplate key sky states.
     */
    SkyStateGenerator mSkyStateGen{bs::Color{}, bs::String{}};

    /**
     * @brief Instance of the current `GameWorld`.
     */
    HGameWorld mGameWorld;

    /**
     * @brief Currently used render mode.
     */
    const RenderMode mRenderMode = RenderMode::Plane;

    /**
     * @brief Used sky material.
     */
    bs::HMaterial mSkyMaterial;

    /**
     * @brief Used sky renderable.
     */
    bs::HRenderable mSkyRenderable;

  public:
    REGOTH_DECLARE_RTTI(Sky)

  protected:
    Sky() = default;  // For RTTI
  };
}  // namespace REGoth
