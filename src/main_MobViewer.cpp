#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include <BsZenLib/ImportSkeletalMesh.hpp>
#include <Components/BsCCamera.h>
#include <Scene/BsSceneObject.h>
#include <components/VisualInteractiveObject.hpp>
#include <original-content/VirtualFileSystem.hpp>

class REGothMobViewer : public REGoth::REGothEngine
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

    for (auto s : gVirtualFileSystem().listAllFiles())
    {
      bs::gDebug().logDebug(s);
    }

    bs::HSceneObject mobSO = bs::SceneObject::create("Mob");

    HVisualInteractiveObject mobVis = mobSO->addComponent<VisualInteractiveObject>();

    // auto mds = BsZenLib::ImportAndCacheMDS("CHESTBIG_OCCHESTMEDIUM.MDS",
    //                                        gVirtualFileSystem().getFileIndex());

    // for (auto s : mds->getMeshes())
    // {
    //   bs::gDebug().logDebug(s->getName());
    // }

    mobVis->setVisual("CHESTBIG_OCCHESTMEDIUM.MDS");

    mMainCamera->SO()->setPosition(bs::Vector3(1, 0, 0));
    mMainCamera->SO()->lookAt(bs::Vector3(0, 0, 0));
  }

protected:
  bs::HFPSCamera mFPSCamera;
};

int main(int argc, char** argv)
{
  REGothMobViewer regoth;

  return REGoth::main(regoth, argc, argv);
}
