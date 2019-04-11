#include "ScriptVMInterface.hpp"

namespace REGoth
{
  namespace Scripting
  {
    ScriptVMInterface::ScriptVMInterface(const Daedalus::DATFile& datFile)
        : DaedalusVMWithExternals(datFile)
    {
    }

    void ScriptVMInterface::print(const bs::String& text)
    {
      bs::gDebug().logDebug("[ScriptVMInterface] [Print] " + text);
    }

    bs::INT32 ScriptVMInterface::HLP_Random(bs::INT32 max)
    {
      return rand() % max;
    }

    bs::String ScriptVMInterface::IntToString(bs::INT32 i)
    {
      return bs::toString(i);
    }

    bs::String ScriptVMInterface::ConcatStrings(const bs::String& a, const bs::String& b)
    {
      return a + b;
    }
  }  // namespace Scripting
}  // namespace REGoth
