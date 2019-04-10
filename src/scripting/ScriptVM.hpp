#pragma once
#include "ScriptObjects.hpp"
#include "ScriptSymbols.hpp"
#include <BsPrerequisites.h>

namespace REGoth
{
  namespace Scripting
  {
    class ScriptVM
    {
    public:
      ScriptVM();

    protected:
      /**
       * Get a list of all symbols and move them into the symbol storage vector.
       */
      virtual void fillSymbolStorage() = 0;

    protected:
      ScriptSymbolStorage mScriptSymbols;
      ScriptObjectStorage mScriptObjects;
    };
  }  // namespace Scripting
}  // namespace REGoth
