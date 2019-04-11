/**\file
 */
#pragma once
#include <BsPrerequisites.h>
#include <scripting/ScriptVM.hpp>

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

    protected:
      void fillSymbolStorage() override;


    private:
      bs::SPtr<DATSymbolStorageLoader> mInternals;
      bs::SPtr<Daedalus::DATFile> mDatFile;

    };
  }  // namespace Scripting
}  // namespace REGoth
