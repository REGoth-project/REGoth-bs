#pragma once
#include "ScriptTypes.hpp"
#include <BsPrerequisites.h>

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
       * Finds all Symbols which are instances of a given class.
       *
       * @param  storage    The script symbol storage to query.
       * @param  className  Name of the class.
       *
       * @return Indices of all found symbols.
       */
      bs::Vector<SymbolIndex> findAllInstancesOfClass(const ScriptSymbolStorage& storage,
                                                      const bs::String& className);

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

      /**
       * Given an address, finds the symbol of the function starting at that address.
       *
       * @param  storage  The script symbol storage to query.
       * @param  address  Address of the function to find.
       *
       * @return Symbol-index of the found function.
       */
      SymbolIndex findSymbolOfFunctionByAddress(const ScriptSymbolStorage& storage,
                                                bs::UINT32 address);
    }  // namespace Queries
  }    // namespace Scripting
}  // namespace REGoth
