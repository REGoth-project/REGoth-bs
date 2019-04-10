#pragma once
#include "ScriptSymbols.hpp"
#include <BsPrerequisites.h>

namespace REGoth
{
  namespace Scripting
  {
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
  }
}
