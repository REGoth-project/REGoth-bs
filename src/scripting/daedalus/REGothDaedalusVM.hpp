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
    class DaedalusClassVarResolver;
    class DaedalusVM : public ScriptVM
    {
    public:
      DaedalusVM(const Daedalus::DATFile& datFile);

    protected:
      void fillSymbolStorage() override;


    private:
      bs::SPtr<DATSymbolStorageLoader> mInternals;
      bs::SPtr<Daedalus::DATFile> mDatFile;
      bs::SPtr<DaedalusClassVarResolver> mClassVarResolver;
    };
  }  // namespace Scripting
}  // namespace REGoth
