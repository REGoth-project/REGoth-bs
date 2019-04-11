#include "REGothDaedalusVM.hpp"
// #include <daedalus/DaedalusVM.h>
#include "DATSymbolStorageLoader.hpp"
#include "DaedalusClassVarResolver.hpp"
#include <daedalus/DATFile.h>
#include <excepction/Throw.hpp>

namespace REGoth
{
  namespace Scripting
  {
    DaedalusVM::DaedalusVM(const Daedalus::DATFile& datFile)
    {
      // mInternals = bs::bs_shared_ptr_new<DATSymbolStorageLoader>(mScriptSymbols, datFile);
      mDatFile = bs::bs_shared_ptr_new<Daedalus::DATFile>(datFile);
      mClassVarResolver =
          bs::bs_shared_ptr_new<DaedalusClassVarResolver>(mScriptSymbols, mScriptObjects);
    }

    void DaedalusVM::fillSymbolStorage()
    {
      REGoth::Scripting::convertDatToREGothSymbolStorage(mScriptSymbols, *mDatFile);
    }

  }  // namespace Scripting
}  // namespace REGoth
