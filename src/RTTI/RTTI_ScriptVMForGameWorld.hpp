#pragma once

#include "RTTIUtil.hpp"
#include <scripting/ScriptVMForGameWorld.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_ScriptVMForGameWorld
        : public bs::RTTIType<ScriptVMForGameWorld, DaedalusVMForGameWorld,
                              RTTI_ScriptVMForGameWorld>
    {
      BS_BEGIN_RTTI_MEMBERS
      // TODO: Fill RTTI Members
      BS_END_RTTI_MEMBERS

    public:
      RTTI_ScriptVMForGameWorld()
      {
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(ScriptVMForGameWorld)
    };
  }  // namespace Scripting
  // namespace Scripting
}  // namespace REGoth
