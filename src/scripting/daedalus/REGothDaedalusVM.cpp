#include "REGothDaedalusVM.hpp"
#include "DATSymbolStorageLoader.hpp"
#include "DaedalusClassVarResolver.hpp"
#include "DaedalusDisassembler.hpp"
#include <daedalus/DATFile.h>
#include <excepction/Throw.hpp>

namespace REGoth
{
  namespace Scripting
  {
    DaedalusVM::DaedalusVM(const Daedalus::DATFile& datFile)
    {
      // mInternals = bs::bs_shared_ptr_new<DATSymbolStorageLoader>(mScriptSymbols, datFile);
      mDatFile = bs::bs_shared_ptr_new<Daedalus::DATFile>(datFile);
      mClassVarResolver =
          bs::bs_shared_ptr_new<DaedalusClassVarResolver>(mScriptSymbols, mScriptObjects);
    }

    void DaedalusVM::fillSymbolStorage()
    {
      REGoth::Scripting::convertDatToREGothSymbolStorage(mScriptSymbols, *mDatFile);
    }

    void DaedalusVM::executeScriptFunction(const bs::String& name)
    {
      bs::String upper = name;
      bs::StringUtil::toUpperCase(upper);

      const auto& symbol = mScriptSymbols.getSymbol<SymbolScriptFunction>(upper);

      mPC = symbol.address;

      executeUntilReturn();
    }

    void DaedalusVM::executeUntilReturn()
    {
      bool didNotReachReturn;

      do
      {
        didNotReachReturn = executeInstructionAtPC();
      } while (didNotReachReturn);
    }

