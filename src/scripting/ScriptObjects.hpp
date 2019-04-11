#pragma once
#include "ScriptTypes.hpp"
#include <BsPrerequisites.h>
#include <excepction/Throw.hpp>

namespace REGoth
{
  namespace Scripting
  {
    struct ScriptInts
    {
      bs::Vector<bs::INT32> values;
    };

    struct ScriptFloats
    {
      bs::Vector<float> values;
    };

    struct ScriptStrings
    {
      bs::Vector<bs::String> values;
    };

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
       */
      bs::Map<bs::String, ScriptInts> ints;
      bs::Map<bs::String, ScriptFloats> floats;
      bs::Map<bs::String, ScriptStrings> strings;
      bs::Map<bs::String, SymbolIndex> functionPointers;
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
      ScriptObject& createScriptObject()
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
  }
}
