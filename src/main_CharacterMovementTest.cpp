#include <memory>

#include <BsFPSCamera.h>
#include <Components/BsCCamera.h>
#include <Components/BsCPlaneCollider.h>
#include <Components/BsCRenderable.h>
#include <Material/BsMaterial.h>
#include <Resources/BsBuiltinResources.h>
#include <Scene/BsSceneObject.h>

#include <BsZenLib/ImportMaterial.hpp>
#include <BsZenLib/ImportTexture.hpp>
#include <daedalus/DATFile.h>

#include <components/Character.hpp>
#include <components/CharacterKeyboardInput.hpp>
#include <components/GameWorld.hpp>
#include <components/Item.hpp>
#include <components/ThirdPersonCamera.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <core/Engine.hpp>
#include <exception/Throw.hpp>
#include <original-content/VirtualFileSystem.hpp>

class REGothCharacterMovementTester : public REGoth::Engine
{
public:
  using REGoth::Engine::Engine;

  void setupMainCamera() override
  {
    REGoth::AbstractEngine::setupMainCamera();

    auto rs = mMainCamera->getRenderSettings();

    rs->enableIndirectLighting = false;
    rs->enableShadows          = true;

    mMainCamera->setRenderSettings(rs);

    mThirdPersonCamera = mMainCamera->SO()->addComponent<REGoth::ThirdPersonCamera>();
  }

  void setupScene() override
  {
    using namespace REGoth;

    HGameWorld world = GameWorld::createEmpty();

    bs::HTexture floorTexture = BsZenLib::ImportTexture("NW_NATURE_STONEGROUND_01.TGA",
                                                        REGoth::gVirtualFileSystem().getFileIndex());

    // bs::HShader shader = bs::gBuiltinResources().getBuiltinShader(bs::BuiltinShader::Standard);
    bs::HShader shader          = BsZenLib::GetShaderOfKind(BsZenLib::ShaderKind::Opaque);
    bs::HMaterial planeMaterial = bs::Material::create(shader);
    planeMaterial->setTexture("gAlbedoTex", floorTexture);

    bs::HMesh planeMesh             = bs::gBuiltinResources().getMesh(bs::BuiltinMesh::Quad);
    bs::HSceneObject floorSO        = bs::SceneObject::create("Floor");
    bs::HRenderable floorRenderable = floorSO->addComponent<bs::CRenderable>();
    floorRenderable->setMesh(planeMesh);
    floorRenderable->setMaterial(planeMaterial);

    constexpr float GROUND_PLANE_SCALE = 20.0f;
    floorSO->setScale(bs::Vector3(GROUND_PLANE_SCALE, 1.0f, GROUND_PLANE_SCALE));

    // Add a plane collider that will prevent physical objects going through the floor
    bs::HPlaneCollider planeCollider = floorSO->addComponent<bs::CPlaneCollider>();

    // REGoth::World::loadWorldEmpty();

    // Add some waypoint
    bs::String wpName = "ADW_ENTRANCE";
    {
      bs::HSceneObject wpSO = bs::SceneObject::create(wpName);

      wpSO->setParent(world->waynet()->SO());
      wpSO->setPosition(bs::Vector3(0, 0, 0));

      REGoth::HWaypoint wp = wpSO->addComponent<REGoth::Waypoint>();
      world->waynet()->addWaypoint(wp);
    }

    REGoth::HCharacter character = world->insertCharacter("PC_HERO", wpName);
    character->useAsHero();
    character->SO()->addComponent<REGoth::CharacterKeyboardInput>(world);
    mThirdPersonCamera->follow(character);
  }

  REGoth::HThirdPersonCamera mThirdPersonCamera;

protected:
};

int main(int argc, char** argv)
{
  std::unique_ptr<const REGoth::EngineConfig> config =
      REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
  REGothCharacterMovementTester engine{std::move(config)};

  return REGoth::runEngine(engine);
}
