#include "VisualMorphMesh.hpp"
#include <BsZenLib/ImportMorphMesh.hpp>
#include <BsZenLib/ImportPath.hpp>
#include <Components/BsCRenderable.h>
#include <RTTI/RTTI_VisualMorphMesh.hpp>
#include <Scene/BsSceneObject.h>
#include <original-content/VirtualFileSystem.hpp>

namespace REGoth
{
  VisualMorphMesh::VisualMorphMesh(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    mRenderable = createRenderable();
  }

  void VisualMorphMesh::setMesh(const bs::String& originalMeshFileName)
  {
    BsZenLib::Res::HMeshWithMaterials mesh;

    if (BsZenLib::HasCachedMorphMesh(originalMeshFileName))
    {
      mesh = BsZenLib::LoadCachedMorphMesh(originalMeshFileName);
    }
    else
    {
      mesh = BsZenLib::ImportAndCacheMorphMesh(originalMeshFileName,
                                                gVirtualFileSystem().getFileIndex());
    }

    if (!mesh)
    {
      bs::gDebug().logWarning("[VisualMorphMesh] Failed to load mesh: " + originalMeshFileName);
      return;
    }

    mRenderable->setMesh(mesh->getMesh());
    mRenderable->setMaterials(mesh->getMaterials());
  }

  bs::HRenderable VisualMorphMesh::createRenderable()
  {
    using namespace bs;
    if (hasRenderableComponent())
    {
      BS_EXCEPT(InvalidStateException, "Scene object should not already have a CRenderable!");
    }

    return SO()->addComponent<bs::CRenderable>();
  }

  bool VisualMorphMesh::hasRenderableComponent()
  {
    return SO()->getComponent<bs::CRenderable>() != nullptr;
  }

  REGOTH_DEFINE_RTTI(VisualMorphMesh)

}  // namespace REGoth
