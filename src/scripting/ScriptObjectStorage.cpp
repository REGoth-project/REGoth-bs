#include "ScriptObjectStorage.hpp"
#include <RTTI/RTTI_ScriptObjectStorage.hpp>

namespace REGoth
{
  namespace Scripting
  {
    ScriptObjectStorage::~ScriptObjectStorage()
    {
    }

    bool ScriptObjectStorage::isDestroyed(ScriptObjectHandle handle) const
    {
      if (handle == SCRIPT_OBJECT_HANDLE_INVALID) return true;

      return mObjects.find(handle) == mObjects.end();
    }

    bool ScriptObjectStorage::isValid(ScriptObjectHandle handle) const
    {
      return !isDestroyed(handle);
    }

    ScriptObject& ScriptObjectStorage::create()
    {
      ScriptObject object;
      object.handle = mNextHandle;
      mNextHandle += 1;

      if (object.handle > mNextHandle)
      {
        REGOTH_THROW(InvalidStateException, "Script Object Handle overflow");
      }

      invalidateCache();

      return mObjects.insert({object.handle, std::move(object)}).first->second;
    }

    void ScriptObjectStorage::destroy(ScriptObjectHandle scriptObjectHandle)
    {
      if (isDestroyed(scriptObjectHandle))
      {
        REGOTH_THROW(InvalidStateException, "Script Object was destroyed already!");
      }

      mObjects.erase(scriptObjectHandle);

      invalidateCache();
    }

    ScriptObject& ScriptObjectStorage::get(ScriptObjectHandle handle)
    {
      if (handle == SCRIPT_OBJECT_HANDLE_INVALID)
      {
        REGOTH_THROW(InvalidStateException, "Script Object Handle is invalid!");
      }

      ScriptObject* pCached = findHandleInCache(handle);

      if (pCached)
      {
        return *pCached;
      }
      else
      {
        auto it = mObjects.find(handle);

        if (it == mObjects.end())
        {
          REGOTH_THROW(InvalidStateException, "Script Object Handle does reference a known object!");
        }

        addObjectToCache(handle, it->second);

        return it->second;
      }
    }

    void ScriptObjectStorage::clear()
    {
      mObjects.clear();
      mNextHandle = 1;

      invalidateCache();
    }

    ScriptObject* ScriptObjectStorage::findHandleInCache(ScriptObjectHandle handle) const
    {
      for (bs::UINT32 i = 0; i < ACCESS_CACHE_SIZE; i++)
      {
        if (mAccessCachedHandles[i] == handle)
        {
          return mAccessCachedObjects[i];
        }
      }

      return nullptr;
    }

    void ScriptObjectStorage::addObjectToCache(ScriptObjectHandle handle, ScriptObject& object)
    {
      mAccessCachedHandles[mCachePosition] = handle;
      mAccessCachedObjects[mCachePosition] = &object;

      mCachePosition = (mCachePosition + 1) % ACCESS_CACHE_SIZE;
    }

    void ScriptObjectStorage::invalidateCache()
    {
      for (auto& h : mAccessCachedHandles)
      {
        h = SCRIPT_OBJECT_HANDLE_INVALID;
      }

      for (auto& p : mAccessCachedObjects)
      {
        p = nullptr;
      }

      mCachePosition = 0;
    }

    REGOTH_DEFINE_RTTI(ScriptObjectStorage)

  }  // namespace Scripting
}  // namespace REGoth
