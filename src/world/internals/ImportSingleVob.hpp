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
  namespace Internals
  {
    /**
     * Imports a single vob and creates a bs:f object as similar as possible.
     *
     * @param  vob       Information from importing the zen-file.
     * @param  parentSO  Parent scene object of the newly created one.
     *
     * @return Scene object modeled after the vob
     */
    bs::HSceneObject importSingleVob(const ZenLoad::zCVobData& vob, bs::HSceneObject parentSO);
  }  // namespace Worlds
}  // namespace REGoth
