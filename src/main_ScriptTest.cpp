#include "REGothEngine.hpp"
#include <scripting/ScriptSymbolStorage.hpp>
#include <scripting/ScriptVMInterface.hpp>
#include <daedalus/DATFile.h>
#include <Components/BsCCamera.h>
#include <Scene/BsSceneObject.h>
#include <components/Item.hpp>

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
    REGoth::Scripting::loadGothicDAT(dat);

    REGoth::gGameScript().instanciateClass("C_ITEM", "ITFO_APPLE");

    REGoth::gGameScript().script_PrintPlus("Hello World!");
    REGoth::gGameScript().script_PrintPlus("This is printed by Daedalus!");
    REGoth::gGameScript().script_PrintPlus("It also appends a random number to the end, look: ");
    REGoth::gGameScript().script_PrintPlus("Pretty cool, eh?");

    bs::HSceneObject item = bs::SceneObject::create("Apple");
    item->addComponent<REGoth::Item>("ITFO_APPLE");

    mMainCamera->SO()->setPosition(bs::Vector3(0, 1, 1));
    mMainCamera->SO()->lookAt(bs::Vector3(0, 0, 0));
  }

protected:
};

int main(int argc, char** argv)
{
  REGothScriptTester regoth;

  return REGoth::main(regoth, argc, argv);
}
