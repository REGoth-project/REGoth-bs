#pragma once
#include "ScriptTypes.hpp"
#include <BsPrerequisites.h>
#include <exception/Throw.hpp>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * General script object, storing key/value pairs of different types.
     */
    struct ScriptObject
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
    };

    /**
     * Storage for all script objects used by the scripting backend to be used by the
     * native game code.
     *
     * Note that you should always access the storage using a handle. Do not save the
     * reference to the actual data somewhere for later use!
     *
     * The handles used here are just a ever increasing number into a map. Deleted
     * script objects will be taken out of the map, but all other handles will
     * remain intact.
     */
    class ScriptObjectStorage
    {
    public:
      /**
       * @return Whether the given handle references an invalid object.
       */
      bool isDestroyed(ScriptObjectHandle handle) const
      {
        if (handle == SCRIPT_OBJECT_HANDLE_INVALID) return true;

        return mObjects.find(handle) == mObjects.end();
      }

      /**
       * @return Whether the given handle references a valid object.
       */
      bool isValid(ScriptObjectHandle handle) const
      {
        return !isDestroyed(handle);
      }

      /**
       * @return a new blank script object, with only the handle set.
       */
      ScriptObject& create()
      {
        ScriptObject object;
        object.handle = mNextHandle;
        mNextHandle += 1;

        if (object.handle > mNextHandle)
        {
          REGOTH_THROW(InvalidStateException, "Script Object Handle overflow");
        }

        return mObjects.insert({object.handle, std::move(object)}).first->second;
      }

      /**
       * Destroys the given script object. All handles to it will
       * then be invalid.
       */
      void destroy(ScriptObjectHandle scriptObjectHandle)
      {
        if (isDestroyed(scriptObjectHandle))
        {
          REGOTH_THROW(InvalidStateException, "Script Object was destroyed already!");
        }

        mObjects.erase(scriptObjectHandle);
      }

      /**
       * Looks up a script object via its handle.
       *
       * Throws if the handle is invalid.
       *
       * @param  handle  Handle of the object to look up.
       *
       * @return Reference to the script objects data.
       */
      ScriptObject& get(ScriptObjectHandle handle)
      {
        if (handle == SCRIPT_OBJECT_HANDLE_INVALID)
        {
          REGOTH_THROW(InvalidStateException, "Script Object Handle is invalid!");
        }

        auto it = mObjects.find(handle);

        if (it == mObjects.end())
        {
          REGOTH_THROW(InvalidStateException, "Script Object Handle does reference a known object!");
        }

        return it->second;
      }

      /**
       * Removes all script objects created so far and resets the handle counter.
       * All existing handles are to be seen as invalidated after this operation.
       */
      void clear()
      {
        mObjects.clear();
        mNextHandle = 1;
      }

    private:
      bs::Map<ScriptObjectHandle, ScriptObject> mObjects;
      ScriptObjectHandle mNextHandle = 1;
    };

    /**
     * Writes the contents of the given object via gDebug().logDebug().
     */
    void debugLogScriptObject(const ScriptObject& object);
  }  // namespace Scripting
}  // namespace REGoth
