#pragma once

#include "RTTIUtil.hpp"
#include <scripting/ScriptVM.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_ScriptVM : public bs::RTTIType<ScriptVM, bs::IReflectable, RTTI_ScriptVM>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_REFL(mScriptSymbols, 1)
      BS_RTTI_MEMBER_REFL(mScriptObjects, 2)
      // BS_RTTI_MEMBER_REFL(mClassTemplates, 3) // Commented out: Can re-create after
      // deserialization
      BS_RTTI_MEMBER_REFL(mScriptObjectMapping, 4)
      BS_END_RTTI_MEMBERS

    public:
      RTTI_ScriptVM()
      {
      }

      void onDeserializationEnded(bs::IReflectable* _obj, bs::SerializationContext* context) override
      {
        auto obj = static_cast<ScriptVM*>(_obj);

        obj->mClassTemplates.createClassTemplates(obj->mScriptSymbols);
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_ABSTRACT(ScriptVM)
    };
  }  // namespace Scripting

}  // namespace REGoth
