#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <scripting/daedalus/REGothDaedalusVM.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_DaedalusVM
        : public bs::RTTIType<DaedalusVM, ScriptVM, RTTI_DaedalusVM>
    {
      BS_BEGIN_RTTI_MEMBERS
      // TODO: Fill RTTI Members
      BS_END_RTTI_MEMBERS

    public:
      RTTI_DaedalusVM()
      {
      }

      bs::SPtr<bs::IReflectable> newRTTIObject() override
      {
        REGOTH_THROW(InvalidParametersException, "Cannot create instance of abstract class!");
        return nullptr;
      }

      const bs::String& getRTTIName() override
      {
        static bs::String name = "DaedalusVM";
        return name;
      }

      bs::UINT32 getRTTIId() override
      {
        return TID_REGOTH_DaedalusVM;
      }
    };
  }  // namespace Scripting
  // namespace Scripting
}  // namespace REGoth
