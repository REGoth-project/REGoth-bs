#include "ScriptClassTemplates.hpp"
#include "ScriptSymbolQueries.hpp"
#include <excepction/Throw.hpp>

namespace REGoth
{
  namespace Scripting
  {
    void ScriptClassTemplates::createClassTemplates(const ScriptSymbolStorage& scriptSymbols)
    {
      auto classes = Queries::findAllClasses(scriptSymbols);

      for (SymbolIndex parent : classes)
      {
        SymbolClass& classSymbol = scriptSymbols.getSymbol<SymbolClass>(parent);

        auto members = Queries::findAllWithParentOf(scriptSymbols, parent);

        ScriptObject classTemplate = createClassTemplate(classSymbol.name, members, scriptSymbols);

        mClassTemplates[classSymbol.name] = classTemplate;
      }
    }

    ScriptObject ScriptClassTemplates::createClassTemplate(const bs::String& className,
                                                           const bs::Vector<SymbolIndex>& members,
                                                           const ScriptSymbolStorage& scriptSymbols)
    {
      ScriptObject obj;
      obj.className = className;

      for (SymbolIndex memberSymbolIndex : members)
      {
        SymbolBase& memberSymbol = scriptSymbols.getSymbolBase(memberSymbolIndex);
        bs::String name          = demangleMemberName(memberSymbol.name);

        if (memberSymbol.type == SymbolType::Int)
        {
          auto num = ((SymbolInt&)memberSymbol).ints.values.size();

          obj.ints[name].values.resize(num);
        }
        else if (memberSymbol.type == SymbolType::Float)
        {
          auto num = ((SymbolFloat&)memberSymbol).floats.values.size();

          obj.floats[name].values.resize(num);
        }
        else if (memberSymbol.type == SymbolType::String)
        {
          auto num = ((SymbolString&)memberSymbol).strings.values.size();

          obj.strings[name].values.resize(num);
        }
        else if (memberSymbol.type == SymbolType::ScriptFunction)
        {
          obj.functionPointers[name] = SYMBOL_INDEX_INVALID;
        }
        else
        {
          REGOTH_THROW(InvalidParametersException,
                       "Unexpected member symbol type: " + symbolTypeToString(memberSymbol.type));
        }
      }

      return obj;
    }

    bs::String ScriptClassTemplates::demangleMemberName(const bs::String& memberSymbolName)
    {
      auto nameParts = bs::StringUtil::split(memberSymbolName, ".");

      if (nameParts.size() != 2) return memberSymbolName;

      return nameParts[1];
    }

    const ScriptObject& ScriptClassTemplates::getClassTemplate(const bs::String& className) const
    {
      auto it = mClassTemplates.find(className);

      if (it == mClassTemplates.end())
      {
        REGOTH_THROW(InvalidParametersException, "Cannot find template for class " + className);
      }

      return it->second;
    }

  }  // namespace Scripting
}  // namespace REGoth
