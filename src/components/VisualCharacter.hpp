/** \file
 */

#pragma once

#include "VisualSkeletalAnimation.hpp"
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class RTTI_VisualCharacter;
  class NodeVisuals;
  using HNodeVisuals = bs::GameObjectHandle<NodeVisuals>;

  /**
   * Component for rendering an animated character (Player, NPC, Monster).
   *
   * This component extends the VisualSkeletalAnimation-Component and
   * adds support for character specific features, such as
   *
   *  - setting the body and head meshes with the correct textures
   *  - Easy access to some node attachments, like the left and right hand
   */
  class VisualCharacter : public VisualSkeletalAnimation
  {
  public:
    VisualCharacter(const bs::HSceneObject& parent);

    /**
     * Sets the body mesh.
     *
     * Throws if the body mesh is not listed inside the model script.
     *
     * @param  bodyMesh  Name of the body mesh to use, e.g. `HUM_BODY_NAKED0`. The
     *                   file extension can be omitted.
     */
    void setBodyMesh(const bs::String& bodyMesh);

    /**
     * Sets the headmesh for this model.
     *
     * @param  head  File of the mesh to use as head, e.g. `HUM_HEAD.MMB`. The file
     *               extension is not required. If none was given, `.MMB` will be
     *               assumed.
     */
    void setHeadMesh(const bs::String& headmesh, bs::UINT32 headTextureIdx = 0,
                     bs::UINT32 teethTextureIdx = 0);

  protected:
    bs::Vector<bs::String> listPossibleDefaultAnimations() const override;

  private:
    /**
     * Replaces the current body mesh of this model from the current body-state
     */
    void updateBodyMesh();

    /**
     * Replaces the current headmesh of this model from the current body-state
     */
    void updateHeadMesh();

    struct BodyState
    {
      bs::String headVisual       = "";
      bs::String bodyVisual       = "";
      bs::UINT32 headTextureIdx   = 0;
      bs::UINT32 teethTextureIdx  = 0;
      bs::UINT32 bodySkinColorIdx = 0;
      bs::UINT32 bodyTextureIdx   = 0;
    };

    // Body Visual Settings ---------------------------------------------------
    BodyState mBodyState;

  public:
    REGOTH_DECLARE_RTTI(VisualCharacter);

  protected:
    VisualCharacter() = default; // For RTTI
  };

  using HVisualCharacter = bs::GameObjectHandle<VisualCharacter>;
}  // namespace REGoth
