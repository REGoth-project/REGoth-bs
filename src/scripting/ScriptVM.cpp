#include "ScriptVM.hpp"
#include "ScriptSymbolQueries.hpp"

namespace REGoth
{
  namespace Scripting
  {
    void ScriptVM::initialize()
    {
      fillSymbolStorage();

      mClassTemplates.createClassTemplates(mScriptSymbols);
    }

    ScriptObjectHandle ScriptVM::instanciateBlankObjectOfClass(const bs::String& objectClass)
    {
      ScriptObjectHandle b;

      return b;
    }

  }  // namespace Scripting
}  // namespace REGoth
