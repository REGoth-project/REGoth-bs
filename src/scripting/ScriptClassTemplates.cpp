#include "ScriptClassTemplates.hpp"
#include "ScriptSymbolQueries.hpp"
#include <RTTI/RTTI_ScriptClassTemplates.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  namespace Scripting
  {
    ScriptClassTemplates::ScriptClassTemplates()
    {
    }

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
          auto num = ((SymbolInt&)memberSymbol).ints.size();

          obj.ints[name].resize(num, 0);
        }
        else if (memberSymbol.type == SymbolType::Float)
        {
          auto num = ((SymbolFloat&)memberSymbol).floats.size();

          obj.floats[name].resize(num, 0.0f);
        }
        else if (memberSymbol.type == SymbolType::String)
        {
          auto num = ((SymbolString&)memberSymbol).strings.size();

          obj.strings[name].resize(num, "");
        }
        else if (memberSymbol.type == SymbolType::ScriptFunction)
        {
          obj.functionPointers[name] = 0;
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

    REGOTH_DEFINE_RTTI(ScriptClassTemplates)
  }  // namespace Scripting
}  // namespace REGoth
