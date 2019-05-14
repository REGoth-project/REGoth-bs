#include "ImportSingleVob.hpp"
#include <Components/BsCLight.h>
#include <Math/BsMatrix4.h>
#include <Scene/BsSceneObject.h>
#include <components/Item.hpp>
#include <components/Visual.hpp>
#include <components/VisualStaticMesh.hpp>
#include <zenload/zTypes.h>

namespace
{
  static bs::Matrix4 convertMatrix(const ZMath::Matrix& m)
  {
    bs::Matrix4 bs = {m.mv[0], m.mv[1], m.mv[2],  m.mv[3],  m.mv[4],  m.mv[5],  m.mv[6],  m.mv[7],
                      m.mv[8], m.mv[9], m.mv[10], m.mv[11], m.mv[12], m.mv[13], m.mv[14], m.mv[15]};

    return bs.transpose();
  }
}  // namespace

namespace REGoth
{
  static bs::HSceneObject import_zCVob(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO);
  static bs::HSceneObject import_zCVobLight(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject parentSO);
  static bs::HSceneObject import_zCVobStartpoint(const ZenLoad::zCVobData& vob,
                                                 bs::HSceneObject parentSO);
  static bs::HSceneObject import_zCVobSpot(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO);
  static bs::HSceneObject import_oCItem(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO);
  static bs::HSceneObject import_zCVobSound(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject parentSO);
  static bs::HSceneObject import_zCVobAnimate(const ZenLoad::zCVobData& vob,
                                              bs::HSceneObject parentSO);
  static bs::HSceneObject import_oCMobInter(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject parentSO);
  static bs::HSceneObject import_oCMobContainer(const ZenLoad::zCVobData& vob,
                                                bs::HSceneObject parentSO);
  static bs::HSceneObject import_oCMobBed(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO);
  static bs::HSceneObject import_oCMobDoor(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO);
  static void addVisualTo(bs::HSceneObject sceneObject, const bs::String& visualName);

  bs::HSceneObject Internals::importSingleVob(const ZenLoad::zCVobData& vob,
                                              bs::HSceneObject parentSO)
  {
    if (vob.objectClass == "zCVob")
    {
      return import_zCVob(vob, parentSO);
    }
    else if (vob.objectClass == "zCVobLight:zCVob")
    {
      return import_zCVobLight(vob, parentSO);
    }
    else if (vob.objectClass == "zCVobStartpoint:zCVob")
    {
      return import_zCVobStartpoint(vob, parentSO);
    }
    else if (vob.objectClass == "zCVobSpot:zCVob")
    {
      return import_zCVobSpot(vob, parentSO);
    }
    else if (vob.objectClass == "zCVobSound:zCVob")
    {
      return import_zCVobSound(vob, parentSO);
    }
    else if (vob.objectClass == "oCItem:zCVob")
    {
      return import_oCItem(vob, parentSO);
    }
    else if (vob.objectClass == "zCVobAnimate:zCVob")
    {
      return import_zCVobAnimate(vob, parentSO);
    }
    else if (vob.objectClass == "oCMobInter:oCMOB:zCVob")
    {
      return import_oCMobInter(vob, parentSO);
    }
    else if (vob.objectClass == "oCMobContainer:oCMobInter:oCMOB:zCVob")
    {
      return import_oCMobContainer(vob, parentSO);
    }
    else if (vob.objectClass == "oCMobBed:oCMobInter:oCMOB:zCVob")
    {
      return import_oCMobBed(vob, parentSO);
    }
    else if (vob.objectClass == "oCMobDoor:oCMobInter:oCMOB:zCVob")
    {
      return import_oCMobDoor(vob, parentSO);
    }
    else
    {
      bs::gDebug().logWarning("[ImportSingleVob] Unsupported vob class: " +
                              bs::String(vob.objectClass.c_str()));

      return {};
    }
  }

  /**
   * The zCVob is the most basic object class we will encounter. It is
   * the base class of all others, so it makes sense to handle
   * position, rotation and the visual here as these are used by all vobs.
   */
  static bs::HSceneObject import_zCVob(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = bs::SceneObject::create(vob.vobName.c_str());

    so->setParent(parentSO);

    bs::Matrix4 worldMatrix = convertMatrix(vob.worldMatrix);
    bs::Quaternion rotation;
    rotation.fromRotationMatrix(worldMatrix.get3x3());

    bs::Vector3 positionCM = bs::Vector3(vob.position.x, vob.position.y, vob.position.z);

    float centimetersToMeters = 0.01f;
    so->setPosition(positionCM * centimetersToMeters);
    so->setRotation(rotation);

    if (!vob.visual.empty())
    {
      addVisualTo(so, vob.visual.c_str());
    }

    return so;
  }

  /**
   * Lights can be pointlights or spotlights, altough spotlights are not
   * used within the original game as it seems.
   */
  static bs::HSceneObject import_zCVobLight(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = import_zCVob(vob, parentSO);

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
  static bs::HSceneObject import_zCVobStartpoint(const ZenLoad::zCVobData& vob,
                                                 bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = import_zCVob(vob, parentSO);

    // Startpoint is found by name of the scene object
    bs::gDebug().logDebug("[ImportSingleVob] Found startpoint: " + so->getName());

    return so;
  }

  /**
   * Spots like free-points.
   */
  static bs::HSceneObject import_zCVobSpot(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = import_zCVob(vob, parentSO);

    // Spots are found by name of the scene object

    return so;
  }

  static bs::HSceneObject import_oCItem(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO)
  {
    if (vob.oCItem.instanceName.empty())
    {
      bs::gDebug().logWarning("[ImportSingleVob] Item with empty script instance: " +
                              bs::String(vob.vobName.c_str()));
      return {};
    }

    bs::HSceneObject so = import_zCVob(vob, parentSO);

    so->addComponent<Item>(vob.oCItem.instanceName.c_str());

    return so;
  }

  static bs::HSceneObject import_zCVobSound(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = import_zCVob(vob, parentSO);

    // TODO: Implement

    return so;
  }

  static bs::HSceneObject import_zCVobAnimate(const ZenLoad::zCVobData& vob,
                                              bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = import_zCVob(vob, parentSO);

    // TODO: Implement

    return so;
  }

  static bs::HSceneObject import_oCMobInter(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = import_zCVob(vob, parentSO);

    // TODO: Implement

    return so;
  }

  static bs::HSceneObject import_oCMobContainer(const ZenLoad::zCVobData& vob,
                                                bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = import_zCVob(vob, parentSO);

    // TODO: Implement

    return so;
  }

  static bs::HSceneObject import_oCMobBed(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = import_zCVob(vob, parentSO);

    // TODO: Implement

    return so;
  }

  static bs::HSceneObject import_oCMobDoor(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO)
  {
    bs::HSceneObject so = import_zCVob(vob, parentSO);

    // TODO: Implement

    return so;
  }

  /**
   * Adds the given visual to the scene object. If that's not possible
   * nothing will be added.
   */
  static void addVisualTo(bs::HSceneObject sceneObject, const bs::String& visualName)
  {
    bool hasAdded = Visual::addToSceneObject(sceneObject, visualName);

    if (!hasAdded)
    {
      bs::gDebug().logWarning("[ImportSingleVob] Unsupported visual: " + visualName);
    }
  }

}  // namespace REGoth
