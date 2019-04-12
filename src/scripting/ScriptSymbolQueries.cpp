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
