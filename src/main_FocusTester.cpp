#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include <Components/BsCCamera.h>
#include <Components/BsCRenderable.h>
#include <Material/BsMaterial.h>
#include <Resources/BsBuiltinResources.h>
#include <Scene/BsSceneObject.h>
#include <components/Focusable.hpp>
#include <components/GameUI.hpp>
#include <components/GameWorld.hpp>
#include <components/Item.hpp>
#include <components/UIDialogueChoice.hpp>
#include <components/UIElement.hpp>
#include <components/UIFocusText.hpp>
#include <components/UISubtitleBox.hpp>

class REGothCharacterMovementTester : public REGoth::REGothEngine
{
public:
  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();

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

    GameUI::createGlobal(mMainCamera);

    gGameUI()->focusText()->putTextAbove(item1->SO()->getComponent<REGoth::Focusable>(),
                                         "Hello Focus!");

    gGameUI()->subtitleBox()->open();

    gGameUI()->choices()->addChoice({"Hello World!", "ScriptFn1"});
    gGameUI()->choices()->addChoice({"This is a test!", "ScriptFn2"});
    gGameUI()->choices()->addChoice({"What is this?", "ScriptFn3"});
    gGameUI()->choices()->addChoice({"I'm hungry!", "ScriptFn4"});
    gGameUI()->choices()->addChoice({"I'm not hungry!", "ScriptFn5"});

    mMainCamera->SO()->setPosition(bs::Vector3(0, 1, 3));
  }

protected:
};

int main(int argc, char** argv)
{
  REGothCharacterMovementTester regoth;

  return REGoth::main(regoth, argc, argv);
}
