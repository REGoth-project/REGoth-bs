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

      /**
       * Checks whether the script object behind the given handle is cached.
       * If so, a pointer to that object is returned. If it was not cached,
       * nullptr is returned.
       */
      ScriptObject* findHandleInCache(ScriptObjectHandle handle) const;

      /**
       * Adds the given object to the cache. The oldest object inside the
       * cache will be overridden.
       */
      void addObjectToCache(ScriptObjectHandle handle, ScriptObject& object);

      /**
       * Invalidates the cache and resets it to default, leaving it completely empty.
       */
      void invalidateCache();

      /**
       * Cache for accessing script objects via handle. Since the objects are
       * stored within a bs::Map, access to them is rather slow. To speed up
       * accessing of the same objects over and over again, the last few
       * objects are cached here.
       *
       * The cache is implemented using two std::arrays, so the data they hold
       * is local to the class and can be accessed without an indirection.
       *
       * There are two arrays: The cached handles and pointers to actual
       * objects. Both arrays index the same way: Index i in one array belongs
       * to index i in the other one.
       *
       * When a new object is supposed to be cached, the oldest entry is overwritten.
       *
       * Since it's not required for the bs::Map to keep all objects at the
       * same addresses, the cache is cleaned once the object map is updated.
       * However, most implementations should leave the data where it is, so
       * we might be able to keep the cache on those implementations of bs::Map.
       * If we ever do that, we only have to take care of invalidating the cache
       * when removing objects.
       */
      static constexpr bs::UINT32 ACCESS_CACHE_SIZE = 5;
      std::array<ScriptObjectHandle, ACCESS_CACHE_SIZE> mAccessCachedHandles;
      std::array<ScriptObject*, ACCESS_CACHE_SIZE> mAccessCachedObjects;
      bs::UINT32 mCachePosition = 0;

    public:
      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(ScriptObjectStorage)
    };
  }
}
