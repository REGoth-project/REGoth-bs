#pragma once

#include "RTTIUtil.hpp"
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

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(DaedalusVMForGameWorld)
    };
  }  // namespace Scripting

}  // namespace REGoth
