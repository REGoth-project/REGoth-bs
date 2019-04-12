#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include <Components/BsCCamera.h>
#include <Scene/BsSceneObject.h>
#include <daedalus/DATFile.h>
#include <scripting/ScriptVMInterface.hpp>
#include <world/GameWorld.hpp>

class REGothWorldViewer : public REGoth::REGothEngine
{
public:
  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();

    mFPSCamera = mMainCamera->SO()->addComponent<bs::FPSCamera>();
  }

  void setupScene() override
  {
    Daedalus::DATFile dat("/home/andre/games/Gothic II/_work/Data/Scripts/_compiled/GOTHIC.DAT");
    REGoth::Scripting::loadGothicDAT(dat);

    REGoth::World::loadWorldFromZEN("OLDWORLD.ZEN");
  }

protected:
  bs::HFPSCamera mFPSCamera;
};

int main(int argc, char** argv)
{
  REGothWorldViewer regoth;

  return REGoth::main(regoth, argc, argv);
}
