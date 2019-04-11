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
      ScriptObject& obj = mScriptObjects.createScriptObject();

      const ScriptObject& classTemplate = mClassTemplates.getClassTemplate(objectClass);

      obj.functionPointers = classTemplate.functionPointers;
      obj.floats           = classTemplate.floats;
      obj.ints             = classTemplate.ints;
      obj.strings          = classTemplate.strings;

      return obj.handle;
    }

  }  // namespace Scripting
}  // namespace REGoth
