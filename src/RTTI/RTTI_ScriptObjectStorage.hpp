#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Reflection/BsRTTIType.h>
#include <scripting/ScriptObjectStorage.hpp>


namespace REGoth
{
  namespace Scripting
  {
    class RTTI_ScriptObjectStorage
        : public bs::RTTIType<ScriptObjectStorage, bs::IReflectable, RTTI_ScriptObjectStorage>
    {
      using UINT32 = bs::UINT32;

      BS_BEGIN_RTTI_MEMBERS
      // BS_RTTI_MEMBER_PLAIN(mObjects, 0) // FIXME: Maps with reflected objects not working?
      BS_RTTI_MEMBER_PLAIN(mNextHandle, 1)
      BS_END_RTTI_MEMBERS

    public:
      RTTI_ScriptObjectStorage()
      {
      }

      bs::SPtr<bs::IReflectable> newRTTIObject() override
      {
        return bs::bs_shared_ptr_new<ScriptObjectStorage>();
      }

      const bs::String& getRTTIName() override
      {
        static bs::String name = "ScriptObjectStorage";
        return name;
      }

      bs::UINT32 getRTTIId() override
      {
        return TID_REGOTH_ScriptObjectStorage;
      }
    };

  }  // namespace Scripting
}  // namespace REGoth
