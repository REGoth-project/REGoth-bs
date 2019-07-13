#include "ScriptObject.hpp"
#include <RTTI/RTTI_ScriptObject.hpp>

namespace REGoth
{
  namespace Scripting
  {
    void debugLogScriptObject(const ScriptObject& object)
    {
      BS_LOG(Info, Uncategorized, "Dumping object of class: " + object.className);

      for (const auto& ints : object.ints)
      {
        const auto& values = ints.second;

        bs::String line;
        for (const auto& v : values)
        {
          line += bs::toString(v) + " ";
        }

        BS_LOG(Info, Uncategorized,
               bs::StringUtil::format(" - {0} : {1} = {2}", ints.first, "int", line));
      }

      for (const auto& floats : object.floats)
      {
        const auto& values = floats.second;

        bs::String line;
        for (const auto& v : values)
        {
          line += bs::toString(v) + " ";
        }

        BS_LOG(Info, Uncategorized,
               bs::StringUtil::format(" - {0} : {1} = {2}", floats.first, "int", line));
      }

      for (const auto& strings : object.strings)
      {
        const auto& values = strings.second;

        bs::String line;
        for (const auto& v : values)
        {
          line += "'" + v + "' ";
        }

        BS_LOG(Info, Uncategorized,
               bs::StringUtil::format(" - {0} : {1} = {2}", strings.first, "string", line));
      }

      for (const auto& ints : object.functionPointers)
      {
        const auto& value = ints.second;

        BS_LOG(Info, Uncategorized,
               bs::StringUtil::format(" - {0} : {1} = {2}", ints.first, "function", value));
      }

      BS_LOG(Info, Uncategorized, "");
    }

    REGOTH_DEFINE_RTTI(ScriptObject)

  }  // namespace Scripting
}  // namespace REGoth
