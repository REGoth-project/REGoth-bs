#pragma once

#include "RTTIUtil.hpp"
#include <scripting/ScriptObjectMapping.hpp>

namespace REGoth
{
  namespace Scripting
  {
    using UINT32 = bs::UINT32;

    class RTTI_ScriptObjectMapping
        : public bs::RTTIType<ScriptObjectMapping, bs::IReflectable, RTTI_ScriptObjectMapping>
    {
      BS_BEGIN_RTTI_MEMBERS
      // TODO: Fill RTTI Members
      BS_END_RTTI_MEMBERS

      bs::HSceneObject& getObject(OwnerType* obj, UINT32 idx)
      {
        return mObjects[idx];
      }

      void setObject(OwnerType* obj, UINT32 idx, bs::HSceneObject& val)
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

      ScriptObjectHandle& getScriptHandle(OwnerType* obj, UINT32 idx)
      {
        return mScriptHandles[idx];
      }

      void setScriptHandle(OwnerType* obj, UINT32 idx, ScriptObjectHandle& val)
      {
        mScriptHandles[idx] = val;
      }

      UINT32 getSizeScriptHandles(OwnerType* obj)
      {
        return (UINT32)mScriptHandles.size();
      }

      void setSizeScriptHandles(OwnerType* obj, UINT32 val)
      {
        mScriptHandles.resize(val);
      }

    public:
      RTTI_ScriptObjectMapping()
      {
        addReflectableArrayField("objects", 0,                                //
                                 &RTTI_ScriptObjectMapping::getObject,        //
                                 &RTTI_ScriptObjectMapping::getSizeObjects,   //
                                 &RTTI_ScriptObjectMapping::setObject,        //
                                 &RTTI_ScriptObjectMapping::setSizeObjects);  //

        addPlainArrayField("objectHandles", 1,                                //
                           &RTTI_ScriptObjectMapping::getScriptHandle,        //
                           &RTTI_ScriptObjectMapping::getSizeScriptHandles,   //
                           &RTTI_ScriptObjectMapping::setScriptHandle,        //
                           &RTTI_ScriptObjectMapping::setSizeScriptHandles);  //
      }

      void onSerializationStarted(bs::IReflectable* _obj, bs::SerializationContext* context) override
      {
        auto obj = static_cast<ScriptObjectMapping*>(_obj);

        for (const auto& v : obj->mScriptToSceneObjectMapping)
        {
          mScriptHandles.push_back(v.first);
          mObjects.push_back(v.second);
        }
      }

      void onDeserializationEnded(bs::IReflectable* _obj, bs::SerializationContext* context) override
      {
        auto obj = static_cast<ScriptObjectMapping*>(_obj);

        for (bs::UINT32 i = 0; i < (bs::UINT32)mScriptHandles.size(); i++)
        {
          obj->mScriptToSceneObjectMapping[mScriptHandles[i]] = mObjects[i];
        }
      }
      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(ScriptObjectMapping)

      bs::Vector<bs::HSceneObject> mObjects;
      bs::Vector<ScriptObjectHandle> mScriptHandles;
    };
  }  // namespace Scripting

}  // namespace REGoth
