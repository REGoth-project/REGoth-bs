#include "DaedalusDisassembler.hpp"
#include "scripting/ScriptSymbolQueries.hpp"
#include <scripting/ScriptSymbolStorage.hpp>

namespace REGoth
{
  namespace Scripting
  {
    bs::String disassembleOpcode(const Daedalus::PARStackOpCode& opcode,
                                 const ScriptSymbolStorage& symbols)
    {
      auto symName = [&](SymbolIndex index) { return symbols.getSymbolBase(index).name; };

      auto findFunctionFromAddress = [&](bs::UINT32 address) {
        SymbolIndex index = Queries::findSymbolOfFunctionByAddress(symbols, address);

        if (index == SYMBOL_INDEX_INVALID)
        {
          return bs::StringUtil::format("[invalid: {0}]", address);
        }

        return symName(index);
      };

      auto symValue = [&](SymbolIndex index, bs::UINT32 arrayindex = 0) {
        SymbolBase& sym = symbols.getSymbolBase(index);

        switch (sym.type)
        {
          case SymbolType::Int:
            return bs::toString(symbols.getSymbol<SymbolInt>(index).ints.values[arrayindex]);
          case SymbolType::Float:
            return bs::toString(symbols.getSymbol<SymbolFloat>(index).floats.values[arrayindex]);
          case SymbolType::String:
            return "'" + symbols.getSymbol<SymbolString>(index).strings.values[arrayindex] + "'";
        default:
          return bs::String("[-]");
        }
      };

      switch (opcode.op)
      {
          // Arithmetic
          // ------------------------------------------------------------------------------

        case Daedalus::EParOp_Add:
          return bs::StringUtil::format("Add a + b");

        case Daedalus::EParOp_Subract:
          return bs::StringUtil::format("Sub a - b");

        case Daedalus::EParOp_Multiply:
          return bs::StringUtil::format("Mul a * b");

        case Daedalus::EParOp_Divide:
          return bs::StringUtil::format("Div a / b");

        case Daedalus::EParOp_Mod:
          return bs::StringUtil::format("Mod a % b");

          // Binary
          // ----------------------------------------------------------------------------------

        case Daedalus::EParOp_BinOr:
          return bs::StringUtil::format("BinOr a | b");

        case Daedalus::EParOp_BinAnd:
          return bs::StringUtil::format("BinAnd a & b");

        case Daedalus::EParOp_ShiftLeft:
          return bs::StringUtil::format("ShiftLeft a << b");

        case Daedalus::EParOp_ShiftRight:
          return bs::StringUtil::format("ShiftRight a >> b");

        case Daedalus::EParOp_Negate:
          return bs::StringUtil::format("Negate ~a");
          // Logic
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_LogOr:
          return bs::StringUtil::format("LogOr a || b");

        case Daedalus::EParOp_LogAnd:
          return bs::StringUtil::format("LogOr a && b");

          // Comparision
          // -----------------------------------------------------------------------------

        case Daedalus::EParOp_Less:
          return bs::StringUtil::format("Less a < b");

        case Daedalus::EParOp_Greater:
          return bs::StringUtil::format("Greater a > b");

        case Daedalus::EParOp_LessOrEqual:
          return bs::StringUtil::format("LessOrEqual a < b");

        case Daedalus::EParOp_Equal:
          return bs::StringUtil::format("Equal a == b");

        case Daedalus::EParOp_NotEqual:
          return bs::StringUtil::format("NotEqual a != b");

        case Daedalus::EParOp_GreaterOrEqual:
          return bs::StringUtil::format("GreaterOrEqual a < b");

          // Unary
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_Plus:
          return bs::StringUtil::format("Plus +a");

        case Daedalus::EParOp_Minus:
          return bs::StringUtil::format("Minus -a");

        case Daedalus::EParOp_Not:
          return bs::StringUtil::format("Not !a");

          // Stack
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_PushInt:
          return bs::StringUtil::format("PushInt {0}", opcode.value);

        case Daedalus::EParOp_PushVar:
          return bs::StringUtil::format("PushVar {0}: {1}", symName(opcode.symbol),
                                        symValue(opcode.symbol));

        case Daedalus::EParOp_PushInstance:
          return bs::StringUtil::format("PushInstance {0}", symName(opcode.symbol),
                                        symValue(opcode.symbol));

        case Daedalus::EParOp_PushArrayVar:
          return bs::StringUtil::format("PushArrayVar {0}[{1}]", symName(opcode.symbol),
                                        (int)opcode.index, symValue(opcode.symbol, opcode.index));

          // Assign
          // ----------------------------------------------------------------------------------

        case Daedalus::EParOp_AssignFunc:
          return bs::StringUtil::format("AssignFunc a = b");

        case Daedalus::EParOp_AssignString:
          return bs::StringUtil::format("AssignString a = b");

        case Daedalus::EParOp_AssignFloat:
          return bs::StringUtil::format("AssignFloat a = b");

        case Daedalus::EParOp_AssignInstance:
          return bs::StringUtil::format("AssignInstance a = b");

        case Daedalus::EParOp_Assign:
          return bs::StringUtil::format("Assign a = b");

        case Daedalus::EParOp_AssignAdd:
          return bs::StringUtil::format("AssignAdd a += b");

        case Daedalus::EParOp_AssignSubtract:
          return bs::StringUtil::format("AssignSubtract a -= b");

        case Daedalus::EParOp_AssignMultiply:
          return bs::StringUtil::format("AssignMultiply a *= b");

        case Daedalus::EParOp_AssignDivide:
          return bs::StringUtil::format("AssignDivide a /= b");

        case Daedalus::EParOp_AssignStringRef:
          return bs::StringUtil::format("AssignStringRef UNIMPLEMENTED");

          // Control flow
          // ----------------------------------------------------------------------------

        case Daedalus::EParOp_Ret:
          return bs::StringUtil::format("Return");

        case Daedalus::EParOp_Jump:
          return bs::StringUtil::format("Jump {0}", opcode.address);

        case Daedalus::EParOp_JumpIf:
          return bs::StringUtil::format("JumpIf {0}", opcode.address);

        case Daedalus::EParOp_Call:
          return bs::StringUtil::format("Call {0}", findFunctionFromAddress(opcode.address));

        case Daedalus::EParOp_CallExternal:
          return bs::StringUtil::format("CallExternal {0}", symName(opcode.symbol));

          // Other
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_SetInstance:
          return bs::StringUtil::format("SetInstance CurrentInstance = {0}", symName(opcode.symbol));

        default:
          return bs::StringUtil::format("Unknown Opcode {0}", opcode.op);
          break;
      }
    }
  }  // namespace Scripting
}  // namespace REGoth
