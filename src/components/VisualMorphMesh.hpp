/** \file
 */

#pragma once

#include <BsPrerequisites.h>
#include <BsZenLib/ZenResources.hpp>
#include <Scene/BsComponent.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  /**
   * Component for loading and rendering a morph-mesh (`.MMB`) for e.g. Heads, flags.
   *
   * After this component is intialized, there will be a renderable component
   * attached to the scene object.
   */
  class VisualMorphMesh : public bs::Component
  {
  public:
    VisualMorphMesh(const bs::HSceneObject& parent);

    /**
     * Set which mesh this component should display.
     *
     * @param  originalMeshFile  The name of the mesh file the original game would
     *                           have set (e.g. "HUM_HEAD.MMD").
     */
    void setMesh(const bs::String& originalMeshFileName);

  private:

    /**
     * Creates the renderable on the scene object
     */
    bs::HRenderable createRenderable();

    /**
     * @return Whether there is a renderable component attached to
     *         the scene object.
     */
    bool hasRenderableComponent();

  private:

    /**
     * Renderable this is setting up for drawing
     */
    bs::HRenderable mRenderable;

  public:
    REGOTH_DECLARE_RTTI(VisualMorphMesh)

  public:  // FIXME: RTTI, make protected
    VisualMorphMesh() = default;
  };

  using HVisualMorphMesh = bs::GameObjectHandle<VisualMorphMesh>;
}
