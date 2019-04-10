#include "NodeVisuals.hpp"
#include <Animation/BsSkeleton.h>
#include <Components/BsCBone.h>
#include <Components/BsCRenderable.h>
#include <Mesh/BsMesh.h>
#include <Scene/BsSceneObject.h>
#include <components/Visual.hpp>
#include <components/VisualStaticMesh.hpp>

namespace REGoth
{
  NodeVisuals::NodeVisuals(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
  }

  bool NodeVisuals::hasNode(const bs::String& name) const
  {
    bs::SPtr<bs::Skeleton> skeleton = getSkeleton();

    if (!skeleton) return false;

    for (bs::UINT32 i = 0; i < skeleton->getNumBones(); i++)
    {
      if (skeleton->getBoneInfo(i).name == name) return true;
    }

    return false;
  }

  void NodeVisuals::attachVisualToNode(const bs::String& node, const bs::String& visual)
  {
    clearNodeAttachment(node);

    assert(hasNode(node));

    bs::SPtr<bs::Skeleton> skeleton = getSkeleton();

    for (bs::UINT32 i = 0; i < skeleton->getNumBones(); i++)
    {
      bs::gDebug().logDebug("Bone: " + skeleton->getBoneInfo(i).name);
    }

    bs::HSceneObject boneSO = bs::SceneObject::create(node);
    boneSO->setParent(SO());

    bs::HBone bone = boneSO->addComponent<bs::CBone>();
    bone->setBoneName(node);

    bool hasCreated = Visual::addToSceneObject(boneSO, visual);

    if (!hasCreated)
    {
      clearNodeAttachment(node);

      bs::gDebug().logWarning("[NodeVisuals] Failed to attach visual '" + visual + "' to node '" +
                              node + "'");
    }
  }

  void NodeVisuals::clearNodeAttachment(const bs::String& node)
  {
    bs::HSceneObject bone = SO()->findChild(node);

    if (!bone.isDestroyed())
    {
      bone->destroy();
    }
  }

  bs::SPtr<bs::Skeleton> NodeVisuals::getSkeleton() const
  {
    bs::HRenderable renderable = SO()->getComponent<bs::CRenderable>();

    if (!renderable) return nullptr;

    if (!renderable->getMesh()) return nullptr;

    return renderable->getMesh()->getSkeleton();
  }
}  // namespace REGoth
