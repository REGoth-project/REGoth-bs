#pragma once
#include "RTTIUtil.hpp"
#include <components/StoryInformation.hpp>

namespace REGoth
{
  class RTTI_StoryInformation : public bs::RTTIType<StoryInformation, bs::Component, RTTI_StoryInformation>
  {

    BS_BEGIN_RTTI_MEMBERS
    // Explicitly Not serialized: mAllInfos
    BS_RTTI_MEMBER_PLAIN(mKnownInfos, 0)
    BS_RTTI_MEMBER_REFL(mSelf, 1)
    BS_RTTI_MEMBER_REFL(mGameWorld, 2)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_StoryInformation()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(StoryInformation)
  };
}  // namespace REGoth
