#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <scripting/daedalus/DaedalusVMForGameWorld.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_DaedalusVMForGameWorld
        : public bs::RTTIType<DaedalusVMForGameWorld, DaedalusVM, RTTI_DaedalusVMForGameWorld>
    {
      BS_BEGIN_RTTI_MEMBERS
      // TODO: Fill RTTI Members
      BS_END_RTTI_MEMBERS

    public:
      RTTI_DaedalusVMForGameWorld()
      {
      }

      bs::SPtr<bs::IReflectable> newRTTIObject() override
      {
        return bs::bs_shared_ptr_new<DaedalusVMForGameWorld>();
      }

      const bs::String& getRTTIName() override
      {
        static bs::String name = "DaedalusVMForGameWorld";
        return name;
      }

      bs::UINT32 getRTTIId() override
      {
        return TID_REGOTH_DaedalusVMForGameWorld;
      }
    };
  }  // namespace Scripting

}  // namespace REGoth
