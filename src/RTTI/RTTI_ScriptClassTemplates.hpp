#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <scripting/ScriptClassTemplates.hpp>

namespace REGoth
{
  namespace Scripting
  {
    using UINT32 = bs::UINT32;

    class RTTI_ScriptClassTemplates
        : public bs::RTTIType<ScriptClassTemplates, bs::IReflectable, RTTI_ScriptClassTemplates>
    {
      BS_BEGIN_RTTI_MEMBERS
      // TODO: Fill RTTI Members
      BS_END_RTTI_MEMBERS

      ScriptObject& getTemplate(OwnerType* obj, UINT32 idx)
      {
        return mTemplates[idx];
      }

      void setTemplate(OwnerType* obj, UINT32 idx, ScriptObject& val)
      {
        mTemplates[idx] = val;
      }

      UINT32 getSizeTemplates(OwnerType* obj)
      {
        return (UINT32)mTemplates.size();
      }

      void setSizeTemplates(OwnerType* obj, UINT32 val)
      {
        mTemplates.resize(val);
      }

      bs::String& getClassName(OwnerType* obj, UINT32 idx)
      {
        return mClassNames[idx];
      }

      void setClassName(OwnerType* obj, UINT32 idx, bs::String& val)
      {
        mClassNames[idx] = val;
      }

      UINT32 getSizeClassNames(OwnerType* obj)
      {
        return (UINT32)mClassNames.size();
      }

      void setSizeClassNames(OwnerType* obj, UINT32 val)
      {
        mClassNames.resize(val);
      }

    public:
      RTTI_ScriptClassTemplates()
      {
        addReflectableArrayField("templates", 0,                                //
                                 &RTTI_ScriptClassTemplates::getTemplate,        //
                                 &RTTI_ScriptClassTemplates::getSizeTemplates,   //
                                 &RTTI_ScriptClassTemplates::setTemplate,        //
                                 &RTTI_ScriptClassTemplates::setSizeTemplates);  //

        addPlainArrayField("classNames", 1,                                //
                           &RTTI_ScriptClassTemplates::getClassName,        //
                           &RTTI_ScriptClassTemplates::getSizeClassNames,   //
                           &RTTI_ScriptClassTemplates::setClassName,        //
                           &RTTI_ScriptClassTemplates::setSizeClassNames);  //
      }

      void onSerializationStarted(ScriptClassTemplates* obj)
      {
        for (const auto& v : obj->mClassTemplates)
        {
          mClassNames.push_back(v.first);
          mTemplates.push_back(v.second);
        }
      }

      void onDeserializationEnded(ScriptClassTemplates* obj)
      {
        for (bs::UINT32 i = 0; i < (bs::UINT32)mClassNames.size(); i++)
        {
          obj->mClassTemplates[mClassNames[i]] = mTemplates[i];
        }
      }

      bs::SPtr<bs::IReflectable> newRTTIObject() override
      {
        return bs::bs_shared_ptr_new<ScriptObject>();
      }

      const bs::String& getRTTIName() override
      {
        static bs::String name = "ScriptClassTemplates";
        return name;
      }

      bs::UINT32 getRTTIId() override
      {
        return TID_REGOTH_ScriptClassTemplates;
      }

      bs::Vector<ScriptObject> mTemplates;
      bs::Vector<bs::String> mClassNames;
    };
  }  // namespace Scripting
  // namespace Scripting
}  // namespace REGoth
