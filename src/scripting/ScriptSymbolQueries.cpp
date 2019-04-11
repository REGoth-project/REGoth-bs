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
    }  // namespace Queries
  }    // namespace Scripting
}  // namespace REGoth
