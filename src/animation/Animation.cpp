#include "Animation.hpp"
#include <Animation/BsAnimation.h>
#include <Animation/BsAnimationClip.h>

namespace REGoth
{
  bs::Vector3 AnimationState::getRootMotionSince(bs::HAnimationClip clip, float then, float now)
  {
    using namespace bs;

    if (!clip->getRootMotion())
    {
      // No root motion wanted
      return bs::Vector3(bs::BsZero);
    }

    // TODO: Look after rotation too
    const auto& curve = clip->getRootMotion()->position;

    enum : bool
    {
      Wrap  = true,
      Clamp = false
    };

    bs::Vector3 positionThen = curve.evaluate(then, Clamp);
    bs::Vector3 positionNow  = curve.evaluate(now, Clamp);

    return positionNow - positionThen;
  }
}  // namespace REGoth
