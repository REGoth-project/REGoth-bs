#include "REGothDaedalusVM.hpp"
// #include <daedalus/DaedalusVM.h>
#include <daedalus/DATFile.h>

namespace REGoth
{
  namespace Scripting
  {

    DaedalusVM::DaedalusVM(const Daedalus::DATFile& datFile)
    {
      // mInternals = bs::bs_shared_ptr_new<DATSymbolStorageLoader>(mScriptSymbols, datFile);
    }
  }  // namespace Scripting
}  // namespace REGoth
