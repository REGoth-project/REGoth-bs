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
     * Holds the list of all created symbols and their data.
     *
     * This is the only place where symbols should created and have
     * their types and names be set.
     */
    class ScriptSymbolStorage
    {
    public:
      ScriptSymbolStorage() = default;

      /**
       * Appends a symbol of the given type to the storage.
       *
       * @tparam T     Type of the symbol to create and append.
       * @param  name  Name of the symbol to append.
       *
       * @return Index of the created symbol.
       */
      template <typename T>
      SymbolIndex appendSymbol(const bs::String& name)
      {
        bs::UPtr<SymbolBase> symbol = bs::bs_unique_ptr<SymbolBase>(new T());
        mStorage.emplace_back(std::move(symbol));
        mStorage.back()->name = name;
        mStorage.back()->type = T::TYPE;

        if (mStorage.size() >= SYMBOL_INDEX_MAX)
        {
          using namespace bs;
          BS_EXCEPT(InvalidStateException, "Symbol Index limit reached!");
        }

        SymbolIndex index = (SymbolIndex)(mStorage.size() - 1);

        mSymbolsByName[name] = index;

        return index;
      }

      /**
       * Looks up the symbol at the given index.
       *
       * This will also do sanity checks on whether the given index is pointing to
       * a valid object and throws if not.
       *
       * HOWEVER, this will not do any type checking, and you should only up-cast the
       * resulting reference if you know what you are doing.
       *
       * Use the type-save variant getSymbol() instead!
       *
       * @param  index  Index of the symbol to look up.
       *
       * @return Reference to the symbol with the given index.
       */
      SymbolBase& getSymbolBase(SymbolIndex index)
      {
        throwOnInvalidSymbol(index);

        return getTypedSymbolReference<SymbolBase>(index);
      }

      /**
       * Looks up the symbol at the given index.
       *
       * This will also do sanity checks on whether the given index is pointing to
       * a valid object of the correct type. Throws if not.
       *
       * @tparam T      Type of symbol to look up.
       * @param  index  Index of the symbol to look up.
       *
       * @return Reference to the symbol with the given index.
       */
      template <class T>
      T& getSymbol(SymbolIndex index)
      {
        throwOnInvalidSymbol(index);
        throwOnMismatchingType<T>(*mStorage[index]);

        return getTypedSymbolReference<T>(index);
      }

      /**
       * Looks up the symbol with the given name.
       *
       * This will also do sanity checks on whether the given name is referring
       * to an existing symbol. Throws if not.
       *
       * @tparam T      Type of symbol to look up.
       * @param  name   Name of the symbol to look up.
       *
       * @return Reference to the symbol with the given name.
       */
      template <class T>
      T& getSymbol(const bs::String& name)
      {
        SymbolIndex index = findIndexBySymbolName(name);
        throwOnInvalidSymbol(index);
        throwOnMismatchingType<T>(*mStorage[index]);

        return getTypedSymbolReference<T>(index);
      }

      /**
       * Looks up the type of the symbol with the given index.
       *
       * This will also do sanity checks on whether the given index is pointing to
       * a valid object of the correct type. Throws if not.
       *
       * @param  index  Index of the symbol to lookup the type from.
       *
       * @return Type of the symbol with the given index.
       */
      SymbolType getSymbolType(SymbolIndex index)
      {
        throwOnInvalidSymbol(index);

        return mStorage[index]->type;
      }

      /**
       * Looks up the type of the symbol with the given name.
       *
       * This will also do sanity checks on whether the given name is referring to
       * a valid object of the correct type. Throws if not.
       *
       * @param  index  Index of the symbol to lookup the type from.
       *
       * @return Type of the symbol with the given name.
       */
      SymbolType getSymbolType(const bs::String& name)
      {
        SymbolIndex index = findIndexBySymbolName(name);
        throwOnInvalidSymbol(index);

        return mStorage[index]->type;
      }

      /**
       * @return Whether a symbol with the given name exists.
       */
      bool hasSymbolWithName(const bs::String& name)
      {
        return mSymbolsByName.find(name) != mSymbolsByName.end();
      }

      /**
       * Tries to find the index of the symbol with the given name.
       *
       * Throws if no such symbol exists.
       *
       * @param  name  Name of the symbol to look for.
       *
       * @return Index of the symbol with the given name.
       */
      SymbolIndex findIndexBySymbolName(const bs::String& name)
      {
        auto it = mSymbolsByName.find(name);

        if (it == mSymbolsByName.end())
        {
          using namespace bs;
          BS_EXCEPT(InvalidStateException, "Symbol with name " + name + " does not exist!");
        }

        return it->second;
      }

    private:
      /**
       * @return The symbol at the given index cast to the passed type.
       */
      template <class T>
      T& getTypedSymbolReference(SymbolIndex index)
      {
        return *(T*)(mStorage[index].get());
      }

      template <class T>
      void throwOnMismatchingType(const SymbolBase& symbol)
      {
        if (T::TYPE != symbol.type)
        {
          using namespace bs;
          BS_EXCEPT(InvalidStateException, "Symbol Index limit reached!");
        }
      }

      void throwOnInvalidSymbol(SymbolIndex index)
      {
        using namespace bs;

        if (index >= mStorage.size())
        {
          BS_EXCEPT(InvalidStateException, "Symbol Index out of range!");
        }

        if (!mStorage[index])
        {
          BS_EXCEPT(InvalidStateException,
                    "Symbol index " + bs::toString(index) + " pointing to NULL!");
        }
      }

      bs::Vector<bs::UPtr<SymbolBase>> mStorage;
      bs::Map<bs::String, SymbolIndex> mSymbolsByName;
    };

    /**
     * Writes information about the given symbol via gDebug().
     */
    void debugLogSymbol(const SymbolBase& symbol);

  }  // namespace Scripting
}  // namespace REGoth
