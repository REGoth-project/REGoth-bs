/** \file
 */

#pragma once

#include <BsPrerequisites.h>

namespace ZenLoad
{
  struct zCVobData;
}

namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  namespace Internals
  {
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
  }  // namespace Worlds
}  // namespace REGoth
