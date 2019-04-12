/** \file
 */

#pragma once

#include <BsPrerequisites.h>

namespace REGoth
{
  namespace World
  {
    /**
     * This function will load the given zenFile from the virtual file system
     * and convert it into a bs::f scene.
     *
     * @param  zenFile  Uppercase Zen file name, e.g. "OLDWORLD.ZEN".
     *
     * @return Root of the created scene.
     */
    bs::HSceneObject constructFromZEN(const bs::String& zenFile);
  }
}
