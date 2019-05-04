#include "ScriptVMForGameWorld.hpp"
#include <RTTI/RTTI_ScriptVMForGameWorld.hpp>
#include <daedalus/DATFile.h>

namespace REGoth
{
  namespace Scripting
  {
    ScriptVMForGameWorld::ScriptVMForGameWorld(HGameWorld gameWorld,
                                               const Daedalus::DATFile& datFile)
        : DaedalusVMForGameWorld(gameWorld, datFile)
    {
    }


    REGOTH_DEFINE_RTTI(ScriptVMForGameWorld);
  }  // namespace Scripting

}  // namespace REGoth
