#pragma once
#include "RTTIUtil.hpp"
#include <components/Focusable.hpp>

namespace REGoth
{
  class RTTI_Focusable : public bs::RTTIType<Focusable, bs::Component, RTTI_Focusable>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_PLAIN(mText, 0)
    BS_RTTI_MEMBER_PLAIN(mHeightOffset, 1)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Focusable()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Focusable)
  };
}  // namespace REGoth
