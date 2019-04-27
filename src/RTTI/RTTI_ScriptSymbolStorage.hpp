#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
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

      bs::SPtr<bs::IReflectable> newRTTIObject() override
      {
        return bs::bs_shared_ptr_new<ScriptSymbolStorage>();
      }

      const bs::String& getRTTIName() override
      {
        static bs::String name = "ScriptSymbolStorage";
        return name;
      }

      bs::UINT32 getRTTIId() override
      {
        return TID_REGOTH_ScriptSymbolStorage;
      }
    };
  }  // namespace Scripting
  // namespace Scripting
}  // namespace REGoth
