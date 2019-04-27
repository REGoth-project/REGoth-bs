#include "ScriptVMForGameWorld.hpp"
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
  }  // namespace Scripting

}  // namespace REGoth
