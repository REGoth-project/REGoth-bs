/** \file
 */

#pragma once

#include <BsPrerequisites.h>

namespace ZenLoad
{
  struct zCVobData;
};

namespace REGoth
{
  namespace World
  {
    /**
     * Imports a single vob and creates a bs:f object as similar as possible.
     *
     * @param  vob  Information from importing the zen-file.
     *
     * @return Scene object modeled after the vob
     */
    bs::HSceneObject importSingleVob(const ZenLoad::zCVobData& vob);
  }  // namespace Worlds
}  // namespace REGoth
