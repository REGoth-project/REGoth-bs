#pragma once
#include "ScriptObjects.hpp"
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

    struct SymbolBase
    {
      /**
       * Name of this symbol
       */
      bs::String name;

      /**
       * Which kind of symbol this is. Needs to be stored here since we only know
       * this base class at some points and we would like to up-cast.
       */
      SymbolType type;

      /**
       * This symbols *parent* symbol. For a classes member variable, this would be the
       * symbol of the actual class.
       *
       * If no parent is available, it will be set to SYMBOL_INDEX_INVALID.
       */
      SymbolIndex parent = SYMBOL_INDEX_INVALID;

      /**
       * If this is a class var, then the values held inside should be ignored
       * and rather taken from the script object set in *Current Instance*.
       */
      bool isClassVar;

      /**
       * If this is true, the variable should not be reset to whatever was in the DAT-file
       * or savegame after the game was (re)loaded. It should rather keep the value it had
       * during the previous run. This means the value stays set, *even after loading a
       * savegame*!
       *
       * Also these are not saved in a savegame.
       *
       * @note In the original daedalus this is called `const`, but those are not really const.
       */
      bool isKeptAfterLoad;
    };

    /**
     * Simple global int (array).
     */
    struct SymbolInt : SymbolBase
    {
      static constexpr SymbolType TYPE = SymbolType::Int;

      bs::Vector<bs::INT32> values;
    };

    /**
     * Simple global float (array).
     */
    struct SymbolFloat : SymbolBase
    {
      static constexpr SymbolType TYPE = SymbolType::Float;

      bs::Vector<float> values;
    };

    /**
     * Simple global string (array).
     */
    struct SymbolString : SymbolBase
    {
      static constexpr SymbolType TYPE = SymbolType::String;

      bs::Vector<bs::String> values;
    };

    /**
     * This symbol kind *would* store the class offset in the original engine.
     * We don't do that here however, so it's left empty.
     */
    struct SymbolClass : SymbolBase
    {
      static constexpr SymbolType TYPE = SymbolType::Class;
    };

    /**
     * Describes a script function. Note that - other than in the original - we distinguish
     * between script- and external functions on symbol level.
     */
    struct SymbolScriptFunction : SymbolBase
    {
      static constexpr SymbolType TYPE = SymbolType::ScriptFunction;

      /**
       * Bytecode address of where this function starts.
       */
      bs::UINT32 address;
    };

    /**
     * Describes an external function. Note that - other than in the original - we distinguish
     * between script- and external functions on symbol level.
     */
    struct SymbolExternalFunction : SymbolBase
    {
      static constexpr SymbolType TYPE = SymbolType::ExternalFunction;

      /**
       * Since the mapping of external functions to native ones is done at VM-level, this
       * struct is left empty.
       */
    };

    /**
     * Holds information about the prototype of a script object. A prototype is
     * something like a mix of an abstract class and a constructor. You cannot
     * create an instance of it, but it runs before a object inheriting from the
     * prototype gets created
     */
    struct SymbolPrototype : SymbolBase
    {
      static constexpr SymbolType TYPE = SymbolType::Prototype;

      /**
       * Bytecode address of where the constructor function starts.
       */
      bs::UINT32 constructorAddress;
    };

    /**
     * Instances are script objects which have been set up in a certain way.
     * This symbol will hold a reference to the instance constructor.
     *
     * They also seem to be the only ones which need to be able to store
     * references to script objects...
     *
     * TODO: What is the address set to at symbols like `self` and `other`?
     */
    struct SymbolInstance : SymbolBase
    {
      static constexpr SymbolType TYPE = SymbolType::Instance;

      /**
       * Bytecode address of where the constructor function starts.
       */
      bs::UINT32 constructorAddress;

      /**
       * Instance referenced by this symbol.
       */
      ScriptObjectHandle instance = SCRIPT_OBJECT_HANDLE_INVALID;
    };

    /**
     * This kind of symbol does not exist in the original gothic, but we need
     * it so we can add something to the symbol storage in case we could not
     * load a symbol. There has to be a dummy symbol there to make the indices
     * match up.
     */
    struct SymbolUnsupported : SymbolBase
    {
      static constexpr SymbolType TYPE = SymbolType::Unsupported;
    };

    /**
     * Writes information about the given symbol via gDebug().
     */
    void debugLogSymbol(const SymbolBase& symbol);

  }  // namespace Scripting
}  // namespace REGoth
