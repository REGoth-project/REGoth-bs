#include "REGothEngine.hpp"
#include <scripting/ScriptSymbolStorage.hpp>
#include <scripting/ScriptVMInterface.hpp>
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

    REGoth::Scripting::ScriptVMInterface vm(dat);

    vm.initialize();

    // vm.script_PrintPlus("Hello World!");
    // vm.script_PrintPlus("This is printed by Daedalus!");
    // vm.script_PrintPlus("It also appends a random number to the end, look: ");
    // vm.script_PrintPlus("Pretty cool, eh?");

    vm.instanciateClass("C_ITEM", "ITFO_APPLE");
  }

protected:
};

int main(int argc, char** argv)
{
  REGothScriptTester regoth;

  return REGoth::main(regoth, argc, argv);
}
