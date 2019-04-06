#include "Animation.hpp"
#include <Animation/BsAnimationClip.h>
#include <Animation/BsAnimation.h>

using namespace REGoth;

bs::Vector3 Animation::getRootMotionSince(bs::HAnimationClip clip, float then, float now)
{
  using namespace bs;

  if (!clip->getRootMotion())
  {
    // No root motion wanted
    return bs::Vector3(bs::BsZero);
  }

  // TODO: Look after rotation too
  const auto& curve = clip->getRootMotion()->position;
  const bool loop = false;

  if (then <= now)
  {
    bs::Vector3 positionThen = curve.evaluate(then, loop);
    bs::Vector3 positionNow = curve.evaluate(now, loop);

    return positionNow - positionThen;
  }
  else
  {
    // Wrapped, need to also take care of the time spent before the wrap
    bs::Vector3 atEnd = curve.evaluate(curve.getLength(), loop);
    bs::Vector3 positionThen = curve.evaluate(then, loop);
    bs::Vector3 positionNow = curve.evaluate(now, loop);

    bs::Vector3 toEnd = atEnd - positionThen;

    return toEnd + positionNow;
  }
}
