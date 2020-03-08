#include "ZenImporter.hpp"
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ImportStaticMesh.hpp>
#include <BsZenLib/ResourceManifest.hpp>
#include <BsZenLib/ZenResources.hpp>
#include <FileSystem/BsFileSystem.h>
#include <Components/BsCMeshCollider.h>
#include <Components/BsCRenderable.h>
#include <Mesh/BsMesh.h>
#include <Physics/BsPhysicsMesh.h>
#include <Resources/BsResources.h>
#include <Scene/BsSceneManager.h>
#include <Scene/BsSceneObject.h>
#include <components/Freepoint.hpp>
#include <components/GameWorld.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <components/Item.hpp>
#include <components/Visual.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/VirtualFileSystem.hpp>
#include <zenload/zCMesh.h>
#include <zenload/zenParser.h>

namespace REGoth
{
  ZenImporter::ZenImporter() : mVobImporter(new VobImporter)
  {}

  ZenImporter::~ZenImporter(){
    delete mVobImporter;
  }

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
      bs::HSceneObject so = mVobImporter->importSingleVob(*this, v, bsfParent, gameWorld);

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

namespace REGoth
{

  bs::HSceneObject ZenImporter::VobImporter::importSingleVob(ZenImporter& zenImporter, const ZenLoad::zCVobData& vob,
                                              bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    if (vob.objectClass == "zCVob")
    {
      return import_zCVob(vob, bsfParent, gameWorld);
    }
    else if (vob.objectClass == "zCVobLight:zCVob")
    {
      return import_zCVobLight(vob, bsfParent, gameWorld);
    }
    else if (vob.objectClass == "zCVobStartpoint:zCVob")
    {
      return import_zCVobStartpoint(zenImporter, vob, bsfParent, gameWorld);
    }
    else if (vob.objectClass == "zCVobSpot:zCVob")
    {
      return import_zCVobSpot(vob, bsfParent, gameWorld);
    }
    else if (vob.objectClass == "zCVobSound:zCVob")
    {
      return import_zCVobSound(vob, bsfParent, gameWorld);
    }
    else if (vob.objectClass == "oCItem:zCVob")
    {
      return import_oCItem(vob, bsfParent, gameWorld);
    }
    else if (vob.objectClass == "zCVobAnimate:zCVob")
    {
      return import_zCVobAnimate(vob, bsfParent, gameWorld);
    }
    // else if (vob.objectClass == "oCMobInter:oCMOB:zCVob")
    // {
    //   return import_oCMobInter(vob, bsfParent, gameWorld);
    // }
    // else if (vob.objectClass == "oCMobContainer:oCMobInter:oCMOB:zCVob")
    // {
    //   return import_oCMobContainer(vob, bsfParent, gameWorld);
    // }
    // else if (vob.objectClass == "oCMobBed:oCMobInter:oCMOB:zCVob")
    // {
    //   return import_oCMobBed(vob, bsfParent, gameWorld);
    // }
    // else if (vob.objectClass == "oCMobDoor:oCMobInter:oCMOB:zCVob")
    // {
    //   return import_oCMobDoor(vob, bsfParent, gameWorld);
    // }
    else
    {
      REGOTH_LOG(Warning, Uncategorized, "[ImportSingleVob] Unsupported vob class: {0}",
                 bs::String(vob.objectClass.c_str()));

      return {};
    }
  }

  bs::Transform ZenImporter::VobImporter::transformFromVob(const ZenLoad::zCVobData& vob)
  {
    bs::Transform result = bs::Transform::IDENTITY;

    bs::Matrix4 worldMatrix = convertMatrix(vob.worldMatrix);
    bs::Quaternion rotation;
    rotation.fromRotationMatrix(worldMatrix.get3x3());

    bs::Vector3 positionCM = bs::Vector3(vob.position.x, vob.position.y, vob.position.z);

    float centimetersToMeters = 0.01f;

    result.setPosition(positionCM * centimetersToMeters);
    result.setRotation(rotation);

    return result;
  }

  /**
   * The zCVob is the most basic object class we will encounter. It is
   * the base class of most other objects, so it makes sense to handle
   * position, rotation and the visual here as these are used by most vobs.
   *
   * @note Not all object types will call `import_zCVob`!
   */
  bs::HSceneObject ZenImporter::VobImporter::import_zCVob(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                       HGameWorld gameWorld)
  {
    bs::HSceneObject so = bs::SceneObject::create(vob.vobName.c_str());

    so->setParent(gameWorld->SO());

    bs::Transform transform = transformFromVob(vob);

    so->setPosition(transform.pos());
    so->setRotation(transform.rot());

    if (!vob.visual.empty())
    {
      addVisualTo(so, vob.visual.c_str());
    }

    // cdDyn seems to be the general "this is supposed to collide with stuff"-flag.
    if (vob.cdDyn)
    {
      addCollisionTo(so);
    }

    return so;
  }

