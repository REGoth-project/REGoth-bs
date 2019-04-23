#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include <BsZenLib/ImportTexture.hpp>
#include <Components/BsCCamera.h>
#include <Components/BsCPlaneCollider.h>
#include <Components/BsCRenderable.h>
#include <Material/BsMaterial.h>
#include <Resources/BsBuiltinResources.h>
#include <Scene/BsSceneObject.h>
#include <components/Character.hpp>
#include <components/CharacterKeyboardInput.hpp>
#include <components/Item.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <daedalus/DATFile.h>
#include <exception/Throw.hpp>
#include <original-content/VirtualFileSystem.hpp>
#include <scripting/ScriptVMInterface.hpp>
#include <world/GameWorld.hpp>

class REGothCharacterMovementTester : public REGoth::REGothEngine
{
public:
  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();

    auto rs = mMainCamera->getRenderSettings();

    rs->enableIndirectLighting = false;
    rs->enableShadows = true;

    mMainCamera->setRenderSettings(rs);
  }

  void setupScene() override
  {
    using namespace REGoth;

    Scripting::loadGothicDAT(gVirtualFileSystem().readFile("GOTHIC.DAT"));

    // World::loadWorldFromZEN("ADDONWORLD.ZEN", World::GameWorld::Init::NoInitScripts);
    World::loadWorldEmpty();

    bs::HTexture floorTexture = BsZenLib::ImportTexture("NW_NATURE_STONEGROUND_01.TGA",
                                                        REGoth::gVirtualFileSystem().getFileIndex());

    bs::HShader shader = bs::gBuiltinResources().getBuiltinShader(bs::BuiltinShader::Standard);
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

      wpSO->setParent(REGoth::gWorld().waynet()->SO());
      wpSO->setPosition(bs::Vector3(0, 0, 0));

      REGoth::HWaypoint wp = wpSO->addComponent<REGoth::Waypoint>();
      REGoth::gWorld().waynet()->addWaypoint(wp);
    }

    REGoth::HCharacter character = REGoth::gWorld().insertCharacter("PC_HERO", wpName);
    character->SO()->addComponent<REGoth::CharacterKeyboardInput>();

    mMainCamera->SO()->setPosition(bs::Vector3(2, 0.5f, 0));
    mMainCamera->SO()->lookAt(bs::Vector3(bs::BsZero));
    mMainCamera->SO()->setParent(character->SO(), false);
  }

protected:
};

int main(int argc, char** argv)
{
  REGothCharacterMovementTester regoth;

  return REGoth::main(regoth, argc, argv);
}
