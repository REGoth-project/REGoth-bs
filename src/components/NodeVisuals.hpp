#pragma once
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>

namespace REGoth
{
  /**
   * Component for attaching visuals to nodes of the skeleton.
   *
   * It will work using gothic standard visual notation, such as `STONE.3DS`
   * describing a static mesh. Such visuals will move with the skeleton as
   * it is animated, like weapons held in the hand of a character.
   *
   * Note that the SceneObject this is attached to must also have a CAnimation-Component
   * for this to work.
   */
  class NodeVisuals : public bs::Component
  {
  public:
    NodeVisuals(const bs::HSceneObject& parent);

    /**
     * @return Whether a specific node exists and something could be attached to it.
     */
    bool hasNode(const bs::String& name) const;

    /**
     * Attaches the given visual to the given node.
     *
     * Existing attachments will be replaced.
     * If either node or visual cannot be found, then nothing happens.
     *
     * @param  node    The node name to attach to, e.g. `BIP01 HEAD`.
     * @param  visual  The original file name of the visual, e.g. `STONE.3DS`.
     */
    void attachVisualToNode(const bs::String& node, const bs::String& visual);

    /**
     * Removes the current attachment from the given node, if one exists.
     *
     * @param  node  Name of the node to remove the attached visual from.
     */
    void clearNodeAttachment(const bs::String& node);

  private:

    /**
     * @return The current skeleton used by the scene objects renderable component.
     */
    bs::SPtr<bs::Skeleton> getSkeleton() const;
  };

  using HNodeVisuals = bs::GameObjectHandle<NodeVisuals>;
}
