#pragma once

#include "RTTIUtil.hpp"
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
        // FIXME: Breaks with an obscure error
        // addReflectableArrayField("templates", 0,                                //
        //                          &RTTI_ScriptClassTemplates::getTemplate,        //
        //                          &RTTI_ScriptClassTemplates::getSizeTemplates,   //
        //                          &RTTI_ScriptClassTemplates::setTemplate,        //
        //                          &RTTI_ScriptClassTemplates::setSizeTemplates);  //

        // addPlainArrayField("classNames", 1,                                //
        //                    &RTTI_ScriptClassTemplates::getClassName,        //
        //                    &RTTI_ScriptClassTemplates::getSizeClassNames,   //
        //                    &RTTI_ScriptClassTemplates::setClassName,        //
        //                    &RTTI_ScriptClassTemplates::setSizeClassNames);  //
      }

      void onSerializationStarted(bs::IReflectable* _obj, bs::SerializationContext* context) override
      {
        auto obj = static_cast<ScriptClassTemplates*>(_obj);

        for (const auto& v : obj->mClassTemplates)
        {
          mClassNames.push_back(v.first);
          mTemplates.push_back(v.second);
        }
      }

      void onDeserializationEnded(bs::IReflectable* _obj, bs::SerializationContext* context) override
      {
        auto obj = static_cast<ScriptClassTemplates*>(_obj);

        for (bs::UINT32 i = 0; i < (bs::UINT32)mClassNames.size(); i++)
        {
          obj->mClassTemplates[mClassNames[i]] = mTemplates[i];
        }
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(ScriptClassTemplates)

      bs::Vector<ScriptObject> mTemplates;
      bs::Vector<bs::String> mClassNames;
    };
  }  // namespace Scripting
  // namespace Scripting
}  // namespace REGoth
