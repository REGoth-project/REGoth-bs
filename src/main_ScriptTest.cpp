#include "REGothEngine.hpp"
#include <scripting/ScriptSymbolStorage.hpp>
#include <scripting/daedalus/DATSymbolStorageLoader.hpp>
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

    REGoth::Scripting::ScriptSymbolStorage symbolStorage;
    Daedalus::DATFile dat("/home/andre/games/Gothic II/_work/Data/Scripts/_compiled/GOTHIC.DAT");

    REGoth::Scripting::convertDatToREGothSymbolStorage(symbolStorage, dat);
  }

protected:
};

int main(int argc, char** argv)
{
  REGothScriptTester regoth;

  return REGoth::main(regoth, argc, argv);
}
