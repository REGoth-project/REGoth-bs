#include "DATSymbolStorageLoader.hpp"
#include <daedalus/DATFile.h>
#include <scripting/ScriptSymbolStorage.hpp>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * Converts ZenLib DATFile-Symbols to REGoth Symbols.
     *
     * A Symbol from a ZenLib DAT is converted in 3 stages:
     *
     *  1. Guess the symbol type
     *  2. Create a matching script symbol instance in the storage
     *  3. Transfer information from the ZenLib-Symbol to the REGoth-Symbol.
     *
     * You will see quite and repetitive looking switch- and if/else-statements. This
     * could be made shorter using templates, but I decided not to for clarity.
     */
    class DATSymbolStorageLoader
    {
    public:
      DATSymbolStorageLoader(ScriptSymbolStorage& storage, const Daedalus::DATFile& datFile)
          : mDatFile(datFile)
          , mStorage(storage)
      {
      }

      /**
       * Converts all symbols from ZenLib to REGoth format
       */
      void loadFromDAT()
      {
        const Daedalus::PARSymTable& symTable = mDatFile.getSymTable();

        for (const Daedalus::PARSymbol& sym : symTable.symbols)
        {
          SymbolType type    = guessSymbolType(sym);
          SymbolIndex target = createSymbolOf(type, sym.name.c_str());
          transferSymbol(target, sym);

          // debugLogSymbol(mStorage.getSymbolBase(target));
        }
      }

    private:
      SymbolType guessSymbolType(const Daedalus::PARSymbol& sym)
      {
        uint32_t type  = sym.properties.elemProps.type;
        uint32_t flags = sym.properties.elemProps.flags;

        if (type == Daedalus::EParType_Int)
        {
          return SymbolType::Int;
        }
        else if (type == Daedalus::EParType_Float)
        {
          return SymbolType::Float;
        }
        else if (type == Daedalus::EParType_String)
        {
          return SymbolType::String;
        }
        else if (type == Daedalus::EParType_Class)
        {
          return SymbolType::Class;
        }
        else if (type == Daedalus::EParType_Instance)
        {
          return SymbolType::Instance;
        }
        else if (type == Daedalus::EParType_Prototype)
        {
          return SymbolType::Prototype;
        }
        else if (type == Daedalus::EParType_Func)
        {
          if (sym.properties.elemProps.flags & Daedalus::EParFlag_External)
          {
            return SymbolType::ExternalFunction;
          }
          else
          {
            return SymbolType::ScriptFunction;
          }
        }
        else
        {
          return SymbolType::Unsupported;
        }
      }

      SymbolIndex createSymbolOf(SymbolType type, const bs::String& name)
      {
        switch (type)
        {
          case SymbolType::Float:
            return mStorage.appendSymbol<SymbolFloat>(name);
          case SymbolType::Int:
            return mStorage.appendSymbol<SymbolInt>(name);
          case SymbolType::String:
            return mStorage.appendSymbol<SymbolString>(name);
          case SymbolType::Class:
            return mStorage.appendSymbol<SymbolClass>(name);
          case SymbolType::ScriptFunction:
            return mStorage.appendSymbol<SymbolScriptFunction>(name);
          case SymbolType::ExternalFunction:
            return mStorage.appendSymbol<SymbolExternalFunction>(name);
          case SymbolType::Prototype:
            return mStorage.appendSymbol<SymbolPrototype>(name);
          case SymbolType::Instance:
            return mStorage.appendSymbol<SymbolInstance>(name);
          case SymbolType::Unsupported:
          default:
            return mStorage.appendSymbol<SymbolUnsupported>(name);
        }
      }

      void transferSymbol(SymbolIndex target, const Daedalus::PARSymbol& source)
      {
        SymbolType type = mStorage.getSymbolType(target);

        if (type == SymbolType::Float)
        {
          auto& t = mStorage.getSymbol<SymbolFloat>(target);
          fill(t, source);
        }
        else if (type == SymbolType::Int)
        {
          auto& t = mStorage.getSymbol<SymbolInt>(target);
          fill(t, source);
        }
        else if (type == SymbolType::String)
        {
          auto& t = mStorage.getSymbol<SymbolString>(target);
          fill(t, source);
        }
        else if (type == SymbolType::Class)
        {
          auto& t = mStorage.getSymbol<SymbolClass>(target);
          fill(t, source);
        }
        else if (type == SymbolType::ScriptFunction)
        {
          auto& t = mStorage.getSymbol<SymbolScriptFunction>(target);
          fill(t, source);
        }
        else if (type == SymbolType::ExternalFunction)
        {
          auto& t = mStorage.getSymbol<SymbolExternalFunction>(target);
          fill(t, source);
        }
        else if (type == SymbolType::Prototype)
        {
          auto& t = mStorage.getSymbol<SymbolPrototype>(target);
          fill(t, source);
        }
        else if (type == SymbolType::Instance)
        {
          auto& t = mStorage.getSymbol<SymbolInstance>(target);
          fill(t, source);
        }
        else /* if (type == SymbolType::Unsupported) */
        {
          auto& t = mStorage.getSymbol<SymbolUnsupported>(target);
          fill(t, source);
        }
      }

      void fillBase(SymbolBase& target, const Daedalus::PARSymbol& source)
      {
        target.isClassVar = source.properties.elemProps.flags & Daedalus::EParFlag_ClassVar;

        // Const as a bug and actually means "Keep this after load" in the original.
        target.isKeptAfterLoad = source.properties.elemProps.flags & Daedalus::EParFlag_Const;

        if (source.parent == 0xFFFFFFFF)
        {
          target.parent = SYMBOL_INDEX_INVALID;
        }
        else
        {
          target.parent = source.parent;
        }
      }

      void fill(SymbolInt& target, const Daedalus::PARSymbol& source)
      {
        fillBase(target, source);

        target.values.resize(source.properties.elemProps.count);
      }

      void fill(SymbolFloat& target, const Daedalus::PARSymbol& source)
      {
        fillBase(target, source);

        target.values.resize(source.properties.elemProps.count);
      }

      void fill(SymbolString& target, const Daedalus::PARSymbol& source)
      {
        fillBase(target, source);

        target.values.resize(source.properties.elemProps.count);
      }

      void fill(SymbolClass& target, const Daedalus::PARSymbol& source)
      {
        fillBase(target, source);

        // Empty - see SymbolClass documentation.
      }

      void fill(SymbolExternalFunction& target, const Daedalus::PARSymbol& source)
      {
        fillBase(target, source);

        // Empty - see SymbolExternalFunction documentation.
      }

      void fill(SymbolScriptFunction& target, const Daedalus::PARSymbol& source)
      {
        fillBase(target, source);

        target.address = source.address;
      }

      void fill(SymbolPrototype& target, const Daedalus::PARSymbol& source)
      {
        fillBase(target, source);

        target.constructorAddress = source.address;
      }

      void fill(SymbolInstance& target, const Daedalus::PARSymbol& source)
      {
        fillBase(target, source);

        target.constructorAddress = source.address;
      }

      void fill(SymbolUnsupported& target, const Daedalus::PARSymbol& source)
      {
        fillBase(target, source);
      }

      const Daedalus::DATFile& mDatFile;
      ScriptSymbolStorage& mStorage;
    };

    void convertDatToREGothSymbolStorage(ScriptSymbolStorage& storage,
                                         const Daedalus::DATFile& datFile)
    {
      DATSymbolStorageLoader loader(storage, datFile);

      loader.loadFromDAT();
    }
  }  // namespace Scripting
}  // namespace REGoth
