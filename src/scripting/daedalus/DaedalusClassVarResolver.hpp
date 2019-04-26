/**\file
 */
#pragma once
#include <BsPrerequisites.h>
#include "scripting/ScriptSymbolStorage.hpp"
#include <scripting/ScriptObject.hpp>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * In Daedalus, Symbols with the `ClassVar`-flag set will not use the data
     * it carries with it, but rather ask the VM for something like a *this-pointer*.
     *
     * The *this-pointer* is actually called *Current Instance*.
     *
     * This class will, given a class-var symbol, go to the object set in the *Current Instance*
     * and return a reference to that data.
     */
    class DaedalusClassVarResolver
    {
    public:
      DaedalusClassVarResolver(const ScriptSymbolStorage& scriptSymbols,
                               ScriptObjectStorage& scriptObjects);

      /**
       * @return Whether the instance set in the *Current Instance*-register is valid.
       */
      bool isCurrentInstanceValid() const;

      /**
       * @return Whether the instance set in the *Current Instance*-register is of the given
       *         class. Will also check whether it is valid.
       */
      bool isCurrentInstanceOfClass(const bs::String& className) const;

      /**
       * @return The object behind the *Current Instance*-register. Throws if it's invalid.
       */
      ScriptObject& getCurrentInstanceObject();

      /**
       * @return The handle of the *Current Instance*.
       */
      SymbolIndex getCurrentInstance() const { return mCurrentInstance; }

      /**
       * Sets the current instance object.
       */
      void setCurrentInstance(ScriptObjectHandle handle);

      /**
       * Get a reference to the data of the member variable in the *Current Instance*.
       *
       * If passed a string like `C_ITEM.VALUE`, this will return a reference to
       * the `VALUE`-members data from the instance set in the *Current Instance*-
       * register.
       *
       * Throws if the member does not exist or no *Current Instance* is set.
       *
       * @param  memberSymbolName  Actual symbol name of the member variable, e.g.
       * `C_ITEM.VALUE`.
       *
       * @return Reference to that members data within the *Current Instance*.
       */
      ScriptInts& resolveClassVariableInts(const bs::String& memberSymbolName);

      /** @copydoc resolveClassVariableInts */
      bs::UINT32& resolveClassVariableFunctionPointer(const bs::String& memberSymbolName);

      /** @copydoc resolveClassVariableInts */
      ScriptFloats& resolveClassVariableFloats(const bs::String& memberSymbolName);

      /** @copydoc resolveClassVariableInts */
      ScriptStrings& resolveClassVariableStrings(const bs::String& memberSymbolName);

    private:

      /**
       * Throws if the object referenced via *Current Instance* is not of the given
       * class name.
       */
      void throwIfCurrentInstanceNotOfClass(const bs::String& className) const;

      /**
       * Throws if the object referenced via *Current Instance* is invalid.
       */
      void throwIfCurrentInstanceIsInvalid() const;

      /**
       * Throws if the object referenced via *Current Instance* is not matching the
       * class named within the member variables symbol name. So if the *Current Instance*
       * is of class `C_NPC` and the given Member Symbol Name is like `C_ITEM.SOMETHING`,
       * then this method will throw.
       *
       * It will also throw if the *Current Instance* is invalid.
       */
      void throwIfCurrentInstanceNotMatching(const bs::String& memberSymbolName) const;

      /**
       * Given the name of a member symbol, e.g. `C_ITEM.VALUE`, this will return the name
       * of the class, so `C_ITEM`.
       *
       * Throws if the given name does not match the pattern.
       *
       * @return Name of the class the given member variable belongs to.
       */
      bs::String extractClassNameFromMemberSymbol(const bs::String& memberSymbolName) const;

      /**
       * Converts a symbol name to the actual member variable name.
       *
       * Inside the symbol-table the symbols for member variables are named
       * like `C_ITEM.SOMETHING`. In our templates, we don't want to repeat
       * the class name in all member variables, so we can just strip the
       * part before and including the `.` to clean it.
       *
       * If there is no `.` or if there is more than one, then the whole name is returned.
       *
       * @param  memberSymbolName  Name of the symbol from the member variable, e.g.
       *                           `C_ITEM.SOMETHING`.
       *
       * @return Actual member variable name.
       */
      bs::String demangleMemberName(const bs::String& memberSymbolName);

      /**
       * *Current Instance*-Register of the VM. This is basically the this-pointer for code running
       * in instance constructors.
       */
      ScriptObjectHandle mCurrentInstance;
      const ScriptSymbolStorage& mScriptSymbols;
      ScriptObjectStorage& mScriptObjects;
    };
  }  // namespace Scripting
}  // namespace REGoth
