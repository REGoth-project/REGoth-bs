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
      ScriptVMForGameWorld(HGameWorld gameWorld, const bs::Vector<bs::UINT8>& datFileData);

    protected:

    public:
      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(ScriptVMForGameWorld);

    protected:
      ScriptVMForGameWorld() = default;  // For RTTI
    };
  }  // namespace Scripting
}  // namespace REGoth
