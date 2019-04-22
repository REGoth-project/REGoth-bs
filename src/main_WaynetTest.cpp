#include "components/AnchoredTextLabels.h"
#include <GUI/BsCGUIWidget.h>
#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include <Components/BsCCamera.h>
#include <Scene/BsSceneObject.h>
#include <components/Item.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <daedalus/DATFile.h>
#include <excepction/Throw.hpp>
#include <original-content/VirtualFileSystem.hpp>
#include <scripting/ScriptVMInterface.hpp>
#include <world/GameWorld.hpp>

class REGothWaynetTester : public REGoth::REGothEngine
{
public:
  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();

    mMainCamera->SO()->addComponent<bs::FPSCamera>();

    auto guiSO = bs::SceneObject::create("GUI");
    auto guiWidget = guiSO->addComponent<bs::CGUIWidget>(mMainCamera);

    auto debugOverlaySO = bs::SceneObject::create("DebugOverlay");
    mTextLabels         = debugOverlaySO->addComponent<REGoth::AnchoredTextLabels>(guiWidget);
    mTextLabels->setMaximumDistance(50.f);
  }

  void setupScene() override
  {
    using namespace REGoth;

    Scripting::loadGothicDAT(gVirtualFileSystem().readFile("GOTHIC.DAT"));

    World::loadWorldFromZEN("OLDWORLD.ZEN", World::GameWorld::Init::NoInitScripts);

    bs::HSceneObject waynetSO = gWorld().sceneRoot()->findChild("Waynet");

    if (!waynetSO)
    {
      REGOTH_THROW(InvalidStateException, "No waynet found in this world?");
    }

    HWaynet waynet = waynetSO->getComponent<Waynet>();

    if (!waynet)
    {
      REGOTH_THROW(InvalidStateException, "Waynet does not have Waynet component?");
    }

    waynet->debugDraw(mTextLabels);
  }

protected:
  REGoth::HAnchoredTextLabels mTextLabels;
};

int main(int argc, char** argv)
{
  REGothWaynetTester regoth;

  return REGoth::main(regoth, argc, argv);
}
