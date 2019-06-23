#include "ScriptSymbolQueries.hpp"
#include "ScriptSymbolStorage.hpp"
#include "ScriptSymbols.hpp"

namespace REGoth
{
  namespace Scripting
  {
    namespace Queries
    {
      bs::Vector<SymbolIndex> findAllClasses(const ScriptSymbolStorage& storage)
      {
        auto isClass = [](const SymbolBase& s) {
          return s.type == SymbolType::Class;  //
        };

        return storage.query(isClass);
      }

      bs::Vector<SymbolIndex> findAllInstancesOfClass(const ScriptSymbolStorage& storage,
                                                      const bs::String& className)
      {
        SymbolClass& classSymbol = storage.getSymbol<SymbolClass>(className);

        auto isInstanceOfClass = [&](const SymbolBase& s) {
          if (s.type != SymbolType::Instance) return false;
          if (s.parent == SYMBOL_INDEX_INVALID) return false;

          if (s.parent != classSymbol.index)
          {
            // If the instances parent is not the class itself, it could be a prototype.
            // The prototype then has to have the class as parent.
            SymbolBase& parent = storage.getSymbolBase(s.parent);

            if (parent.type != SymbolType::Prototype) return false;
            if (parent.parent != classSymbol.index) return false;
          }

          return true;
        };

        return storage.query(isInstanceOfClass);
      }

      bs::Vector<SymbolIndex> findAllWithParentOf(const ScriptSymbolStorage& storage,
                                                  SymbolIndex parent)
      {
        auto isMemberVariable = [&](const SymbolBase& s) {
          if (s.parent != parent) return false;
          if (!s.isClassVar) return false;

          return true;
        };

        return storage.query(isMemberVariable);
      }

      SymbolIndex findSymbolOfFunctionByAddress(const ScriptSymbolStorage& storage,
                                                bs::UINT32 address)
      {
        auto isCorrectFunction = [&](const SymbolBase& s) {
          if (s.type != SymbolType::ScriptFunction) return false;
          const SymbolScriptFunction& fn = (const SymbolScriptFunction&)s;

          return fn.address == address;
        };

        auto result = storage.query(isCorrectFunction);

        if (result.empty())
        {
          return SYMBOL_INDEX_INVALID;
        }
        else
        {
          return result[0];
        }
      }
    }  // namespace Queries
  }    // namespace Scripting
}  // namespace REGoth
