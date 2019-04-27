#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Reflection/BsRTTIType.h>
#include <scripting/ScriptObjectStorage.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_ScriptObjectStorage
        : public bs::RTTIType<ScriptObjectStorage, bs::IReflectable, RTTI_ScriptObjectStorage>
    {
      using UINT32 = bs::UINT32;

      BS_BEGIN_RTTI_MEMBERS
      // BS_RTTI_MEMBER_PLAIN(mObjects, 0) // Commented out: Added manually, see constructor
      // BS_RTTI_MEMBER_PLAIN(mObjectHandles, 1) // Commented out: Added manually, see constructor
      BS_RTTI_MEMBER_PLAIN(mNextHandle, 2)
      BS_END_RTTI_MEMBERS

      ScriptObject& getObject(OwnerType* obj, UINT32 idx)
      {
        return mObjects[idx];
      }

      void setObject(OwnerType* obj, UINT32 idx, ScriptObject& val)
      {
        mObjects[idx] = val;
      }

      UINT32 getSizeObjects(OwnerType* obj)
      {
        return (UINT32)mObjects.size();
      }

      void setSizeObjects(OwnerType* obj, UINT32 val)
      {
        mObjects.resize(val);
      }

      ScriptObjectHandle& getObjectHandle(OwnerType* obj, UINT32 idx)
      {
        return mObjectHandles[idx];
      }

      void setObjectHandle(OwnerType* obj, UINT32 idx, ScriptObjectHandle& val)
      {
        mObjectHandles[idx] = val;
      }

      UINT32 getSizeObjectHandles(OwnerType* obj)
      {
        return (UINT32)mObjectHandles.size();
      }

      void setSizeObjectHandles(OwnerType* obj, UINT32 val)
      {
        mObjectHandles.resize(val);
      }

    public:
      RTTI_ScriptObjectStorage()
      {
        addReflectableArrayField("objects", 0,                                //
                                 &RTTI_ScriptObjectStorage::getObject,        //
                                 &RTTI_ScriptObjectStorage::getSizeObjects,   //
                                 &RTTI_ScriptObjectStorage::setObject,        //
                                 &RTTI_ScriptObjectStorage::setSizeObjects);  //

        addPlainArrayField("objectHandles", 1,                                //
                           &RTTI_ScriptObjectStorage::getObjectHandle,        //
                           &RTTI_ScriptObjectStorage::getSizeObjectHandles,   //
                           &RTTI_ScriptObjectStorage::setObjectHandle,        //
                           &RTTI_ScriptObjectStorage::setSizeObjectHandles);  //
      }

      void onSerializationStarted(ScriptObjectStorage* obj)
      {
        for (const auto& v : obj->mObjects)
        {
          mObjectHandles.push_back(v.first);
          mObjects.push_back(v.second);
        }
      }

      void onDeserializationEnded(ScriptObjectStorage* obj)
      {
        for (bs::UINT32 i = 0; i < (bs::UINT32)mObjectHandles.size(); i++)
        {
          obj->mObjects[mObjectHandles[i]] = mObjects[i];
        }
      }

      bs::SPtr<bs::IReflectable> newRTTIObject() override
      {
        return bs::bs_shared_ptr_new<ScriptObjectStorage>();
      }

      const bs::String& getRTTIName() override
      {
        static bs::String name = "ScriptObjectStorage";
        return name;
      }

      bs::UINT32 getRTTIId() override
      {
        return TID_REGOTH_ScriptObjectStorage;
      }

      bs::Vector<ScriptObject> mObjects;
      bs::Vector<ScriptObjectHandle> mObjectHandles;
    };

  }  // namespace Scripting
}  // namespace REGoth
