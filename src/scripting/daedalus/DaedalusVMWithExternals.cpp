#include "DaedalusVMWithExternals.hpp"

namespace REGoth
{
  namespace Scripting
  {
    DaedalusVMWithExternals::DaedalusVMWithExternals(const Daedalus::DATFile& datFile)
        : DaedalusVM(datFile)
    {
    }

    void DaedalusVMWithExternals::script_PrintPlus(const bs::String& text)
    {
      mStack.pushString(text);
      executeScriptFunction("PrintPlus");
    }

  }  // namespace Scripting
}  // namespace REGoth
