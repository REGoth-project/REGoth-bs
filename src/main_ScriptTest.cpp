#include "REGothEngine.hpp"
#include <scripting/ScriptSymbolStorage.hpp>
#include <scripting/daedalus/REGothDaedalusVM.hpp>
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

    REGoth::Scripting::DaedalusVM vm(dat);
    vm.initialize();

  }

protected:
};

int main(int argc, char** argv)
{
  REGothScriptTester regoth;

  return REGoth::main(regoth, argc, argv);
}