  /**
   * Lights can be pointlights or spotlights, although spotlights are not
   * used within the original game as it seems.
   */
  bs::HSceneObject ZenImporter::VobImporter::import_zCVobLight(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // FIXME: Put lights back in
    return so;
#if 0
    bs::HLight light = so->addComponent<bs::CLight>();

    auto lightColor = bs::Color::fromRGBA(vob.zCVobLight.color);

    light->setType(bs::LightType::Radial);
    light->setUseAutoAttenuation(false);
    light->setAttenuationRadius(vob.zCVobLight.range);
    light->setColor(lightColor);

    return so;
#endif
  }

  /**
   * The startpoint of the player in the current world. There should be only one.
   */
  bs::HSceneObject ZenImporter::VobImporter::import_zCVobStartpoint(ZenImporter& zenImporter, const ZenLoad::zCVobData& vob,
                                                 bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);
    //FIXME: Rotate by 180Degrees?; Why do I spawn in the Oldmine looking the wrong way?
    /*
    bs::Quaternion rotate180Y = bs::Quaternion(bs::Radian(0),
                                              bs::Radian(bs::Degree(180)),
                                              bs::Radian(0));
    so->rotate(rotate180Y);
*/

    // Startpoint is found by name of the scene object
    REGOTH_LOG(Info, Uncategorized, "[ImportSingleVob] Found startpoint: {0}", so->getName());

    zenImporter.mStartpoint = so;

    return so;
  }

  /**
   * Spots like free-points.
   */
  bs::HSceneObject ZenImporter::VobImporter::import_zCVobSpot(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                           HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    so->addComponent<Freepoint>();

    return so;
  }

  bs::HSceneObject ZenImporter::VobImporter::import_oCItem(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                        HGameWorld gameWorld)
  {
    if (vob.oCItem.instanceName.empty())
    {
      REGOTH_LOG(Warning, Uncategorized, "[ImportSingleVob] Item with empty script instance: {0}",
                 bs::String(vob.vobName.c_str()));
      return {};
    }

    bs::Transform transform = transformFromVob(vob);

    // Items are interactable, they need to be registered within the GameWorld
    HItem item = gameWorld->insertItem(vob.oCItem.instanceName.c_str(), transform);

    item->SO()->setParent(bsfParent);

    return item->SO();
  }

  bs::HSceneObject ZenImporter::VobImporter::import_zCVobSound(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject ZenImporter::VobImporter::import_zCVobAnimate(const ZenLoad::zCVobData& vob,
                                              bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject ZenImporter::VobImporter::import_oCMobInter(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject ZenImporter::VobImporter::import_oCMobContainer(const ZenLoad::zCVobData& vob,
                                                bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject ZenImporter::VobImporter::import_oCMobBed(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                          HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject ZenImporter::VobImporter::import_oCMobDoor(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                           HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  /**
   * Adds the given visual to the scene object. If that's not possible
   * nothing will be added.
   */
  void ZenImporter::VobImporter::addVisualTo(bs::HSceneObject sceneObject, const bs::String& visualName)
  {
    bool hasAdded = Visual::addToSceneObject(sceneObject, visualName);

    if (!hasAdded)
    {
      REGOTH_LOG(Warning, Uncategorized, "[ImportSingleVob] Unsupported visual: {0}", visualName);
    }
  }

  /**
   * Adds a triangle physics mesh to the given scene object. Only works if the
   * scene object has a renderable with a mesh set. The mesh must also have
   * CPU-caching enabled, so we get access to the mesh data.
   */
  void ZenImporter::VobImporter::addCollisionTo(bs::HSceneObject sceneObject)
  {
    bs::HRenderable renderable = sceneObject->getComponent<bs::CRenderable>();

    if (!renderable) return;

    auto mesh = renderable->getMesh();

    if (!mesh) return;

    auto meshData = mesh->getCachedData();

    if (!meshData) return;

    bs::Path physicsMeshPath = BsZenLib::GothicPathToCachedStaticMesh(mesh->getName() + ".physics");

    bs::HPhysicsMesh physicsMesh;
    if (bs::FileSystem::exists(physicsMeshPath))
    {
      physicsMesh = bs::gResources().load<bs::PhysicsMesh>(physicsMeshPath);
    }
    else
    {
      REGOTH_LOG(Info, Uncategorized, "[ImportSingleVob] Caching physics mesh for {0}",
                 mesh->getName());

      physicsMesh = bs::PhysicsMesh::create(meshData, bs::PhysicsMeshType::Triangle);

      BsZenLib::AddToResourceManifest(physicsMesh, physicsMeshPath);
      bs::gResources().save(physicsMesh, physicsMeshPath, true);
    }

    if (!physicsMesh) return;

    bs::HMeshCollider collider = sceneObject->addComponent<bs::CMeshCollider>();
    collider->setMesh(physicsMesh);
  }

  bs::Matrix4 ZenImporter::VobImporter::convertMatrix(const ZMath::Matrix& m)
  {
    bs::Matrix4 bs = {m.mv[0], m.mv[1], m.mv[2],  m.mv[3],  m.mv[4],  m.mv[5],  m.mv[6],  m.mv[7],
                      m.mv[8], m.mv[9], m.mv[10], m.mv[11], m.mv[12], m.mv[13], m.mv[14], m.mv[15]};

    return bs.transpose();
  }

}  // namespace REGoth
