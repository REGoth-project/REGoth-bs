#pragma once
#include "ScriptObject.hpp"

namespace REGoth
{
  namespace Scripting
  {
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
  }
}
