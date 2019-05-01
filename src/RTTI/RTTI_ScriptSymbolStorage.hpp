#pragma once

#include "RTTIUtil.hpp"
#include <scripting/ScriptSymbolStorage.hpp>

namespace REGoth
{
  namespace Scripting
  {
    using UINT32 = bs::UINT32;

    class RTTI_ScriptSymbolStorage
        : public bs::RTTIType<ScriptSymbolStorage, bs::IReflectable, RTTI_ScriptSymbolStorage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(mSymbolsByName, 1)
      BS_RTTI_MEMBER_REFLPTR_ARRAY(mStorage, 2)
      BS_END_RTTI_MEMBERS

    public:
      RTTI_ScriptSymbolStorage()
      {
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(ScriptSymbolStorage)
    };
  }  // namespace Scripting
  // namespace Scripting
}  // namespace REGoth
