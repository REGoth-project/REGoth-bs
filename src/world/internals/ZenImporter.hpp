/** \file
 */

#pragma once

#include <BsPrerequisites.h>
#include "VobImporter.hpp"

namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  class ZenImporter
  {
  public:
    ZenImporter() = default;
    ~ZenImporter() = default;

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
  
  private:
    VobImporter mVobImporter;

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

}  // namespace REGoth
