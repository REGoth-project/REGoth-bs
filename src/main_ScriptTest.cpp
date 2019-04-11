#include "REGothEngine.hpp"
#include <scripting/ScriptSymbolStorage.hpp>
#include <scripting/daedalus/DaedalusVMWithExternals.hpp>
#include <daedalus/DATFile.h>

class REGothScriptTester : public REGoth::REGothEngine
{
public:
  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();
  }

  void setupScene() override
  {

    Daedalus::DATFile dat("/home/andre/games/Gothic II/_work/Data/Scripts/_compiled/GOTHIC.DAT");

    REGoth::Scripting::DaedalusVMWithExternals vm(dat);
    vm.initialize();

    vm.script_PrintPlus("Hello World!");
  }

protected:
};

int main(int argc, char** argv)
{
  REGothScriptTester regoth;

  return REGoth::main(regoth, argc, argv);
}
