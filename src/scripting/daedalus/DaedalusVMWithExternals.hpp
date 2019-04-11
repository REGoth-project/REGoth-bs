/**\file
 */
#pragma once
#include "REGothDaedalusVM.hpp"
#include <BsPrerequisites.h>

namespace REGoth
{
  namespace Scripting
  {
    class IDaedalusScriptInterface
    {
    public:
      virtual void script_PrintPlus(const bs::String& text) = 0;
    };

    class IDaedalusScriptExternals
    {
    protected:
      virtual void print(const bs::String& text) = 0;
      virtual bs::INT32 HLP_Random(bs::INT32 max) = 0;
      virtual bs::String IntToString(bs::INT32 i) = 0;
      virtual bs::String ConcatStrings(const bs::String& a, const bs::String& b) = 0;
    };

    /**
     * DaedalusVM interface to the rest of the engine.
     */
    class DaedalusVMWithExternals : public DaedalusVM,
                                    public IDaedalusScriptInterface,
                                    public IDaedalusScriptExternals
    {
    public:
      DaedalusVMWithExternals(const Daedalus::DATFile& datFile);

      ScriptObjectHandle instanciateClass(const bs::String& className) override;

      void script_PrintPlus(const bs::String& text) override;

      void external_Print();
      void external_HLP_Random();
      void external_IntToString();
      void external_ConcatStrings();

      void registerAllExternals() override;
    };
  }  // namespace Scripting
}  // namespace REGoth
