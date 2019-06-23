#pragma once

#include "RTTIUtil.hpp"
#include <scripting/ScriptObject.hpp>

namespace REGoth
{
  namespace Scripting
  {
    using UINT32 = bs::UINT32;

    class RTTI_ScriptObject : public bs::RTTIType<ScriptObject, bs::IReflectable, RTTI_ScriptObject>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(className, 0)
      BS_RTTI_MEMBER_PLAIN(handle, 1)
      BS_RTTI_MEMBER_PLAIN(ints, 2)
      BS_RTTI_MEMBER_PLAIN(floats, 3)
      BS_RTTI_MEMBER_PLAIN(strings, 4)
      BS_RTTI_MEMBER_PLAIN(functionPointers, 5)
      BS_RTTI_MEMBER_PLAIN(instanceName, 6)
      BS_END_RTTI_MEMBERS

    public:
      RTTI_ScriptObject()
      {
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(ScriptObject)
    };
  }  // namespace Scripting
}  // namespace REGoth
