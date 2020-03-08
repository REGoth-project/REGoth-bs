#include "VobImporter.hpp"
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ResourceManifest.hpp>
#include <Components/BsCMeshCollider.h>
#include <Components/BsCRenderable.h>
#include <FileSystem/BsFileSystem.h>
#include <Mesh/BsMesh.h>
#include <Physics/BsPhysicsMesh.h>
#include <Resources/BsResources.h>
#include <Scene/BsSceneObject.h>
#include <components/Freepoint.hpp>
#include <components/GameWorld.hpp>
#include <components/Item.hpp>
#include <components/Visual.hpp>
#include <log/logging.hpp>

namespace REGoth
{

  bs::HSceneObject VobImporter::importSingleVob(const ZenLoad::zCVobData& vob,
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
      return import_zCVobStartpoint(vob, bsfParent, gameWorld);
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

  bs::Transform VobImporter::transformFromVob(const ZenLoad::zCVobData& vob)
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
  bs::HSceneObject VobImporter::import_zCVob(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
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
  bs::HSceneObject VobImporter::import_zCVobLight(const ZenLoad::zCVobData& vob,
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
  bs::HSceneObject VobImporter::import_zCVobStartpoint(const ZenLoad::zCVobData& vob,
                                                 bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // Startpoint is found by name of the scene object
    REGOTH_LOG(Info, Uncategorized, "[ImportSingleVob] Found startpoint: {0}", so->getName());

    // A startpoint has an arbitrary names, which makes it hard to find it at a later point.
    // Thus, rename it to a known constant. As there should always only be one startpoint in
    // a world, this should cause no conflicts.
    so->setName(WORLD_STARTPOINT);

    return so;
  }

  /**
   * Spots like free-points.
   */
  bs::HSceneObject VobImporter::import_zCVobSpot(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                           HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    so->addComponent<Freepoint>();

    return so;
  }

  bs::HSceneObject VobImporter::import_oCItem(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
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

  bs::HSceneObject VobImporter::import_zCVobSound(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject VobImporter::import_zCVobAnimate(const ZenLoad::zCVobData& vob,
                                              bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject VobImporter::import_oCMobInter(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject VobImporter::import_oCMobContainer(const ZenLoad::zCVobData& vob,
                                                bs::HSceneObject bsfParent, HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject VobImporter::import_oCMobBed(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                          HGameWorld gameWorld)
  {
    bs::HSceneObject so = import_zCVob(vob, bsfParent, gameWorld);

    // TODO: Implement

    return so;
  }

  bs::HSceneObject VobImporter::import_oCMobDoor(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
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
  void VobImporter::addVisualTo(bs::HSceneObject sceneObject, const bs::String& visualName)
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
  void VobImporter::addCollisionTo(bs::HSceneObject sceneObject)
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

  bs::Matrix4 VobImporter::convertMatrix(const ZMath::Matrix& m)
  {
    bs::Matrix4 bs = {m.mv[0], m.mv[1], m.mv[2],  m.mv[3],  m.mv[4],  m.mv[5],  m.mv[6],  m.mv[7],
                      m.mv[8], m.mv[9], m.mv[10], m.mv[11], m.mv[12], m.mv[13], m.mv[14], m.mv[15]};

    return bs.transpose();
  }

}  // namespace REGoth
