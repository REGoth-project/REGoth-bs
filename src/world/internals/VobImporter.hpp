/** \file
 */

#pragma once

#include <BsPrerequisites.h>
#include <Math/BsMatrix4.h>
#include <zenload/zTypes.h>

namespace ZenLoad
{
  struct zCVobData;
}

namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  class VobImporter
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
    bs::HSceneObject importSingleVob(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                     HGameWorld gameWorld);

    //FIXME: Come up with a better way, possibly by reworking the importer into one, so there only need to be 2 getters instead of 3
    bs::HSceneObject getWorldStartPoint()
    {
      return mStartpoint;
    }

  private:
    bs::HSceneObject mStartpoint;

    bs::Matrix4 convertMatrix(const ZMath::Matrix& m);
    bs::HSceneObject import_zCVob(const ZenLoad::zCVobData& vob, bs::HSceneObject bsfParent,
                                       HGameWorld gameWorld);
    bs::HSceneObject import_zCVobLight(const ZenLoad::zCVobData& vob,
                                            bs::HSceneObject bsfParent, HGameWorld gameWorld);
    bs::HSceneObject import_zCVobStartpoint(const ZenLoad::zCVobData& vob,
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
