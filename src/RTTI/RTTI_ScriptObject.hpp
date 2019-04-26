#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <scripting/ScriptObject.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_ScriptObject : public bs::RTTIType<ScriptObject, bs::IReflectable, RTTI_ScriptObject>
    {
      BS_BEGIN_RTTI_MEMBERS
      // TODO: Fill RTTI Members
      BS_END_RTTI_MEMBERS

    public:
      RTTI_ScriptObject()
      {
      }

      bs::SPtr<bs::IReflectable> newRTTIObject() override
      {
        return bs::bs_shared_ptr_new<ScriptObject>();
      }

      const bs::String& getRTTIName() override
      {
        static bs::String name = "ScriptObject";
        return name;
      }

      bs::UINT32 getRTTIId() override
      {
        return TID_REGOTH_ScriptObject;
      }
    };
  }  // namespace Scripting
}  // namespace REGoth
