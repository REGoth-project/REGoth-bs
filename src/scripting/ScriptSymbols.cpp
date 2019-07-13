#include "ScriptSymbols.hpp"
#include <RTTI/RTTI_ScriptSymbols.hpp>

namespace REGoth
{
  namespace Scripting
  {
    bs::String symbolTypeToString(SymbolType type)
    {
      switch (type)
      {
        case SymbolType::Float:
          return "Float";
        case SymbolType::Int:
          return "Int";
        case SymbolType::String:
          return "String";
        case SymbolType::Class:
          return "Class";
        case SymbolType::ScriptFunction:
          return "ScriptFunction";
        case SymbolType::ExternalFunction:
          return "ExternalFunction";
        case SymbolType::Prototype:
          return "Prototype";
        case SymbolType::Instance:
          return "Instance";
        case SymbolType::Unsupported:
          return "Unsupported";
        default:
          return "Unknown";
      }
    }

    void debugLogSymbol(const SymbolBase& symbol)
    {
      SymbolType type = symbol.type;

      bs::String line = "[" + symbol.name + " : " + symbolTypeToString(type) + "] ";

      if (symbol.isClassVar)
      {
        line += "[CV] ";
      }

      if (symbol.isKeptAfterLoad)
      {
        line += "[KAL] ";
      }

      else if (type == SymbolType::Float)
      {
      }
      else if (type == SymbolType::Int)
      {
      }
      else if (type == SymbolType::String)
      {
      }
      else if (type == SymbolType::Class)
      {
      }
      else if (type == SymbolType::ScriptFunction)
      {
      }
      else if (type == SymbolType::ExternalFunction)
      {
      }
      else if (type == SymbolType::Prototype)
      {
      }
      else if (type == SymbolType::Instance)
      {
      }
      else /* if (type == SymbolType::Unsupported) */
      {
      }

      BS_LOG(Info, Uncategorized, line);
    }

    REGOTH_DEFINE_RTTI(SymbolBase)
    REGOTH_DEFINE_RTTI(SymbolInt)
    REGOTH_DEFINE_RTTI(SymbolFloat)
    REGOTH_DEFINE_RTTI(SymbolString)
    REGOTH_DEFINE_RTTI(SymbolClass)
    REGOTH_DEFINE_RTTI(SymbolScriptFunction)
    REGOTH_DEFINE_RTTI(SymbolExternalFunction)
    REGOTH_DEFINE_RTTI(SymbolPrototype)
    REGOTH_DEFINE_RTTI(SymbolInstance)
    REGOTH_DEFINE_RTTI(SymbolUnsupported)
  }  // namespace Scripting
}  // namespace REGoth
