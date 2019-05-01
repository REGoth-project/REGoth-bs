#pragma once

#include "RTTIUtil.hpp"
#include <scripting/daedalus/REGothDaedalusVM.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_DaedalusVM : public bs::RTTIType<DaedalusVM, ScriptVM, RTTI_DaedalusVM>
    {
      BS_BEGIN_RTTI_MEMBERS
      // TODO: Fill RTTI Members
      BS_END_RTTI_MEMBERS

    public:
      RTTI_DaedalusVM()
      {
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_ABSTRACT(DaedalusVM)
    };
  }  // namespace Scripting
  // namespace Scripting
}  // namespace REGoth
