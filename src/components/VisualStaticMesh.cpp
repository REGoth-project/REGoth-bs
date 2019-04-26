#include "VisualStaticMesh.hpp"
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ImportStaticMesh.hpp>
#include <Components/BsCRenderable.h>
#include <Scene/BsSceneObject.h>
#include <original-content/VirtualFileSystem.hpp>
#include <RTTI/RTTI_VisualStaticMesh.hpp>

namespace REGoth
{
  VisualStaticMesh::VisualStaticMesh(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    mRenderable = createRenderable();
  }

  void VisualStaticMesh::setMesh(const bs::String& originalMeshFileName)
  {
    BsZenLib::Res::HMeshWithMaterials mesh;

    if (BsZenLib::HasCachedStaticMesh(originalMeshFileName))
    {
      mesh = BsZenLib::LoadCachedStaticMesh(originalMeshFileName);
    }
    else
    {
      mesh = BsZenLib::ImportAndCacheStaticMesh(originalMeshFileName,
                                                gVirtualFileSystem().getFileIndex());
    }

    if (!mesh)
    {
      bs::gDebug().logWarning("[VisualStaticMesh] Failed to load mesh: " + originalMeshFileName);
      return;
    }

    mRenderable->setMesh(mesh->getMesh());
    mRenderable->setMaterials(mesh->getMaterials());
  }

  bs::HRenderable VisualStaticMesh::createRenderable()
  {
    using namespace bs;
    if (hasRenderableComponent())
    {
      BS_EXCEPT(InvalidStateException, "Scene object should not already have a CRenderable!");
    }

    return SO()->addComponent<bs::CRenderable>();
  }

  bool VisualStaticMesh::hasRenderableComponent()
  {
    return SO()->getComponent<bs::CRenderable>() != nullptr;
  }

  REGOTH_DEFINE_RTTI(VisualStaticMesh);
}  // namespace REGoth
