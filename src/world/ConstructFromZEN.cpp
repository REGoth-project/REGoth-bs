#include "ConstructFromZEN.hpp"
#include "internals/ImportSingleVob.hpp"
#include <BsZenLib/ImportStaticMesh.hpp>
#include <BsZenLib/ZenResources.hpp>
#include <Components/BsCMeshCollider.h>
#include <Physics/BsPhysicsMesh.h>
#include <Scene/BsSceneObject.h>
#include <original-content/VirtualFileSystem.hpp>
#include <zenload/zCMesh.h>
#include <zenload/zenParser.h>

namespace REGoth
{
  struct OriginalZen
  {
    bs::String fileName;
    ZenLoad::oCWorldData vobTree;
    ZenLoad::PackedMesh worldMesh;
  };

  static bool importZEN(const bs::String& zenFile, OriginalZen& result);
  static bs::HSceneObject importWorldMesh(const OriginalZen& zen);
  static void importVobs(bs::HSceneObject sceneRoot, const OriginalZen& zen);
  static void walkVobTree(bs::HSceneObject bsfParent, const ZenLoad::zCVobData& zenParent);

  bs::HSceneObject World::constructFromZEN(const bs::String& zenFile)
  {
    OriginalZen zen;

    bool hasLoadedZEN = importZEN(zenFile, zen);

    if (!hasLoadedZEN)
    {
      bs::gDebug().logWarning("[ConstructFromZEN] Failed to read zen-file: " + zenFile);
      return {};
    }

    bs::HSceneObject worldMesh = importWorldMesh(zen);

    importVobs(worldMesh, zen);

    return worldMesh;
  }

  static void importVobs(bs::HSceneObject sceneRoot, const OriginalZen& zen)
  {
    for (const ZenLoad::zCVobData& root : zen.vobTree.rootVobs)
    {
      walkVobTree(sceneRoot, root);
    }
  }

  static void walkVobTree(bs::HSceneObject bsfParent, const ZenLoad::zCVobData& zenParent)
  {
    for (const auto& v : zenParent.childVobs)
    {
      bs::HSceneObject so = World::importSingleVob(v);

      if (so)
      {
        so->setParent(bsfParent);

        walkVobTree(so, v);
      }
    }
  }

  /**
   * Import a zenfile and load it into datastructures to work with
   */
  static bool importZEN(const bs::String& zenFile, OriginalZen& result)
  {
    ZenLoad::ZenParser zenParser(zenFile.c_str(), gVirtualFileSystem().getFileIndex());

    if (zenParser.getFileSize() == 0) return false;

    zenParser.readHeader();

    result.fileName = zenFile;

    zenParser.readWorld(result.vobTree);

    // FIXME: Don't pack the mesh if it was already cached, packing takes a long time...
    zenParser.getWorldMesh()->packMesh(result.worldMesh, 0.01f);

    return true;
  }

  /**
   * Create a bs:f scene object holding the world mesh.
   */
  static bs::HSceneObject importWorldMesh(const OriginalZen& zen)
  {
    bs::String meshFileName = zen.fileName + ".worldmesh";

    BsZenLib::Res::HMeshWithMaterials mesh;
    if (BsZenLib::HasCachedStaticMesh(meshFileName))
    {
      mesh = BsZenLib::LoadCachedStaticMesh(meshFileName);
    }
    else
    {
      mesh = BsZenLib::ImportAndCacheStaticMesh(meshFileName, zen.worldMesh,
                                                gVirtualFileSystem().getFileIndex());
    }

    if (!mesh || !mesh->getMesh()) return {};

    bs::HSceneObject meshSO = bs::SceneObject::create(meshFileName);
    bs::HRenderable renderable = meshSO->addComponent<bs::CRenderable>();
    renderable->setMesh(mesh->getMesh());
    renderable->setMaterials(mesh->getMaterials());

    bs::HMesh actualMesh = mesh->getMesh();

    if (!actualMesh)
    {
      bs::gDebug().logError("Mesh not there?");
      return {};
    }

    if (!actualMesh->getCachedData())
    {
      bs::gDebug().logError("Cannot extract world mesh for physics, no mesh data available!");
    }
    else
    {
      bs::HPhysicsMesh physicsMesh =
          bs::PhysicsMesh::create(mesh->getMesh()->getCachedData(), bs::PhysicsMeshType::Triangle);

      bs::HMeshCollider collider = meshSO->addComponent<bs::CMeshCollider>();
      collider->setMesh(physicsMesh);
    }

    return meshSO;
  }

}  // namespace REGoth
