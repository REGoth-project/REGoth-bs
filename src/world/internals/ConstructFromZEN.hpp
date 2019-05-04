/** \file
 */

#pragma once

#include <BsPrerequisites.h>

namespace REGoth
{
  namespace Internals
  {
    /**
     * This function will load the given zenFile from the virtual file system
     * and fully convert it into a bs::f scene.
     *
     * @param  zenFile  Uppercase ZEN-file name, e.g. "OLDWORLD.ZEN".
     *
     * @return Root of the created scene.
     */
    bs::HSceneObject constructFromZEN(bs::HSceneObject root, const bs::String& zenFile);

    /**
     * Will load the given ZEN, but only add its world mesh to the scene.
     *
     * @param  zenFile  Uppercase ZEN-File name, e.g. "OLDWORLD.ZEN".
     *
     * @return Root of the created scene.
     */
    bs::HSceneObject loadWorldMeshFromZEN(const bs::String& zenFile);
  }
}
