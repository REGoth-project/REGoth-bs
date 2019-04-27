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

    template <typename T>
    struct ScriptValues
    {
      bs::Vector<T> values;
    };

    using ScriptInts = ScriptValues<bs::INT32>;
    using ScriptFloats = ScriptValues<float>;
    using ScriptStrings = ScriptValues<bs::String>;

  }  // namespace Scripting
}  // namespace REGoth
