#include "ScriptVMInterface.hpp"

namespace REGoth
{
  namespace Scripting
  {
    static bs::SPtr<ScriptVMInterface> s_GameScript = nullptr;

    void loadGothicDAT(const Daedalus::DATFile& datFile)
    {
      s_GameScript = bs::bs_unique_ptr_new<ScriptVMInterface>(datFile);
      s_GameScript->initialize();
    }

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

  Scripting::ScriptVMInterface& gGameScript()
  {
    if (!Scripting::s_GameScript)
    {
      REGOTH_THROW(InvalidStateException, "Game-Script not available. Was loadGothicDAT() called?");
    }

    return *Scripting::s_GameScript;
  }

}  // namespace REGoth
