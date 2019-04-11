/**\file
 */
#pragma once
#include <BsPrerequisites.h>
#include <scripting/ScriptVM.hpp>
#include "DaedalusStack.hpp"

namespace Daedalus
{
  class DATFile;
}

namespace REGoth
{
  namespace Scripting
  {
    class DATSymbolStorageLoader;
    class DaedalusClassVarResolver;
    class DaedalusVM : public ScriptVM
    {
    public:
      DaedalusVM(const Daedalus::DATFile& datFile);

    protected:
      /**
       * Executes a script function until it hits a return.
       *
       * Throws, if the function does not exist.
       *
       * @param  name  Name of the script function to execute.
       */

      void executeScriptFunction(const bs::String& name);
      /**
       * Runs the instruction found at the Program Counter and modifies it.
       *
       * @note If this encounteres a CALL-instruction, it will execute the
       *       whole sub-function.
       *
       * @return Whether the script function is not over yet. If this returns
       *         `false` then a Return-statement has been executed.
       */
      bool executeInstructionAtPC();

      /**
       * Runs from the current PC until the function it is in returned.
       */
      void executeUntilReturn();

      /**
       * Looks up the instruction memory at the given address and returns
       * the byte at that location.
       *
       * Throws if the given address is out of range.
       *
       * @param  address  Address to take the byte from.
       *
       * @return Byte at the given address in the instruction memory.
       */
      bs::UINT8 instructionMemoryAt(bs::UINT32 address);

      void fillSymbolStorage() override;

      /**
       * Pops an value from the stack. Also resolves variables.
       */
      bs::INT32 popIntValue();
      float popFloatValue();
      bs::String popStringValue();

      /**
       * Pops a reference to an variable stored inside a script symbol.
       *
       * Throws if the value on the stack is not a variable.
       */
      bs::INT32& popIntReference();
      float& popFloatReference();
      bs::String& popStringReference();

      /**
       * Pushes the given variable onto the stack.
       *
       * This function will put the given variable onto the stack with the
       * correct type.
       *
       * Throws if no stack for the symbol type exists.
       *
       * @param  symbolIndex  Index of the symbol to push.
       * @param  arrayIndex   Array-Index of the variable inside the symbol.
       */
      void pushVariable(SymbolIndex symbolIndex, bs::UINT32 arrayIndex);


    protected:
      DaedalusStack mStack;

    private:
      /**
       * Program counter register
       */
      bs::UINT32 mPC;

      bs::SPtr<DATSymbolStorageLoader> mInternals;
      bs::SPtr<Daedalus::DATFile> mDatFile;
      bs::SPtr<DaedalusClassVarResolver> mClassVarResolver;
    };
  }  // namespace Scripting
}  // namespace REGoth
