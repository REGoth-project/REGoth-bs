#include "ZenImporter.hpp"
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ImportStaticMesh.hpp>
#include <BsZenLib/ResourceManifest.hpp>
#include <BsZenLib/ZenResources.hpp>
#include <Components/BsCMeshCollider.h>
#include <Physics/BsPhysicsMesh.h>
#include <Resources/BsResources.h>
#include <Scene/BsSceneManager.h>
#include <Scene/BsSceneObject.h>
#include <components/Freepoint.hpp>
#include <components/GameWorld.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/VirtualFileSystem.hpp>
#include <zenload/zCMesh.h>
#include <zenload/zenParser.h>

namespace REGoth
{
  
  bs::HSceneObject ZenImporter::constructFromZEN(HGameWorld gameWorld, const bs::String& zenFile)
  {
    OriginalZen zen;

    bool hasLoadedZEN = importZEN(zenFile, zen);

    if (!hasLoadedZEN)
    {
      REGOTH_LOG(Warning, Uncategorized, "[ConstructFromZEN] Failed to read zen-file: {0}", zenFile);
      return {};
    }

    bs::HSceneObject worldMesh = importWorldMesh(zen);
    worldMesh->setParent(gameWorld->SO());

    importVobs(gameWorld->SO(), gameWorld, zen);
    importWaynet(gameWorld->SO(), zen);

    return worldMesh;
  }

  bs::HSceneObject ZenImporter::loadWorldMeshFromZEN(const bs::String& zenFile)
  {
    OriginalZen zen;

    bool hasLoadedZEN = importZEN(zenFile, zen);

    if (!hasLoadedZEN)
    {
      REGOTH_LOG(Warning, Uncategorized, "[ConstructFromZEN] Failed to read zen-file: {0}", zenFile);
      return {};
    }

    return importWorldMesh(zen);
  }

  void ZenImporter::importVobs(bs::HSceneObject sceneRoot, HGameWorld gameWorld, const OriginalZen& zen)
  {
    for (const ZenLoad::zCVobData& root : zen.vobTree.rootVobs)
    {
      walkVobTree(sceneRoot, gameWorld, root);
    }
  }

  void ZenImporter::walkVobTree(bs::HSceneObject bsfParent, HGameWorld gameWorld,
                          const ZenLoad::zCVobData& zenParent)
  {
    for (const auto& v : zenParent.childVobs)
    {
      bs::HSceneObject so = mVobImporter.importSingleVob(v, bsfParent, gameWorld);

      if (so)
      {
        walkVobTree(so, gameWorld, v);
      }
    }
  }

  /**
   * Import a zenfile and load it into datastructures to work with
   */
  bool ZenImporter::importZEN(const bs::String& zenFile, OriginalZen& result)
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
  bs::HSceneObject ZenImporter::importWorldMesh(const OriginalZen& zen)
  {
    bs::String meshFileName = zen.fileName + ".worldmesh";

    BsZenLib::Res::HMeshWithMaterials mesh;
    if (BsZenLib::HasCachedStaticMesh(meshFileName))
    {
      mesh = BsZenLib::LoadCachedStaticMesh(meshFileName);

      // This shouldn't be needed, but sometimes the worldmesh in mesh->getMesh() seems to get lost?
      if (!mesh.isLoaded())
      {
        REGOTH_LOG(Warning, Uncategorized,
                   "Failed to load cached world mesh of zen {0} - recaching it!", zen.fileName);
        mesh = BsZenLib::ImportAndCacheStaticMesh(meshFileName, zen.worldMesh,
                                                  gVirtualFileSystem().getFileIndex());
      }
    }
    else
    {
      mesh = BsZenLib::ImportAndCacheStaticMesh(meshFileName, zen.worldMesh,
                                                gVirtualFileSystem().getFileIndex());
    }

    bs::HMesh actualMesh = mesh->getMesh();

    if (!mesh.isLoaded() || !actualMesh.isLoaded())
    {
      REGOTH_THROW(InvalidStateException, "Failed to load world mesh for zen " + zen.fileName);
    }

    bs::HSceneObject meshSO    = bs::SceneObject::create(meshFileName);
    bs::HRenderable renderable = meshSO->addComponent<bs::CRenderable>();
    renderable->setMesh(mesh->getMesh());
    renderable->setMaterials(mesh->getMaterials());

    if (!actualMesh->getCachedData())
    {
      REGOTH_LOG(Error, Uncategorized,
                 "Cannot extract world mesh for physics, no mesh data available!");
    }
    else
    {
      bs::HPhysicsMesh physicsMesh =
          bs::PhysicsMesh::create(mesh->getMesh()->getCachedData(), bs::PhysicsMeshType::Triangle);

      bs::Path physicsMeshPath = BsZenLib::GothicPathToCachedStaticMesh(meshFileName + ".physics");

      BsZenLib::AddToResourceManifest(physicsMesh, physicsMeshPath);
      bs::gResources().save(physicsMesh, physicsMeshPath, true);

      bs::HMeshCollider collider = meshSO->addComponent<bs::CMeshCollider>();
      collider->setMesh(physicsMesh);
    }

    return meshSO;
  }

  void ZenImporter::importWaynet(bs::HSceneObject sceneRoot, const OriginalZen& zen)
  {
    const ZenLoad::zCWayNetData& zenWaynet = zen.vobTree.waynet;

    bs::HSceneObject waynetSO = bs::SceneObject::create("Waynet");
    waynetSO->setParent(sceneRoot);

    HWaynet waynet = waynetSO->addComponent<Waynet>();

    bs::Vector<HWaypoint> waypoints;

    for (const ZenLoad::zCWaypointData& zenWP : zenWaynet.waypoints)
    {
      bs::String wpName     = zenWP.wpName.c_str();
      bs::HSceneObject wpSO = bs::SceneObject::create(wpName);
      wpSO->setParent(waynetSO);

      bs::Vector3 positionCM = bs::Vector3(zenWP.position.x, zenWP.position.y, zenWP.position.z);

      wpSO->setPosition(positionCM * 0.01f);
      wpSO->setForward(bs::Vector3(zenWP.direction.x, zenWP.direction.y, zenWP.direction.z));

      HWaypoint wp = wpSO->addComponent<Waypoint>();

      waynet->addWaypoint(wp);
      waypoints.push_back(wp);
    }

    for (const auto& edge : zenWaynet.edges)
    {
      if (edge.first > waypoints.size() || edge.second > waypoints.size())
      {
        REGOTH_THROW(InvalidParametersException, "Waynet Edge Indices out of range!");
      }

      waypoints[edge.first]->addPathTo(waypoints[edge.second]);
      waypoints[edge.second]->addPathTo(waypoints[edge.first]);
    }

    // FIXME: Initializes internal data structures for findComponents() to work. Should be removed
    //        once this is fixed upstream.
    bs::gSceneManager().setComponentState(bs::ComponentState::Paused);
    bs::gSceneManager().setComponentState(bs::ComponentState::Running);

    // Find Freepoints and save them into the waynet
    auto freepoints = bs::gSceneManager().findComponents<Freepoint>(false);

    for (const auto& fp : freepoints)
    {
      waynet->addFreepoint(fp);
    }
  }

}  // namespace REGoth
