#pragma once
#include "ScriptObjects.hpp"
#include <BsPrerequisites.h>

namespace Daedalus
{
  class DATFile;
}

namespace REGoth
{
  namespace Scripting
  {
    class ScriptSymbolStorage;

    /**
     * Will take the given DAT-File, convert all symbols inside and place them
     * inside the given script symbol storage.
     *
     * @param  storage  The target script symbol storage.
     * @param  datFile  The input DAT-File to convert from.
     */
    void convertDatToREGothSymbolStorage(ScriptSymbolStorage& storage,
                                         const Daedalus::DATFile& datFile);
  }
}
