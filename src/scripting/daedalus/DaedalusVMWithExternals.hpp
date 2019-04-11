/**\file
 */
#pragma once
#include <BsPrerequisites.h>
#include "REGothDaedalusVM.hpp"

namespace REGoth
{
  namespace Scripting
  {
    /**
     * DaedalusVM interface to the rest of the engine.
     */
    class DaedalusVMWithExternals : public DaedalusVM
    {
    public:
      DaedalusVMWithExternals(const Daedalus::DATFile& datFile);

      /**
       * Execute a script function
       */
      void script_PrintPlus(const bs::String& text);
    };
  }  // namespace Scripting
}  // namespace REGoth
