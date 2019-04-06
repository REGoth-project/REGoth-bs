#include "BsFPSCamera.h"
#include "BsFPSWalker.h"
#include "REGothEngine.hpp"
#include "original-content/VirtualFileSystem.hpp"
#include <Components/BsCCamera.h>
#include <Math/BsVector3.h>
#include <Scene/BsSceneObject.h>
#include <world/ConstructFromZEN.hpp>

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
    REGoth::World::constructFromZEN("OLDWORLD.ZEN");
  }

protected:
  bs::HFPSCamera mFPSCamera;
};

int main(int argc, char** argv)
{
  REGothWorldViewer regoth;

  return REGoth::main(regoth, argc, argv);
}
