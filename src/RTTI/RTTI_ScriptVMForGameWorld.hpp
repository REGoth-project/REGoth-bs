#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <scripting/ScriptVMForGameWorld.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_ScriptVMForGameWorld
        : public bs::RTTIType<ScriptVMForGameWorld, DaedalusVMForGameWorld, RTTI_ScriptVMForGameWorld>
    {
      BS_BEGIN_RTTI_MEMBERS
      // TODO: Fill RTTI Members
      BS_END_RTTI_MEMBERS

    public:
      RTTI_ScriptVMForGameWorld()
      {
      }

      bs::SPtr<bs::IReflectable> newRTTIObject() override
      {
        return bs::bs_shared_ptr_new<ScriptVMForGameWorld>();
      }

      const bs::String& getRTTIName() override
      {
        static bs::String name = "ScriptVMForGameWorld";
        return name;
      }

      bs::UINT32 getRTTIId() override
      {
        return TID_REGOTH_ScriptVMForGameWorld;
      }
    };
  }
  // namespace Scripting
}  // namespace REGoth
