/** \file
 */

#pragma once

#include <BsPrerequisites.h>
#include <BsZenLib/ZenResources.hpp>
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class MeshVisual;
  using HMeshVisual = bs::GameObjectHandle<MeshVisual>;

  /**
   * Component for loading and rendering a static mesh (Decoration, Items).
   *
   * After this component is intialized, there will be a renderable component
   * attached to the scene object.
   */
  class VisualStaticMesh : public bs::Component
  {
  public:
    VisualStaticMesh(const bs::HSceneObject& parent);

    virtual void onInitialized() override;

    /**
     * Set which mesh this component should display.
     *
     * @param  originalMeshFile  The name of the mesh file the original game would
     *                           have set (e.g. "STONE.3DS").
     */
    void setMesh(const bs::String& originalMeshFileName);

  private:

    /**
     * Creates the renderable on the scene object
     */
    HMeshVisual createRenderable();

    /**
     * @return Whether there is a renderable component attached to
     *         the scene object.
     */
    bool hasRenderableComponent();

  private:

    /**
     * Renderable this is setting up for drawing
     */
    HMeshVisual mVisual;

  public:
    REGOTH_DECLARE_RTTI(VisualStaticMesh)

  protected:
    VisualStaticMesh() = default; // For RTTI
  };

  using HVisualStaticMesh = bs::GameObjectHandle<VisualStaticMesh>;
}
