/**\file
 */
#pragma once
#include "daedalus/DaedalusVMWithExternals.hpp"
#include <BsPrerequisites.h>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * This class implements all scripting related logic, as portable as possible.
     * It inherits from a scripting backend - most likely the DaedalusVM - and implements
     * the script externals, hopefully without depending on daedalus specific details.
     */
    class ScriptVMInterface : public DaedalusVMWithExternals
    {
    public:
      // I don't quite like to reference the dat-file here... This will
      // be annoying when we switch to another scripting backend.
      ScriptVMInterface(const Daedalus::DATFile& datFile);

    protected:
      void print(const bs::String& text) override;
      bs::INT32 HLP_Random(bs::INT32 max) override;
      bs::String IntToString(bs::INT32 i) override;
      bs::String ConcatStrings(const bs::String& a, const bs::String& b) override;
    };

    /**
     * Initializes the scripting system with a Gothic DAT-File.
     * If this is called a second time, everything will be scrapped,
     * all existing handles will be invalidated.
     *
     * @param  gothicDAT  Byte-Data of the GOTHIC.DAT to load.
     */
    void loadGothicDAT(const bs::Vector<bs::UINT8>& gothicDAT);
  }

  /**
   * Global access to the game-scripting interface.
   *
   * Needs to be initialized via Scripting::loadDAT() first.
   */
  Scripting::ScriptVMInterface& gGameScript();
}
