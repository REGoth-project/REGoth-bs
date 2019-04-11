#include "DaedalusVMWithExternals.hpp"

namespace REGoth
{
  namespace Scripting
  {
    DaedalusVMWithExternals::DaedalusVMWithExternals(const Daedalus::DATFile& datFile)
        : DaedalusVM(datFile)
    {
    }

    void DaedalusVMWithExternals::registerAllExternals()
    {
      using This = DaedalusVMWithExternals;

      registerExternal("PRINT", (externalCallback)&This::external_Print);
      registerExternal("HLP_RANDOM", (externalCallback)&This::external_HLP_Random);
      registerExternal("INTTOSTRING", (externalCallback)&This::external_IntToString);
      registerExternal("CONCATSTRINGS", (externalCallback)&This::external_ConcatStrings);
    }

    void DaedalusVMWithExternals::external_Print()
    {
      print(popStringValue());
    }

    void DaedalusVMWithExternals::external_HLP_Random()
    {
      mStack.pushInt(HLP_Random(popIntValue()));
    }

    void DaedalusVMWithExternals::external_IntToString()
    {
      mStack.pushString(IntToString(popIntValue()));
    }

    void DaedalusVMWithExternals::external_ConcatStrings() {
      bs::String b = popStringValue();
      bs::String a = popStringValue();

      mStack.pushString(ConcatStrings(a, b));
    }

    void DaedalusVMWithExternals::script_PrintPlus(const bs::String& text)
    {
      mStack.pushString(text);
      executeScriptFunction("PrintPlus");
    }
  }  // namespace Scripting
}  // namespace REGoth
