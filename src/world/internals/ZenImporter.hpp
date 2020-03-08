/** \file
 */

#pragma once

#include <BsPrerequisites.h>
#include <Math/BsMatrix4.h>
#include <zenload/zTypes.h>

namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  class ZenImporter
  {
  public:
    ZenImporter();
    ~ZenImporter();

    /**
     * This function will load the given zenFile from the virtual file system
     * and fully convert it into a bs::f scene.
     *
     * @param  gameWorld  World to create the objects in.
     * @param  zenFile    Uppercase ZEN-file name, e.g. "OLDWORLD.ZEN".
     *
     * @return Root of the created scene.
     */
    bs::HSceneObject constructFromZEN(HGameWorld gameWorld, const bs::String& zenFile);

    /**
     * Will load the given ZEN, but only add its world mesh to the scene.
     *
     * @param  zenFile  Uppercase ZEN-File name, e.g. "OLDWORLD.ZEN".
     *
     * @return Root of the created scene.
     */
    bs::HSceneObject loadWorldMeshFromZEN(const bs::String& zenFile);
  
    bs::HSceneObject getStartpoint()
    {
      return mStartpoint;
    }

  private:
    bs::HSceneObject mStartpoint;
    class VobImporter;
    VobImporter* mVobImporter;

    struct OriginalZen
    {
      bs::String fileName;
      ZenLoad::oCWorldData vobTree;
      ZenLoad::PackedMesh worldMesh;
    };

    bool importZEN(const bs::String& zenFile, OriginalZen& result);
    bs::HSceneObject importWorldMesh(const OriginalZen& zen);
    void importVobs(bs::HSceneObject sceneRoot, HGameWorld gameWorld, const OriginalZen& zen);
    void importWaynet(bs::HSceneObject sceneRoot, const OriginalZen& zen);
    void walkVobTree(bs::HSceneObject bsfParent, HGameWorld gameWorld,
                          const ZenLoad::zCVobData& zenParent);
  };

  class ZenImporter::VobImporter
  {
  public:
    VobImporter() = default;
    ~VobImporter() = default;

    /**
     * Imports a single vob and creates a bs:f object as similar as possible.
     *
     * @param  vob        Information from importing the zen-file.
     * @param  bsfParent  Parent game object.
     * @param  gameWorld  World to create the object in.
     *
     * @return Scene object modeled after the vob
     */
    bs::HSceneObject importSingleVob(ZenImporter& zenImporter, const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                     HGameWorld gameWorld);

  private:
    bs::Matrix4 convertMatrix(const ZMath::Matrix& m);
    bs::HSceneObject import_zCVob(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                       HGameWorld gameWorld);
    bs::HSceneObject import_zCVobLight(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject bsfParent, HGameWorld gameWorld);
    bs::HSceneObject import_zCVobStartpoint(ZenImporter& zenImporter, const ZenLoad::zCVobData& vob,
                                                 bs::HSceneObject bsfParent, HGameWorld gameWorld);
    bs::HSceneObject import_zCVobSpot(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                           HGameWorld gameWorld);
    bs::HSceneObject import_oCItem(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                        HGameWorld gameWorld);
    bs::HSceneObject import_zCVobSound(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject bsfParent, HGameWorld gameWorld);
    bs::HSceneObject import_zCVobAnimate(const ZenLoad::zCVobData& vob,
                                              bs::HSceneObject bsfParent, HGameWorld gameWorld);
    bs::HSceneObject import_oCMobInter(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject bsfParent, HGameWorld gameWorld);
    bs::HSceneObject import_oCMobContainer(const ZenLoad::zCVobData& vob,
                                                bs::HSceneObject bsfParent, HGameWorld gameWorld);
    bs::HSceneObject import_oCMobBed(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                          HGameWorld gameWorld);
    bs::HSceneObject import_oCMobDoor(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                           HGameWorld gameWorld);
    void addVisualTo(bs::HSceneObject sceneObject, const bs::String& visualName);
    void addCollisionTo(bs::HSceneObject sceneObject);
    bs::Transform transformFromVob(const ZenLoad::zCVobData& vob);

  };

}  // namespace REGoth
