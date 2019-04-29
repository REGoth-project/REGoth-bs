#pragma once

#include "RTTIUtil.hpp"
#include <visual/RaycastShadowSampler.hpp>

#include <Mesh/BsMesh.h>

namespace REGoth
{
  class RTTI_RaycastShadowSampler
      : public bs::RTTIType<RaycastShadowSampler, ShadowSampler, RTTI_RaycastShadowSampler>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mMesh, 0)
    BS_RTTI_MEMBER_REFL(mCollider, 1)
    BS_RTTI_MEMBER_PLAIN_ARRAY(mBrightnessPerVertex, 2)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_RaycastShadowSampler()
    {
    }

    virtual void onDeserializationEnded(bs::IReflectable* obj,
                                        bs::SerializationContext* context) override
    {
      auto sampler = reinterpret_cast<RaycastShadowSampler*>(obj);

      // Make sure to set the face accessor (which is a lambda and cannot be (de)serialized)
      sampler->mFaceAccessor =
          RaycastShadowSampler::getFaceAccessor(*(sampler->mMesh->getCachedData()));
    }

    //REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(RaycastShadowSampler)

    bs::SPtr<bs::IReflectable> newRTTIObject() override
    {
      return RaycastShadowSampler::createEmpty();
    }

    const bs::String& getRTTIName() override
    {
      static bs::String name = "RaycastShadowSampler";
      return name;
    }

    bs::UINT32 getRTTIId() override
    {
      return TID_REGOTH_RaycastShadowSampler;
    }
  };
}  // namespace REGoth