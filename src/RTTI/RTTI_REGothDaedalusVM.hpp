#pragma once

#include "RTTIUtil.hpp"
#include <scripting/daedalus/REGothDaedalusVM.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_DaedalusVM : public bs::RTTIType<DaedalusVM, ScriptVM, RTTI_DaedalusVM>
    {
      using UINT32 = bs::UINT32;

      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(mPC, 0)
      BS_RTTI_MEMBER_PLAIN(mCallDepth, 1)
      BS_RTTI_MEMBER_PLAIN_ARRAY(mDatFileData, 2)
      BS_END_RTTI_MEMBERS

    public:
      RTTI_DaedalusVM()
      {
      }

      void onDeserializationEnded(bs::IReflectable* _obj, bs::SerializationContext* context) override
      {
        auto obj = static_cast<DaedalusVM*>(_obj);

        // Can't save the DAT-files datastructure, but we can save the raw dat-file as
        // a workaround until we come up with something else.
        obj->mDatFile = bs::bs_shared_ptr_new<Daedalus::DATFile>(obj->mDatFileData.data(),
                                                                 obj->mDatFileData.size());

        obj->mClassVarResolver = bs::bs_shared_ptr_new<DaedalusClassVarResolver>(
            obj->mScriptSymbols, obj->mScriptObjects);

        obj->mClassTemplates.createClassTemplates(obj->mScriptSymbols);
        obj->registerAllExternals();
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_ABSTRACT(DaedalusVM)
    };
  }  // namespace Scripting
  // namespace Scripting
}  // namespace REGoth
