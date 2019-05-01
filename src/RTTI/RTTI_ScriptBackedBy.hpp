#pragma once

#include "RTTIUtil.hpp"
#include <components/ScriptBackedBy.hpp>

namespace REGoth
{
  class RTTI_ScriptBackedBy
      : public bs::RTTIType<ScriptBackedBy, NeedsGameWorld, RTTI_ScriptBackedBy>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_PLAIN(mScriptClassName, 0)
    BS_RTTI_MEMBER_PLAIN(mScriptInstance, 1)
    BS_RTTI_MEMBER_PLAIN(mScriptObject, 2)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_ScriptBackedBy()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(ScriptBackedBy)
  };

}  // namespace REGoth
