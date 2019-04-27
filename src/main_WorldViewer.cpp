#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include <Components/BsCCamera.h>
#include <Scene/BsSceneObject.h>
#include <components/GameWorld.hpp>

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
    using namespace REGoth;

    HGameWorld world = GameWorld::importZEN("ADDONWORLD.ZEN");
  }

protected:
  bs::HFPSCamera mFPSCamera;
};

int main(int argc, char** argv)
{
  REGothWorldViewer regoth;

  return REGoth::main(regoth, argc, argv);
}
