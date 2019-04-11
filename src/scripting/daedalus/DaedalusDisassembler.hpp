/**\file
 */
#pragma once
#include <BsPrerequisites.h>
#include <daedalus/DATFile.h>

namespace REGoth
{
  namespace Scripting
  {
    class ScriptSymbolStorage;

    /**
     * Disassembles the given opcode.
     *
     * @return Disassembly as text.
     */
    bs::String disassembleOpcode(const Daedalus::PARStackOpCode& opcode,
                                 const ScriptSymbolStorage& symbols);
  }  // namespace Scripting
}  // namespace REGoth
