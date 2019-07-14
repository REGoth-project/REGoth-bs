#include "REGothDaedalusVM.hpp"
#include "DATSymbolStorageLoader.hpp"
#include "DaedalusClassVarResolver.hpp"
#include "DaedalusDisassembler.hpp"
#include <RTTI/RTTI_REGothDaedalusVM.hpp>
#include <daedalus/DATFile.h>
#include <exception/Throw.hpp>
#include <log/logging.hpp>

namespace REGoth
{
  namespace Scripting
  {
    const bs::Set<bs::String> FUNCTIONS_TO_ACTIVATE_DISASSEMBLER_FOR = {
        "DIA_BAALPARVEZ_GOTOPSI_CONDITION",
        "DIA_BAALPARVEZ_GOTOPSI_INFO",
        "ZS_GUIDEPC_LOOP",
        "INFO_DIEGO_EXIT_GAMESTART_CONDITION",
    };

    const bs::Set<bs::String> FUNCTIONS_TO_HIDE_IN_DISASSEMBLY = {
        "PRINTDEBUGNPC",
        "PRINTGLOBALS",
        "PRINTDEBUGINT",
    };

    DaedalusVM::DaedalusVM(const bs::Vector<bs::UINT8>& datFileData)
    {
      mDatFile = bs::bs_shared_ptr_new<Daedalus::DATFile>(datFileData.data(), datFileData.size());
      mClassVarResolver =
          bs::bs_shared_ptr_new<DaedalusClassVarResolver>(mScriptSymbols, mScriptObjects);
      mDatFileData = datFileData;
    }

    void DaedalusVM::fillSymbolStorage()
    {
      REGoth::Scripting::convertDatToREGothSymbolStorage(mScriptSymbols, *mDatFile);

      registerAllExternals();
    }

    bool DaedalusVM::shouldEnableDisassemblerForFunction(const bs::String& uppercaseName) const
    {
      return FUNCTIONS_TO_ACTIVATE_DISASSEMBLER_FOR.find(uppercaseName) !=
             FUNCTIONS_TO_ACTIVATE_DISASSEMBLER_FOR.end();
    }

    bool DaedalusVM::shouldHideFunctionInDisassembly(const bs::String& uppercaseName) const
    {
      return FUNCTIONS_TO_HIDE_IN_DISASSEMBLY.find(uppercaseName) !=
             FUNCTIONS_TO_HIDE_IN_DISASSEMBLY.end();
    }

    void DaedalusVM::executeScriptFunction(const bs::String& name)
    {
      bs::String upper = name;
      bs::StringUtil::toUpperCase(upper);

      const auto& symbol = mScriptSymbols.getSymbol<SymbolScriptFunction>(upper);

      mPC = symbol.address;

      executeUntilReturn();
    }

    void DaedalusVM::executeScriptFunction(bs::UINT32 address)
    {
      mPC = address;

      executeUntilReturn();
    }

    void DaedalusVM::executeUntilReturn()
    {
      bool wasDisassemblerEnabledBefore = mIsDisassemblerEnabled;

      auto symIndex = scriptSymbols().findFunctionByAddress(mPC);

      // TODO: Guard these by some configuration variable so they only run during development
      if (symIndex != SYMBOL_INDEX_INVALID)
      {
        bs::String name = scriptSymbols().getSymbolName(symIndex);

        if (shouldEnableDisassemblerForFunction(name))
        {
          mIsDisassemblerEnabled = true;
        }

        if (mIsDisassemblerEnabled && shouldHideFunctionInDisassembly(name))
        {
          mIsDisassemblerEnabled = false;
        }

        if (mIsDisassemblerEnabled)
        {
          findFunctionAtAddressAndLog(mPC);
        }
      }

      bool didNotReachReturn;

      do
      {
        didNotReachReturn = executeInstructionAtPC();
      } while (didNotReachReturn);

      mIsDisassemblerEnabled = wasDisassemblerEnabledBefore;
    }