    bool DaedalusVM::executeInstructionAtPC()
    {
      Daedalus::PARStackOpCode opcode = mDatFile->getStackOpCode(mPC);

      bs::gDebug().logDebug("[DaedalusVM] Exec: " + disassembleOpcode(opcode, mScriptSymbols));

      mPC += opcode.opSize;

      switch (opcode.op)
      {
          // Arithmetic
          // ------------------------------------------------------------------------------

        case Daedalus::EParOp_Add:
          mStack.pushInt(popIntValue() + popIntValue());
          break;

        case Daedalus::EParOp_Subract:
          mStack.pushInt(popIntValue() - popIntValue());
          break;

        case Daedalus::EParOp_Multiply:
          mStack.pushInt(popIntValue() * popIntValue());
          break;

        case Daedalus::EParOp_Divide:
          mStack.pushInt(popIntValue() / popIntValue());
          break;

        case Daedalus::EParOp_Mod:
          mStack.pushInt(popIntValue() % popIntValue());
          break;

          // Binary
          // ----------------------------------------------------------------------------------

        case Daedalus::EParOp_BinOr:
          mStack.pushInt(popIntValue() | popIntValue());
          break;

        case Daedalus::EParOp_BinAnd:
          mStack.pushInt(popIntValue() & popIntValue());
          break;

        case Daedalus::EParOp_ShiftLeft:
          mStack.pushInt(popIntValue() << popIntValue());
          break;

        case Daedalus::EParOp_ShiftRight:
          mStack.pushInt(popIntValue() >> popIntValue());
          break;

        case Daedalus::EParOp_Negate:
          mStack.pushInt(~popIntValue());
          break;

          // Logic
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_LogOr:
          // Note: This can't be done in one line because the second pop might not be executed,
          // due to c++'s short-circuit evaluation
          {
            bs::INT32 a = popIntValue();
            bs::INT32 b = popIntValue();
            mStack.pushInt(a || b ? 1 : 0);
          }
          break;

        case Daedalus::EParOp_LogAnd:
          // Note: This can't be done in one line because the second pop might not be executed,
          // due to c++'s short-circuit evaluation
          {
            bs::INT32 a = popIntValue();
            bs::INT32 b = popIntValue();
            mStack.pushInt(a && b ? 1 : 0);
          }
          break;

          // Comparision
          // -----------------------------------------------------------------------------

        case Daedalus::EParOp_Less:
          mStack.pushInt(popIntValue() < popIntValue() ? 1 : 0);
          break;

        case Daedalus::EParOp_Greater:
          mStack.pushInt(popIntValue() > popIntValue() ? 1 : 0);
          break;

        case Daedalus::EParOp_LessOrEqual:
          mStack.pushInt(popIntValue() <= popIntValue() ? 1 : 0);
          break;

        case Daedalus::EParOp_Equal:
          mStack.pushInt(popIntValue() == popIntValue() ? 1 : 0);
          break;

        case Daedalus::EParOp_NotEqual:
          mStack.pushInt(popIntValue() != popIntValue() ? 1 : 0);
          break;

        case Daedalus::EParOp_GreaterOrEqual:
          mStack.pushInt(popIntValue() >= popIntValue() ? 1 : 0);
          break;

          // Unary
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_Plus:
          mStack.pushInt(+popIntValue());
          break;

        case Daedalus::EParOp_Minus:
          mStack.pushInt(-popIntValue());
          break;

        case Daedalus::EParOp_Not:
          mStack.pushInt(!popIntValue());
          break;

          // Stack
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_PushInt:
          mStack.pushInt(opcode.value);
          break;

        case Daedalus::EParOp_PushVar:
          pushVariable((bs::UINT32)opcode.symbol, 0);
          break;

        case Daedalus::EParOp_PushInstance:
          mStack.pushInstance((bs::UINT32)opcode.symbol);
          break;

        case Daedalus::EParOp_PushArrayVar:
          pushVariable((bs::UINT32)opcode.symbol, opcode.index);
          break;

          // Assign
          // ----------------------------------------------------------------------------------

        case Daedalus::EParOp_AssignFunc:
          // Function Pointes are pushed as intergers
          {
            SymbolIndex targetIndex = mStack.popFunction();
            SymbolIndex sourceIndex = mStack.popFunction();

            auto& target = mScriptSymbols.getSymbol<SymbolScriptFunction>(targetIndex);
            auto& source = mScriptSymbols.getSymbol<SymbolScriptFunction>(sourceIndex);

            if (target.isClassVar)
            {
              mClassVarResolver->resolveClassVariableFunctionPointer(target.name) = source.address;
            }
            else
            {
              target.address = source.address;
            }
          }
          break;

        case Daedalus::EParOp_AssignString:
        {
          auto& ref = popStringReference();
          ref       = popStringValue();
        }

        break;

        case Daedalus::EParOp_AssignFloat:
          popFloatReference() = popFloatValue();
          break;

        case Daedalus::EParOp_AssignInstance:
          // -
          {
            SymbolIndex targetIndex = mStack.popInstance();
            SymbolIndex sourceIndex = mStack.popInstance();

            auto& target = mScriptSymbols.getSymbol<SymbolInstance>(targetIndex);
            auto& source = mScriptSymbols.getSymbol<SymbolInstance>(sourceIndex);

            target.instance = source.instance;
          }
          break;

        case Daedalus::EParOp_Assign:
          popIntReference() = popIntValue();
          break;

        case Daedalus::EParOp_AssignAdd:
          popIntReference() += popIntValue();
          break;

        case Daedalus::EParOp_AssignSubtract:
          popIntReference() -= popIntValue();
          break;

        case Daedalus::EParOp_AssignMultiply:
          popIntReference() *= popIntValue();
          break;

        case Daedalus::EParOp_AssignDivide:
          popIntReference() /= popIntValue();
          break;

        case Daedalus::EParOp_AssignStringRef:
          REGOTH_THROW(NotImplementedException, "AssignStringRef is not implemented.");
          break;

          // Control flow
          // ----------------------------------------------------------------------------

        case Daedalus::EParOp_Ret:
          // Script function Ends here!
          return false;

        case Daedalus::EParOp_Jump:
          mPC = (bs::UINT32)opcode.address;
          break;

        case Daedalus::EParOp_JumpIf:
          // Jump if value on stack is 0
          if (!popIntValue())
          {
            mPC = (bs::UINT32)opcode.address;
          }
          break;

        case Daedalus::EParOp_Call:
          // Save some of this functions state and execute the whole sub-function
          {
            SymbolIndex currentInstance = mClassVarResolver->getCurrentInstance();
            bs::UINT32 pc               = mPC;

            mPC = (bs::UINT32)opcode.address;

            executeUntilReturn();

            mPC = pc;
            mClassVarResolver->setCurrentInstance(currentInstance);
          }
          break;

        case Daedalus::EParOp_CallExternal:
          break;

          // Other
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_SetInstance:
          mClassVarResolver->setCurrentInstance(opcode.symbol);
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

    bs::INT32& DaedalusVM::popIntReference()
    {
      DaedalusStack::StackVariableValue var = mStack.popIntVariable();

      SymbolBase& symbol = mScriptSymbols.getSymbolBase(var.symbol);

      if (symbol.type == SymbolType::Int)
      {
        if (symbol.isClassVar)
        {
          ScriptInts& ints = mClassVarResolver->resolveClassVariableInts(symbol.name);

          return ints.values[var.arrayIndex];
        }
        else
        {
          SymbolInt& symbolInt = (SymbolInt&)symbol;

          return symbolInt.ints.values[var.arrayIndex];
        }
      }
      else
      {
        REGOTH_THROW(InvalidStateException, "Popped variable is not an integer: " + symbol.name);
      }
    }

    float& DaedalusVM::popFloatReference()
    {
      DaedalusStack::StackVariableValue var = mStack.popStringVariable();

      SymbolBase& symbol = mScriptSymbols.getSymbolBase(var.symbol);

      if (symbol.type == SymbolType::Float)
      {
        if (symbol.isClassVar)
        {
          ScriptFloats& floats = mClassVarResolver->resolveClassVariableFloats(symbol.name);

          return floats.values[var.arrayIndex];
        }
        else
        {
          SymbolFloat& symbolFloat = (SymbolFloat&)symbol;

          return symbolFloat.floats.values[var.arrayIndex];
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

          return strings.values[var.arrayIndex];
        }
        else
        {
          SymbolString& symbolString = (SymbolString&)symbol;

          return symbolString.strings.values[var.arrayIndex];
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

  }  // namespace Scripting
}  // namespace REGoth
