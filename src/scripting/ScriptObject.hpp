#pragma once
#include "ScriptTypes.hpp"
#include <BsPrerequisites.h>
#include <exception/Throw.hpp>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * General script object, storing key/value pairs of different types.
     */
    struct ScriptObject : public bs::IReflectable
    {
      /**
       * Name of the script class this object represents
       */
      bs::String className;

      /**
       * Handle referencing this very script object. Should not be modified!
       */
      ScriptObjectHandle handle;

      /**
       * Data storage mapping. Each of these holds a key/value pair to describe the
       * member variables of the object like in this example:
       *
       *     int healh;
       *     int attributes[50];
       *     string name;
       *
       * Note that there are save access methods below.
       */
      bs::Map<bs::String, ScriptInts> ints;
      bs::Map<bs::String, ScriptFloats> floats;
      bs::Map<bs::String, ScriptStrings> strings;
      bs::Map<bs::String, bs::UINT32> functionPointers;

      /**
       * Save access to a string value. Throws if the value does not exist.
       */
      bs::String& stringValue(const bs::String& name, bs::UINT32 arrayIndex = 0)
      {
        auto it = strings.find(name);

        if (it == strings.end())
        {
          throwVariableDoesNotExist(name, "String");
        }

        if (arrayIndex >= it->second.values.size())
        {
          throwArrayOutOfRange(name, "String", arrayIndex);
        }

        return it->second.values[arrayIndex];
      }

      /**
       * Save access to a float value. Throws if the value does not exist.
       */
      float& floatValue(const bs::String& name, bs::UINT32 arrayIndex = 0)
      {
        auto it = floats.find(name);

        if (it == floats.end())
        {
          throwVariableDoesNotExist(name, "Float");
        }

        if (arrayIndex >= it->second.values.size())
        {
          throwArrayOutOfRange(name, "Float", arrayIndex);
        }

        return it->second.values[arrayIndex];
      }

      /**
       * Save access to an int value. Throws if the value does not exist.
       */
      bs::INT32& intValue(const bs::String& name, bs::UINT32 arrayIndex = 0)
      {
        auto it = ints.find(name);

        if (it == ints.end())
        {
          throwVariableDoesNotExist(name, "Int");
        }

        if (arrayIndex >= it->second.values.size())
        {
          throwArrayOutOfRange(name, "Int", arrayIndex);
        }

        return it->second.values[arrayIndex];
      }

      void throwVariableDoesNotExist(const bs::String& name, const bs::String& type)
      {
        REGOTH_THROW(InvalidParametersException, "ScriptObject of class " + className +
                                                     " has no member " + name + " of type " + type);
      }

      void throwArrayOutOfRange(const bs::String& name, const bs::String& type, bs::UINT32 index)
      {
        REGOTH_THROW(InvalidParametersException, "Array-Index out of range in class " + className +
                                                     " at member " + name + " of type " + type +
                                                     " with index " + bs::toString(index));
      }

    public:
      REGOTH_DECLARE_RTTI(ScriptObject);
    };

    /**
     * Writes the contents of the given object via gDebug().logDebug().
     */
    void debugLogScriptObject(const ScriptObject& object);
  }  // namespace Scripting
}  // namespace REGoth
