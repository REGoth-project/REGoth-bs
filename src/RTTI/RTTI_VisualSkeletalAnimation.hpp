#pragma once

#include "RTTIUtil.hpp"
#include <components/VisualSkeletalAnimation.hpp>

namespace REGoth
{
  class RTTI_VisualSkeletalAnimation
      : public bs::RTTIType<VisualSkeletalAnimation, bs::Component, RTTI_VisualSkeletalAnimation>
  {
    using UINT32 = bs::UINT32;

    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mModelScript, 0)
    BS_RTTI_MEMBER_REFL(mMesh, 1)
    BS_RTTI_MEMBER_REFL_ARRAY(mSubObjects, 2)
    BS_RTTI_MEMBER_REFL(mSubRenderable, 3)
    BS_RTTI_MEMBER_REFL(mSubAnimation, 4)
    BS_RTTI_MEMBER_REFL(mSubNodeVisuals, 5)
    BS_RTTI_MEMBER_REFL(mRootMotionLastClip, 6)
    BS_RTTI_MEMBER_PLAIN(mRootMotionLastTime, 7)
    BS_RTTI_MEMBER_REFL(mPlayingMainAnimation, 8)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_VisualSkeletalAnimation()
    {
    }

    void onDeserializationEnded(bs::IReflectable* _obj, bs::SerializationContext* context) override
    {
      auto obj = static_cast<VisualSkeletalAnimation*>(_obj);

      // Might have saved an empty visual, which is indeed okay to do
      if (obj->mModelScript)
      {
        obj->createAnimationMap();
      }
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(VisualSkeletalAnimation)
  };

}  // namespace REGoth
