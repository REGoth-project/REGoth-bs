#include <memory>

#include <BsCameraZoomer.h>
#include <BsFPSCamera.h>
#include <BsFPSWalker.h>
#include <BsObjectRotator.h>
#include <Components/BsCCamera.h>
#include <Components/BsCCharacterController.h>
#include <Components/BsCRenderable.h>
#include <Debug/BsDebugDraw.h>
#include <Input/BsVirtualInput.h>
#include <Math/BsVector3.h>
#include <Scene/BsSceneObject.h>
#include <Utility/BsTime.h>

#include <BsZenLib/ImportAnimation.hpp>
#include <BsZenLib/ImportMorphMesh.hpp>
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ImportSkeletalMesh.hpp>
#include <BsZenLib/ImportZEN.hpp>
#include <daedalus/DATFile.h>

#include <animation/StateNaming.hpp>
#include <components/Character.hpp>
#include <components/GameWorld.hpp>
#include <components/VisualCharacter.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <core/Engine.hpp>
#include <log/logging.hpp>
#include <original-content/VirtualFileSystem.hpp>

class REGothCharacterViewer : public REGoth::Engine
{
public:
  using REGoth::Engine::Engine;

  void setupMainCamera() override
  {
    REGoth::AbstractEngine::setupMainCamera();

    // mFPSCamera = mMainCamera->SO()->addComponent<bs::FPSCamera>();
    mMainCamera->SO()->addComponent<bs::CameraZoomer>();
  }

  void setupInput() override
  {
    using namespace bs;

    REGoth::AbstractEngine::setupInput();

    auto inputConfig = gVirtualInput().getConfiguration();

    inputConfig->registerButton("RotateObj", BC_MOUSE_LEFT);
    inputConfig->registerButton("RotateCam", BC_MOUSE_RIGHT);
    inputConfig->registerAxis("Zoom", VIRTUAL_AXIS_DESC((UINT32)InputAxis::MouseZ));
  }

  void setupScene() override
  {
    using namespace bs;
    using namespace REGoth;

    for (auto s : gVirtualFileSystem().listAllFiles())
    {
      REGOTH_LOG(Info, Uncategorized, s);
    }

    HGameWorld world = GameWorld::createEmpty();

    HSceneObject playerSO = SceneObject::create("Player");

    bs::HRenderable renderable = playerSO->addComponent<bs::CRenderable>();
    // REGoth::HVisualCharacter playerVisual = playerSO->addComponent<REGoth::VisualCharacter>();
    playerSO->addComponent<bs::ObjectRotator>();

    // Load a model and its animations
    BsZenLib::Res::HModelScriptFile model;

    const String file   = "HUMANS.MDS";
    const String visual = "HUM_BODY_NAKED0.ASC";

    if (BsZenLib::HasCachedMDS(file))
    {
      model = BsZenLib::LoadCachedMDS(file);
    }
    else
    {
      model = BsZenLib::ImportAndCacheMDS(file, REGoth::gVirtualFileSystem().getFileIndex());
    }

    if (!model || model->getMeshes().empty())
    {
    }

    for (const auto& h : model->getMeshes())
    {
      // gDebug().logDebug(h->getName());
    }

    // playerVisual->setModelScript(model);
    // playerVisual->setMesh(model->getMeshes()[2]);
    // playerVisual->setHeadMesh("HUM_HEAD_PONY.MMB");
    // playerVisual->playAnimation({});

    renderable->setMesh(model->getMeshes()[2]->getMesh());
    renderable->setMaterials(model->getMeshes()[2]->getMaterials());

    Sphere bounds = renderable->getBounds().getSphere();

    Vector3 cameraDirection = Vector3(-1, 0, 0);
    cameraDirection.normalize();

    auto cameraOffset = cameraDirection * bounds.getRadius() * 1.7f;
    mMainCamera->SO()->setPosition(cameraOffset);
    mMainCamera->SO()->lookAt(Vector3(0, 0, 0));
  }

protected:
  bs::HFPSCamera mFPSCamera;
};

int main(int argc, char** argv)
{
  std::unique_ptr<const REGoth::EngineConfig> config =
      REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
  REGothCharacterViewer engine{std::move(config)};

  return REGoth::runEngine(engine);
}
