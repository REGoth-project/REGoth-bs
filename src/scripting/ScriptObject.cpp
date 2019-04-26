#include "ScriptObject.hpp"
#include <RTTI/RTTI_ScriptObject.hpp>

namespace REGoth
{
  namespace Scripting
  {
    void debugLogScriptObject(const ScriptObject& object)
    {
      bs::gDebug().logDebug("Dumping object of class: " + object.className);

      for (const auto& ints : object.ints)
      {
        const auto& values = ints.second.values;

        bs::String line;
        for (const auto& v : values)
        {
          line += bs::toString(v) + " ";
        }

        bs::gDebug().logDebug(bs::StringUtil::format(" - {0} : {1} = {2}", ints.first, "int", line));
      }

      for (const auto& floats : object.floats)
      {
        const auto& values = floats.second.values;

        bs::String line;
        for (const auto& v : values)
        {
          line += bs::toString(v) + " ";
        }

        bs::gDebug().logDebug(
            bs::StringUtil::format(" - {0} : {1} = {2}", floats.first, "int", line));
      }

      for (const auto& strings : object.strings)
      {
        const auto& values = strings.second.values;

        bs::String line;
        for (const auto& v : values)
        {
          line += "'" + v + "' ";
        }

        bs::gDebug().logDebug(
            bs::StringUtil::format(" - {0} : {1} = {2}", strings.first, "string", line));
      }

      for (const auto& ints : object.functionPointers)
      {
        const auto& value = ints.second;

        bs::gDebug().logDebug(
            bs::StringUtil::format(" - {0} : {1} = {2}", ints.first, "function", value));
      }

      bs::gDebug().logDebug("");
    }

    REGOTH_DEFINE_RTTI(ScriptObject)

  }  // namespace Scripting
}  // namespace REGoth
