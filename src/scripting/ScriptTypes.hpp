#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * Index of a symbol into the symbol storage.
     */
    typedef bs::UINT32 SymbolIndex;
    enum : SymbolIndex
    {
      SYMBOL_INDEX_MAX = UINT32_MAX
    };

    enum : SymbolIndex
    {
      SYMBOL_INDEX_INVALID = UINT32_MAX
    };

    enum class SymbolType
    {
      Float,
      Int,
      String,
      Class,
      ScriptFunction,
      ExternalFunction,
      Prototype,
      Instance,
      Unsupported,
    };

    /**
     * Script objects will get a simple increasing number as a handle.
     * No number can be used twice. An invalid handle will get the number 0.
     */
    typedef bs::UINT32 ScriptObjectHandle;

    enum : ScriptObjectHandle
    {
      SCRIPT_OBJECT_HANDLE_INVALID = 0
    };

    struct ScriptInts
    {
      bs::Vector<bs::INT32> values;
    };

    struct ScriptFloats
    {
      bs::Vector<float> values;
    };

    struct ScriptStrings
    {
      bs::Vector<bs::String> values;
    };

  }  // namespace Scripting
}  // namespace REGoth
