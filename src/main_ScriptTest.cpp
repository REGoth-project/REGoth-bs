#include "REGothEngine.hpp"
#include <Components/BsCCamera.h>
#include <Scene/BsSceneObject.h>
#include <components/Item.hpp>
#include <daedalus/DATFile.h>
#include <original-content/VirtualFileSystem.hpp>
#include <scripting/ScriptSymbolStorage.hpp>
#include <components/GameWorld.hpp>

class REGothScriptTester : public REGoth::REGothEngine
{
public:
  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();
  }

  void setupScene() override
  {
    using namespace REGoth;
    using ScriptObject       = Scripting::ScriptObject;
    using ScriptObjectHandle = Scripting::ScriptObjectHandle;

    HGameWorld world = GameWorld::createEmpty();

    // ScriptObjectHandle appleHandle = gGameScript().instanciateClass("C_ITEM", "ITFO_APPLE");

    // ScriptObject& apple = REGoth::gGameScript().scriptObjects().get(appleHandle);

    // bs::gDebug().logDebug("Apple's visual: " + apple.stringValue("VISUAL"));

    // gGameScript().script_PrintPlus("Hello World!");
    // gGameScript().script_PrintPlus("This is printed by Daedalus!");
    // gGameScript().script_PrintPlus("It also appends a random number to the end, look: ");
    // gGameScript().script_PrintPlus("Pretty cool, eh?");

    world->insertItem("ITFO_APPLE", bs::Transform::IDENTITY);

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