    bool DaedalusVM::executeInstructionAtPC()
    {
      Daedalus::PARStackOpCode opcode = mDatFile->getStackOpCode(mPC);

      mPC += opcode.opSize;

      switch (opcode.op)
      {
          // Arithmetic
          // ------------------------------------------------------------------------------

        case Daedalus::EParOp_Add:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs + rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_Subract:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs - rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_Multiply:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs * rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_Divide:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs / rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_Mod:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs % rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

          // Binary
          // ----------------------------------------------------------------------------------

        case Daedalus::EParOp_BinOr:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs | rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_BinAnd:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs & rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_ShiftLeft:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs << rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_ShiftRight:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs >> rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_Negate:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 res = ~lhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), "", bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

          // Logic
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_LogOr:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs || rhs ? 1 : 0;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_LogAnd:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs && rhs ? 1 : 0;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

          // Comparision
          // -----------------------------------------------------------------------------

        case Daedalus::EParOp_Less:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs < rhs ? 1 : 0;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_Greater:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs > rhs ? 1 : 0;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_LessOrEqual:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs <= rhs ? 1 : 0;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_Equal:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs == rhs ? 1 : 0;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_NotEqual:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs != rhs ? 1 : 0;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_GreaterOrEqual:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 rhs = popIntValue();
          bs::INT32 res = lhs >= rhs ? 1 : 0;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), bs::toString(rhs), bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

