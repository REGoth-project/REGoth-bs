/**\file
 */

#pragma once
#include <BsPrerequisites.h>
#include <Math/BsVector3.h>

namespace REGoth
{
  namespace AnimationState
  {
    bs::Vector3 getRootMotionSince(bs::HAnimationClip clip, float then, float now);
  }
}
