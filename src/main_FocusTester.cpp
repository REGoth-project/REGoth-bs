#include <memory>

#include <BsFPSCamera.h>
#include <Components/BsCCamera.h>
#include <Components/BsCRenderable.h>
#include <Material/BsMaterial.h>
#include <Resources/BsBuiltinResources.h>
#include <Scene/BsSceneObject.h>

#include <core.hpp>
#include <components/Focusable.hpp>
#include <components/GameWorld.hpp>
#include <components/GameplayUI.hpp>
#include <components/Item.hpp>
#include <components/UIDialogueChoice.hpp>
#include <components/UIElement.hpp>
#include <components/UIFocusText.hpp>
#include <components/UISubtitleBox.hpp>

class REGothCharacterMovementTester : public REGoth::Engine
{
public:
  using REGoth::Engine::Engine;

  void setupMainCamera() override
  {
    REGoth::AbstractEngine::setupMainCamera();

    auto rs = mMainCamera->getRenderSettings();

    rs->enableIndirectLighting   = true;
    rs->ambientOcclusion.enabled = true;

    mMainCamera->setRenderSettings(rs);

    mMainCamera->SO()->addComponent<bs::FPSCamera>();
  }

  void setupScene() override
  {
    using namespace REGoth;

    HGameWorld world = GameWorld::createEmpty();

    bs::HShader shader = bs::gBuiltinResources().getBuiltinShader(bs::BuiltinShader::Standard);
    bs::HMaterial planeMaterial = bs::Material::create(shader);
    planeMaterial->setTexture("gAlbedoTex",
                              bs::gBuiltinResources().getTexture(bs::BuiltinTexture::Black));

    bs::HMesh planeMesh             = bs::gBuiltinResources().getMesh(bs::BuiltinMesh::Quad);
    bs::HSceneObject floorSO        = bs::SceneObject::create("Floor");
    bs::HRenderable floorRenderable = floorSO->addComponent<bs::CRenderable>();
    floorRenderable->setMesh(planeMesh);
    floorRenderable->setMaterial(planeMaterial);

    constexpr float GROUND_PLANE_SCALE = 20.0f;
    floorSO->setScale(bs::Vector3(GROUND_PLANE_SCALE, 1.0f, GROUND_PLANE_SCALE));

    auto item1 = world->insertItem("FOCUS_1", bs::Transform::IDENTITY);
    auto item2 = world->insertItem("ITFO_PLANTS_HERB_02", bs::Transform::IDENTITY);

    item1->SO()->setPosition(bs::Vector3(1, 0.2, 0));
    item2->SO()->setPosition(bs::Vector3(-1, 0, 0));

    GameplayUI::createGlobal(mMainCamera);

    gGameplayUI()->focusText()->putTextAbove(item1->SO()->getComponent<REGoth::Focusable>());

    gGameplayUI()->subtitleBox()->open();

    gGameplayUI()->choices()->addChoice({"X", "Hello World!", 0});
    gGameplayUI()->choices()->addChoice({"X", "This is a test!", 0});
    gGameplayUI()->choices()->addChoice({"X", "What is this?", 0});
    gGameplayUI()->choices()->addChoice({"X", "I'm hungry!", 0});
    gGameplayUI()->choices()->addChoice({"X", "I'm not hungry!", 0});

    mMainCamera->SO()->setPosition(bs::Vector3(0, 1, 3));
  }

protected:
};

int main(int argc, char** argv)
{
  std::unique_ptr<const REGoth::EngineConfig> config =
      REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
  REGothCharacterMovementTester engine{std::move(config)};

  return REGoth::runEngine(engine);
}
