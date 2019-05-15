/**\file
 */
#pragma once
#include <BsPrerequisites.h>
#include "daedalus/DaedalusVMForGameWorld.hpp"

namespace REGoth
{
  namespace Scripting
  {
    /**
     * This class implements all scripting related logic, as portable as possible.
     * It inherits from a scripting backend - most likely the DaedalusVM - and implements
     * the script externals, hopefully without depending on daedalus specific details.
     */
    class ScriptVMForGameWorld : public DaedalusVMForGameWorld
    {
    public:
      // I don't quite like to reference the dat-file here... This will
      // be annoying when we switch to another scripting backend.
      ScriptVMForGameWorld(HGameWorld gameWorld, const Daedalus::DATFile& datFile);


    protected:
    public:
      REGOTH_DECLARE_RTTI(ScriptVMForGameWorld);

    public:                              // FIXME: RTTI protected
      ScriptVMForGameWorld() = default;  // For RTTI
    };
  }  // namespace Scripting
}  // namespace REGoth
