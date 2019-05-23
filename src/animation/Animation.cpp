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

    if (then <= now)
    {
      bs::Vector3 positionThen = curve.evaluate(then, Clamp);
      bs::Vector3 positionNow  = curve.evaluate(now, Clamp);

      return positionNow - positionThen;
    }
    else /* now < then */
    {
      return bs::Vector3(bs::BsZero);

      // Wrapped, need to also take care of the time spent before the wrap
      // bs::Vector3 atEnd = curve.evaluate(curve.getLength(), Clamp);

      // bs::Vector3 positionThen = curve.evaluate(then, Clamp);
      // bs::Vector3 positionNow  = curve.evaluate(now, Clamp);

      // bs::Vector3 toEnd = atEnd - positionThen;
      // bs::Vector3 fromStart = positionNow;

      // return fromStart;
      // return toEnd + fromStart;
    }
  }
}  // namespace REGoth
