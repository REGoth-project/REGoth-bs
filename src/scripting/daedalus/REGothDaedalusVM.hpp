/**\file
 */
#pragma once
#include <scripting/ScriptVM.hpp>
#include <BsPrerequisites.h>

namespace Daedalus
{
  class DATFile;
}

namespace REGoth
{
  namespace Scripting
  {
    class DATSymbolStorageLoader;
    class DaedalusVM : public ScriptVM
    {
    public:
      DaedalusVM(const Daedalus::DATFile& datFile);

    private:
      bs::SPtr<DATSymbolStorageLoader> mInternals;
    };
  }  // namespace Scripting
}  // namespace REGoth
