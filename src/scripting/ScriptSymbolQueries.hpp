#pragma once
#include <BsPrerequisites.h>
#include "ScriptTypes.hpp"

namespace REGoth
{
  namespace Scripting
  {
    class ScriptSymbolStorage;

    namespace Queries
    {
      /**
       * Finds all Symbols of type `Class`.
       *
       * @param  storage  The script symbol storage to query.
       *
       * @return Indices of all found symbols.
       */
      bs::Vector<SymbolIndex> findAllClasses(const ScriptSymbolStorage& storage);

      /**
       * Finds all Symbols which have the given parent.
       *
       * @param  storage  The script symbol storage to query.
       * @param  parent   Parent to search for.
       *
       * @return Indices of all found symbols.
       */
      bs::Vector<SymbolIndex> findAllWithParentOf(const ScriptSymbolStorage& storage,
                                                  SymbolIndex parent);
    }  // namespace Queries
  }    // namespace Scripting
}  // namespace REGoth