          // Unary
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_Plus:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 res = +lhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), "", bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_Minus:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 res = -lhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), "", bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

        case Daedalus::EParOp_Not:
        {
          bs::INT32 lhs = popIntValue();
          bs::INT32 res = !lhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs), "", bs::toString(res));
          }

          mStack.pushInt(res);
        }
        break;

          // Stack
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_PushInt:
          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(opcode.value), "", "");
          }

          mStack.pushInt(opcode.value);
          break;

        case Daedalus::EParOp_PushVar:
          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, "", "", "");
          }

          pushVariable((bs::UINT32)opcode.symbol, 0);
          break;

        case Daedalus::EParOp_PushInstance:
          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, "", "", "");
          }

          mStack.pushInstance((bs::UINT32)opcode.symbol);
          break;

        case Daedalus::EParOp_PushArrayVar:
          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, "", "", "");
          }

          pushVariable((bs::UINT32)opcode.symbol, opcode.index);
          break;

          // Assign
          // ----------------------------------------------------------------------------------

        case Daedalus::EParOp_AssignFunc:
          // Function Pointes are pushed as intergers
          {
            SymbolIndex targetIndex = mStack.popFunction();
            SymbolIndex sourceIndex = (SymbolIndex)popIntValue();

            if (mIsDisassemblerEnabled)
            {
              disassembleAndLogOpcode(opcode, "", "", "");
            }

            auto& target = mScriptSymbols.getSymbol<SymbolScriptFunction>(targetIndex);

            SymbolBase& sourceBase = mScriptSymbols.getSymbolBase(sourceIndex);

            bs::UINT32 sourceAddress;

            if (sourceBase.type == SymbolType::ScriptFunction)
            {
              auto& sourceFunction = (SymbolScriptFunction&)sourceBase;
              sourceAddress        = sourceFunction.address;
            }
            else if (sourceBase.type == SymbolType::Instance)
            {
              // Because deadalus' type safety isn't what it seems like, we need to handle this
              // stupid edgecase of an instance being assigned to a function-pointer class variable.
              // Specifically `C_ITEM.owner`, which is declared as `VAR FUNC owner` but is then
              // given instances of class `C_NPC`.
              //
              // Since the original just seems to store the symbol index, not caring about the type,
              // it works there. However, REGoth stores the function address, so we have to work
              // around that by storing the instances constructor address. Let's just hope it will
              // work...
              //
              // We could also add another type of symbol for function pointers, but then we have
              // Symbols which *should* refer to functions, but sometimes refer to instances, which
              // sucks as well.
              auto& sourceInstance = (SymbolInstance&)sourceBase;
              sourceAddress        = sourceInstance.constructorAddress;
            }
            else
            {
              REGOTH_THROW(InvalidParametersException,
                           bs::StringUtil::format("Cannot get the address of symbol {0} of type {1}",
                                                  sourceBase.name, (int)sourceBase.type));
            }

            if (target.isClassVar)
            {
              mClassVarResolver->resolveClassVariableFunctionPointer(target.name) = sourceAddress;
            }
            else
            {
              target.address = sourceAddress;
            }
          }
          break;

        case Daedalus::EParOp_AssignString:
        {
          auto& lhs       = popStringReference();
          const auto& rhs = popStringValue();

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, rhs, lhs, "");
          }

          lhs = rhs;
        }

        break;

        case Daedalus::EParOp_AssignFloat:
        {
          auto& lhs       = popFloatReference();
          const auto& rhs = popFloatValue();

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(rhs), bs::toString(lhs), "");
          }

          lhs = rhs;
        }
        break;

        case Daedalus::EParOp_AssignInstance:
          // -
          {
            SymbolIndex targetIndex = mStack.popInstance();
            SymbolIndex sourceIndex = mStack.popInstance();

            auto& target = mScriptSymbols.getSymbol<SymbolInstance>(targetIndex);
            auto& source = mScriptSymbols.getSymbol<SymbolInstance>(sourceIndex);

            if (mIsDisassemblerEnabled)
            {
              disassembleAndLogOpcode(opcode, target.name, source.name, "");
            }

            target.instance = source.instance;
          }
          break;

        case Daedalus::EParOp_Assign:
        {
          auto& lhs       = popIntReference();
          const auto& rhs = popIntValue();

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          lhs = rhs;
        }
        break;

        case Daedalus::EParOp_AssignAdd:
        {
          auto& lhs       = popIntReference();
          const auto& rhs = popIntValue();
          auto res        = lhs + rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          lhs = res;
        }
        break;

        case Daedalus::EParOp_AssignSubtract:
        {
          auto& lhs       = popIntReference();
          const auto& rhs = popIntValue();
          auto res        = lhs - rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          lhs = res;
        }
        break;

        case Daedalus::EParOp_AssignMultiply:
        {
          auto& lhs       = popIntReference();
          const auto& rhs = popIntValue();
          auto res        = lhs * rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          lhs = res;
        }
        break;

        case Daedalus::EParOp_AssignDivide:
        {
          auto& lhs       = popIntReference();
          const auto& rhs = popIntValue();
          auto res        = lhs / rhs;

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          lhs = res;
        }
        break;

        case Daedalus::EParOp_AssignStringRef:
          REGOTH_THROW(NotImplementedException, "AssignStringRef is not implemented.");
          break;

          // Control flow
          // ----------------------------------------------------------------------------

        case Daedalus::EParOp_Ret:
          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          // Script function Ends here!
          return false;

        case Daedalus::EParOp_Jump:
          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          mPC = (bs::UINT32)opcode.address;
          break;

        case Daedalus::EParOp_JumpIf:
        {
          bs::UINT32 lhs = popIntValue();

          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode, bs::toString(lhs));
          }

          // Jump if value on stack is 0
          if (!lhs)
          {
            mPC = (bs::UINT32)opcode.address;
          }
        }
        break;

        case Daedalus::EParOp_Call:
        {
          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          // Save some of this functions state and execute the whole sub-function
          SymbolIndex currentInstance = mClassVarResolver->getCurrentInstance();
          bs::UINT32 pc               = mPC;

          mPC = (bs::UINT32)opcode.address;
          mCallDepth += 1;

          executeUntilReturn();

          mCallDepth -= 1;
          mPC = pc;
          mClassVarResolver->setCurrentInstance(currentInstance);
        }
        break;

        case Daedalus::EParOp_CallExternal:
        {
          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          auto it = mExternals.find(opcode.symbol);

          if (it != mExternals.end())
          {
            SymbolIndex currentInstance = mClassVarResolver->getCurrentInstance();
            bs::UINT32 pc               = mPC;
            mCallDepth += 1;

            (this->*it->second)();

            mCallDepth -= 1;
            mPC = pc;
            mClassVarResolver->setCurrentInstance(currentInstance);
          }
          else
          {
            auto& sym = mScriptSymbols.getSymbol<SymbolExternalFunction>(opcode.symbol);

            // REGOTH_LOG(Info, Uncategorized, "[REGothDaedalusVM] External not implemented: " + sym.name);

            // Put a dummy value onto the stack to get deterministic results
            switch (sym.returnType)
            {
              case ReturnType::Int:
                mStack.pushInt(0);
                break;
              case ReturnType::Float:
                mStack.pushFloat(0.0f);
                break;
              case ReturnType::String:
                mStack.pushString("");
                break;
              case ReturnType::Invalid:
              case ReturnType::Void:
                break;
            }

            // REGOTH_THROW(
            //     NotImplementedException,
            //     "External not implemented: " +
            //     mScriptSymbols.getSymbolBase(opcode.symbol).name);
          }
        }
        break;

          // Other
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_SetInstance:
        {
          if (mIsDisassemblerEnabled)
          {
            disassembleAndLogOpcode(opcode);
          }

          const SymbolInstance& instance = mScriptSymbols.getSymbol<SymbolInstance>(opcode.symbol);
          mClassVarResolver->setCurrentInstance(instance.instance);
        }
        break;

        default:
          REGOTH_THROW(InvalidParametersException,
                       "Unsupported or invalid opcode: " + bs::toString(opcode.op));
          break;
      }

      // Script function hasn't returned yes, keep going.
      // See RET-Instruction for `return false`.
      return true;
    }

    bs::INT32 DaedalusVM::popIntValue()
    {
      if (mStack.isTopOfIntStackVariable())
      {
        return popIntReference();
      }
      else
      {
        return mStack.popInt();
      }
    }

    float DaedalusVM::popFloatValue()
    {
      if (mStack.isTopOfFloatStackVariable())
      {
        return popFloatReference();
      }
      else
      {
        return mStack.popFloat();
      }
    }

    bs::String DaedalusVM::popStringValue()
    {
      if (mStack.isTopOfStringStackVariable())
      {
        return popStringReference();
      }
      else
      {
        return mStack.popString();
      }
    }

    ScriptObjectHandle DaedalusVM::popInstanceScriptObject()
    {
      SymbolIndex symbol   = mStack.popInstance();
      const auto& instance = mScriptSymbols.getSymbol<SymbolInstance>(symbol);

      if (instance.isClassVar)
      {
        REGOTH_THROW(InvalidParametersException, "Instances cannot be classvars!");
      }

      return instance.instance;
    }

    bs::INT32& DaedalusVM::popIntReference()
    {
      DaedalusStack::StackVariableValue var = mStack.popIntVariable();

      SymbolBase& symbol = mScriptSymbols.getSymbolBase(var.symbol);

      if (symbol.type == SymbolType::Int)
      {
        if (symbol.isClassVar)
        {
          ScriptInts& ints = mClassVarResolver->resolveClassVariableInts(symbol.name);

          if (var.arrayIndex >= ints.size())
          {
            REGOTH_THROW(
                InvalidParametersException,
                bs::StringUtil::format("Array index out of range! (index: {0}, ArraySize: {1})",
                                       var.arrayIndex, ints.size()));
          }

          return ints[var.arrayIndex];
        }
        else
        {
          SymbolInt& symbolInt = (SymbolInt&)symbol;

          if (var.arrayIndex >= symbolInt.ints.size())
          {
            REGOTH_THROW(
                InvalidParametersException,
                bs::StringUtil::format("Array index out of range! (index: {0}, ArraySize: {1})",
                                       var.arrayIndex, symbolInt.ints.size()));
          }

          return symbolInt.ints[var.arrayIndex];
        }
      }
      else
      {
        REGOTH_THROW(InvalidStateException, "Popped variable is not an integer: " + symbol.name);
      }
    }

    float& DaedalusVM::popFloatReference()
    {
      DaedalusStack::StackVariableValue var = mStack.popFloatVariable();

      SymbolBase& symbol = mScriptSymbols.getSymbolBase(var.symbol);

      if (symbol.type == SymbolType::Float)
      {
        if (symbol.isClassVar)
        {
          ScriptFloats& floats = mClassVarResolver->resolveClassVariableFloats(symbol.name);

          if (var.arrayIndex >= floats.size())
          {
            REGOTH_THROW(
                InvalidParametersException,
                bs::StringUtil::format("Array index out of range! (index: {0}, ArraySize: {1})",
                                       var.arrayIndex, floats.size()));
          }

          return floats[var.arrayIndex];
        }
        else
        {
          SymbolFloat& symbolFloat = (SymbolFloat&)symbol;

          if (var.arrayIndex >= symbolFloat.floats.size())
          {
            REGOTH_THROW(
                InvalidParametersException,
                bs::StringUtil::format("Array index out of range! (index: {0}, ArraySize: {1})",
                                       var.arrayIndex, symbolFloat.floats.size()));
          }

          return symbolFloat.floats[var.arrayIndex];
        }
      }
      else
      {
        REGOTH_THROW(InvalidStateException, "Popped variable is not an float: " + symbol.name);
      }
    }

    bs::String& DaedalusVM::popStringReference()
    {
      DaedalusStack::StackVariableValue var = mStack.popStringVariable();

      SymbolBase& symbol = mScriptSymbols.getSymbolBase(var.symbol);

      if (symbol.type == SymbolType::String)
      {
        if (symbol.isClassVar)
        {
          ScriptStrings& strings = mClassVarResolver->resolveClassVariableStrings(symbol.name);

          if (var.arrayIndex >= strings.size())
          {
            REGOTH_THROW(
                InvalidParametersException,
                bs::StringUtil::format("Array index out of range! (index: {0}, ArraySize: {1})",
                                       var.arrayIndex, strings.size()));
          }

          return strings[var.arrayIndex];
        }
        else
        {
          SymbolString& symbolString = (SymbolString&)symbol;

          if (var.arrayIndex >= symbolString.strings.size())
          {
            REGOTH_THROW(
                InvalidParametersException,
                bs::StringUtil::format("Array index out of range! (index: {0}, ArraySize: {1})",
                                       var.arrayIndex, symbolString.strings.size()));
          }

          return symbolString.strings[var.arrayIndex];
        }
      }
      else
      {
        REGOTH_THROW(InvalidStateException, "Popped variable is not an string: " + symbol.name);
      }
    }

    void DaedalusVM::pushVariable(SymbolIndex symbolIndex, bs::UINT32 arrayIndex)
    {
      SymbolBase& symbol = mScriptSymbols.getSymbolBase(symbolIndex);

      if (symbol.type == SymbolType::Int)
      {
        mStack.pushIntVariable(symbolIndex, arrayIndex);
      }
      else if (symbol.type == SymbolType::Float)
      {
        mStack.pushFloatVariable(symbolIndex, arrayIndex);
      }
      else if (symbol.type == SymbolType::String)
      {
        mStack.pushStringVariable(symbolIndex, arrayIndex);
      }
      else if (symbol.type == SymbolType::Instance)
      {
        mStack.pushInstance(symbolIndex);
      }
      else if (symbol.type == SymbolType::ScriptFunction)
      {
        mStack.pushFunction(symbolIndex);
      }
    }

    void DaedalusVM::registerExternal(const bs::String& name, externalCallback callback)
    {
      SymbolIndex symbol = mScriptSymbols.findIndexBySymbolName(name);

      mExternals[symbol] = callback;
    }

    void DaedalusVM::disassembleAndLogOpcode(const Daedalus::PARStackOpCode& opcode,
                                             const bs::String& lhs, const bs::String& rhs,
                                             const bs::String& res)
    {
      REGOTH_LOG(Info, Uncategorized,
             bs::StringUtil::format("[DaedalusVM] Exec: {0}{1}", makeCallDepthString(mCallDepth),
                                    disassembleOpcode(opcode, mScriptSymbols, lhs, rhs, res)));
    }

    void DaedalusVM::findFunctionAtAddressAndLog(bs::UINT32 address)
    {
      auto fnSymbolIdx = scriptSymbols().findFunctionByAddress(address);

      bs::String name;
      if (fnSymbolIdx == SYMBOL_INDEX_INVALID)
      {
        name = "<invalid function address>";
      }
      else
      {
        const auto& fnSymbol = scriptSymbols().getSymbol<SymbolScriptFunction>(fnSymbolIdx);

        name = fnSymbol.name;
      }

      REGOTH_LOG(Info, Uncategorized,
             bs::StringUtil::format("[DaedalusVM] Exec: {0}Call {1}",
                                    makeCallDepthString(mCallDepth), name));
    }

    REGOTH_DEFINE_RTTI(DaedalusVM);
  }  // namespace Scripting
}  // namespace REGoth
