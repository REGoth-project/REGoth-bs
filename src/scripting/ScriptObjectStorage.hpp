#pragma once
#include "ScriptObject.hpp"
#include <RTTI/RTTIUtil.hpp>

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
    class ScriptObjectStorage : public bs::IReflectable
    {
    public:
      ScriptObjectStorage() = default;
      virtual ~ScriptObjectStorage();

      /**
       * @return Whether the given handle references an invalid object.
       */
      bool isDestroyed(ScriptObjectHandle handle) const;

      /**
       * @return Whether the given handle references a valid object.
       */
      bool isValid(ScriptObjectHandle handle) const;

      /**
       * @return a new blank script object, with only the handle set.
       */
      ScriptObject& create();

      /**
       * Destroys the given script object. All handles to it will
       * then be invalid.
       */
      void destroy(ScriptObjectHandle scriptObjectHandle);

      /**
       * Looks up a script object via its handle.
       *
       * Throws if the handle is invalid.
       *
       * @param  handle  Handle of the object to look up.
       *
       * @return Reference to the script objects data.
       */
      ScriptObject& get(ScriptObjectHandle handle);

      /**
       * Removes all script objects created so far and resets the handle counter.
       * All existing handles are to be seen as invalidated after this operation.
       */
      void clear();

    private:
      bs::Map<ScriptObjectHandle, ScriptObject> mObjects;
      ScriptObjectHandle mNextHandle = 1;

    public:
      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(ScriptObjectStorage)
    };
  }
}
