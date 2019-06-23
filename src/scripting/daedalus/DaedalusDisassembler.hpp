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
                                 const ScriptSymbolStorage& symbols, const bs::String& lhs = "a",
                                 const bs::String& rhs = "b", const bs::String& res = "");

    /**
     * Makes a string with different characters for every call-depth.
     *
     * For example, these could be the strings generated for different depths:
     *
     *     |
     *     |;
     *     |;.
     *
     * This is so you can simply read off the call depth from the disassembly output.
     */
    bs::String makeCallDepthString(bs::UINT32 callDepth);

  }  // namespace Scripting
}  // namespace REGoth
