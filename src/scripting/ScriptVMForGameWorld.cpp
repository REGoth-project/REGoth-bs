#include "ScriptVMForGameWorld.hpp"
#include <RTTI/RTTI_ScriptVMForGameWorld.hpp>
#include <daedalus/DATFile.h>

namespace REGoth
{
  namespace Scripting
  {
    ScriptVMForGameWorld::ScriptVMForGameWorld(HGameWorld gameWorld,
                                               const bs::Vector<bs::UINT8>& datFileData)
        : DaedalusVMForGameWorld(gameWorld, datFileData)
    {
    }


    REGOTH_DEFINE_RTTI(ScriptVMForGameWorld);
  }  // namespace Scripting

}  // namespace REGoth
