#include "ScriptSymbols.hpp"

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

      bs::gDebug().logDebug(line);
    }

  }  // namespace Scripting
}  // namespace REGoth
