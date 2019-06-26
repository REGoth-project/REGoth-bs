#include "DaedalusDisassembler.hpp"
#include "scripting/ScriptSymbolQueries.hpp"
#include <scripting/ScriptSymbolStorage.hpp>

namespace REGoth
{
  namespace Scripting
  {
    bs::String disassembleOpcode(const Daedalus::PARStackOpCode& opcode,
                                 const ScriptSymbolStorage& symbols, const bs::String& lhs,
                                 const bs::String& rhs, const bs::String& res)
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
            return bs::toString(symbols.getSymbol<SymbolInt>(index).ints[arrayindex]);
          case SymbolType::Float:
            return bs::toString(symbols.getSymbol<SymbolFloat>(index).floats[arrayindex]);
          case SymbolType::String:
            return "'" + symbols.getSymbol<SymbolString>(index).strings[arrayindex] + "'";
          default:
            return bs::String("[-]");
        }
      };

      switch (opcode.op)
      {
          // Arithmetic
          // ------------------------------------------------------------------------------

        case Daedalus::EParOp_Add:
          return bs::StringUtil::format("Add {0} + {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_Subract:
          return bs::StringUtil::format("Sub {0} - {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_Multiply:
          return bs::StringUtil::format("Mul {0} * {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_Divide:
          return bs::StringUtil::format("Div {0} / {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_Mod:
          return bs::StringUtil::format("Mod {0} % {1} -> {2}", lhs, rhs, res);

          // Binary
          // ----------------------------------------------------------------------------------

        case Daedalus::EParOp_BinOr:
          return bs::StringUtil::format("BinOr {0} | {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_BinAnd:
          return bs::StringUtil::format("BinAnd {0} & {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_ShiftLeft:
          return bs::StringUtil::format("ShiftLeft {0} << {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_ShiftRight:
          return bs::StringUtil::format("ShiftRight {0} >> {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_Negate:
          return bs::StringUtil::format("Negate ~a");
          // Logic
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_LogOr:
          return bs::StringUtil::format("LogOr {0} || {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_LogAnd:
          return bs::StringUtil::format("LogAnd {0} && {1} -> {2}", lhs, rhs, res);

          // Comparision
          // -----------------------------------------------------------------------------

        case Daedalus::EParOp_Less:
          return bs::StringUtil::format("Less {0} < {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_Greater:
          return bs::StringUtil::format("Greater {0} > {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_LessOrEqual:
          return bs::StringUtil::format("LessOrEqual {0} < {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_Equal:
          return bs::StringUtil::format("Equal {0} == {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_NotEqual:
          return bs::StringUtil::format("NotEqual {0} != {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_GreaterOrEqual:
          return bs::StringUtil::format("GreaterOrEqual {0} < {1} -> {2}", lhs, rhs, res);

          // Unary
          // -----------------------------------------------------------------------------------

        case Daedalus::EParOp_Plus:
          return bs::StringUtil::format("Plus +{0} -> {1}", lhs, res);

        case Daedalus::EParOp_Minus:
          return bs::StringUtil::format("Minus -{0} -> {1}", lhs, res);

        case Daedalus::EParOp_Not:
          return bs::StringUtil::format("Not !{0} -> {1}", lhs, res);

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
          return bs::StringUtil::format("AssignFunc {0} = {1}", lhs, rhs);

        case Daedalus::EParOp_AssignString:
          return bs::StringUtil::format("AssignString {0} = {1}", lhs, rhs);

        case Daedalus::EParOp_AssignFloat:
          return bs::StringUtil::format("AssignFloat {0} = {1}", lhs, rhs);

        case Daedalus::EParOp_AssignInstance:
          return bs::StringUtil::format("AssignInstance {0} = {1}", lhs, rhs);

        case Daedalus::EParOp_Assign:
          return bs::StringUtil::format("Assign {0} = {1}", lhs, rhs);

        case Daedalus::EParOp_AssignAdd:
          return bs::StringUtil::format("AssignAdd {0} += {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_AssignSubtract:
          return bs::StringUtil::format("AssignSubtract {0} -= {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_AssignMultiply:
          return bs::StringUtil::format("AssignMultiply {0} *= {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_AssignDivide:
          return bs::StringUtil::format("AssignDivide {0} /= {1} -> {2}", lhs, rhs, res);

        case Daedalus::EParOp_AssignStringRef:
          return bs::StringUtil::format("AssignStringRef UNIMPLEMENTED");

          // Control flow
          // ----------------------------------------------------------------------------

        case Daedalus::EParOp_Ret:
          return bs::StringUtil::format("Return");

        case Daedalus::EParOp_Jump:
          return bs::StringUtil::format("Jump -> {0}", opcode.address);

        case Daedalus::EParOp_JumpIf:
          return bs::StringUtil::format("JumpIf !{0} -> {1}", lhs, opcode.address);

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

    bs::String makeCallDepthString(bs::UINT32 callDepth)
    {
      bs::String depth = "";

      for (bs::INT32 i = 0; i < callDepth; i++)
      {
        bs::INT32 level = i % 3;
        switch (level)
        {
          case 0:
            depth += "|";
            break;
          case 1:
            depth += ";";
            break;
          case 2:
            depth += ".";
            break;
        }
      }

      return depth;
    }
  }  // namespace Scripting
}  // namespace REGoth
