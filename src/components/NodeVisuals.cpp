#include "NodeVisuals.hpp"
#include <Animation/BsSkeleton.h>
#include <Components/BsCBone.h>
#include <Components/BsCRenderable.h>
#include <Mesh/BsMesh.h>
#include <RTTI/RTTI_NodeVisuals.hpp>
#include <Scene/BsSceneObject.h>
#include <components/MeshVisual.hpp>
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

    bs::HSceneObject boneSO = bs::SceneObject::create(node);

    enum
    {
      KeepWorldTransform   = true,
      MoveRelativeToParent = false,
    };

    boneSO->setParent(SO(), MoveRelativeToParent);

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

  void NodeVisuals::attachMeshToNode(const bs::String& node, BsZenLib::Res::HMeshWithMaterials mesh)
  {
    clearNodeAttachment(node);

    bs::HSceneObject boneSO = bs::SceneObject::create(node);

    enum
    {
      KeepWorldTransform   = true,
      MoveRelativeToParent = false,
    };

    boneSO->setParent(SO(), MoveRelativeToParent);

    bs::HBone bone = boneSO->addComponent<bs::CBone>();
    bone->setBoneName(node);

    HMeshVisual renderable = boneSO->addComponent<MeshVisual>();
    renderable->setMesh(mesh->getMesh());
    renderable->setMaterials(mesh->getMaterials());
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
    HMeshVisual renderable = SO()->getComponent<MeshVisual>();

    if (!renderable) return nullptr;

    if (!renderable->renderable()->getMesh()) return nullptr;

    return renderable->renderable()->getMesh()->getSkeleton();
  }

  REGOTH_DEFINE_RTTI(NodeVisuals)
}  // namespace REGoth
