#include "BsCameraZoomer.h"
#include "BsFPSCamera.h"
#include "BsFPSWalker.h"
#include "BsObjectRotator.h"
#include "REGothEngine.hpp"
#include "Utility/BsTime.h"
#include "animation/StateNaming.hpp"
#include "components/VisualCharacter.hpp"
#include "original-content/VirtualFileSystem.hpp"
#include <BsZenLib/ImportAnimation.hpp>
#include <BsZenLib/ImportMorphMesh.hpp>
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ImportSkeletalMesh.hpp>
#include <BsZenLib/ImportZEN.hpp>
#include <Components/BsCCamera.h>
#include <Components/BsCCharacterController.h>
#include <Components/BsCRenderable.h>
#include <Debug/BsDebugDraw.h>
#include <Input/BsVirtualInput.h>
#include <Math/BsVector3.h>
#include <Scene/BsSceneObject.h>
#include <components/Character.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <daedalus/DATFile.h>
#include <scripting/ScriptVMInterface.hpp>
#include <world/GameWorld.hpp>

class SimpleCharacterController : public bs::Component
{
public:
  SimpleCharacterController(const bs::HSceneObject& parent, REGoth::HVisualCharacter visual)
      : Component(parent)
  {
    setName("SimpleCharacterController");

    mVisual = visual;

    playAnimationIfFound("S_FISTRUN");
  }

  /** Triggered once per frame. Allows the component to handle input and move. */
  void fixedUpdate() override
  {
    using namespace bs;

    mGoingForward      = gVirtualInput().isButtonHeld(VirtualButton("Forward"));
    mGoingBack         = gVirtualInput().isButtonHeld(VirtualButton("Back"));
    mGoingLeft         = gVirtualInput().isButtonHeld(VirtualButton("Left"));
    mGoingRight        = gVirtualInput().isButtonHeld(VirtualButton("Right"));
    mToggleMeleeWeapon = gVirtualInput().isButtonHeld(VirtualButton("ToggleMeleeWeapon"));

    DebugDraw::instance().clear();
    DebugDraw::instance().drawWireCube(SO()->getTransform().getPosition(),
                                       Vector3(0.5f, 0.5f, 0.5f));

    bs::String state = REGoth::Animation::getStateName(mVisual->getPlayingAnimationName());

    static String s_lastFrameAnim;

    if (s_lastFrameAnim != mVisual->getPlayingAnimationName())
    {
      gDebug().logDebug("[Anim] " + mVisual->getPlayingAnimationName());
      s_lastFrameAnim = mVisual->getPlayingAnimationName();
    }

    if (mVisual->getPlayingAnimationName().empty() || mToggleMeleeWeapon)
    {
      // Some animations have an empty `nextAnim`-field. Fall back to default
      // animation in that case
      playAnimationIfFound("S_FISTRUN");  // FIXME: Respect weapon mode
    }
    else if (state.empty() || !mVisual->isPlayingAnimationInterruptable())
    {
      // Do nothing and wait for the current animation to finish
      // After it is done, we should be in a valid state again.
    }
    else if (mGoingForward)
    {
      mVisual->tryPlayTransitionAnimationTo("S_FISTRUNL");
    }
    else if (mGoingBack)
    {
      mVisual->tryPlayTransitionAnimationTo("T_JUMPB");
    }
    // else if (mGoingLeft)
    // {
    //   mVisual->tryPlayTransitionAnimationTo("T_RUNSTRAFER");
    // }
    else
    {
      mVisual->tryPlayTransitionAnimationTo("S_FISTRUN");
    }
  }

  void playAnimationIfFound(const bs::String& name)
  {
    bs::HAnimationClip clip = mVisual->findAnimationClip(name);

    if (clip)
    {
      if (!mVisual->isAnimationPlaying(clip))
      {
        mVisual->playAnimation(clip);
      }
    }
    else
    {
      bs::gDebug().logWarning("[SimpleCharacterController] Failed to find animation: " + name);
    }
  }

private:
  REGoth::HVisualCharacter mVisual;

  // Keystates, valid during fixedUpdate()
  bool mGoingForward;
  bool mGoingBack;
  bool mGoingLeft;
  bool mGoingRight;
  bool mToggleMeleeWeapon;
};

class REGothCharacterViewer : public REGoth::REGothEngine
{
public:
  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();

    // mFPSCamera = mMainCamera->SO()->addComponent<bs::FPSCamera>();
    mMainCamera->SO()->addComponent<bs::CameraZoomer>();
  }

  void setupInput() override
  {
    using namespace bs;

    REGoth::REGothEngine::setupInput();

    auto inputConfig = gVirtualInput().getConfiguration();

    inputConfig->registerButton("RotateObj", BC_MOUSE_LEFT);
    inputConfig->registerButton("RotateCam", BC_MOUSE_RIGHT);
    inputConfig->registerAxis("Zoom", VIRTUAL_AXIS_DESC((UINT32)InputAxis::MouseZ));
  }

  void setupScene() override
  {
    using namespace bs;

    for (auto s : REGoth::gVirtualFileSystem().listAllFiles())
    {
      gDebug().logDebug(s);
    }

    const String file   = "HUMANS.MDS";
    const String visual = "HUM_BODY_NAKED0.ASC";

    Daedalus::DATFile dat("/home/andre/games/Gothic II/_work/Data/Scripts/_compiled/GOTHIC.DAT");
    REGoth::Scripting::loadGothicDAT(dat);

    REGoth::World::loadWorldEmpty();

    // Add some waypoint
    bs::String wpName = "SOMEPLACE";
    {
      bs::HSceneObject wpSO = bs::SceneObject::create(wpName);

      wpSO->setParent(REGoth::gWorld().waynet()->SO());
      wpSO->setPosition(bs::Vector3(0, 0, 0));

      REGoth::HWaypoint wp = wpSO->addComponent<REGoth::Waypoint>();
      REGoth::gWorld().waynet()->addWaypoint(wp);
    }

    REGoth::HCharacter character = REGoth::gWorld().insertCharacter("ORCWARRIOR_REST", wpName);

    REGoth::HVisualCharacter playerVisual = character->SO()->getComponent<REGoth::VisualCharacter>();
    // character->SO()->addComponent<SimpleCharacterController>(playerVisual);
    character->SO()->addComponent<bs::ObjectRotator>();

    // playerVisual->setModelScript(model);
    // playerVisual->setMesh(model->getMeshes()[0]);
    // playerVisual->setHeadMesh("HUM_HEAD_PONY.MMB");

    Sphere bounds = playerVisual->getBounds().getSphere();

    Vector3 cameraDirection = Vector3(1,0,0);
    cameraDirection.normalize();

    auto cameraOffset = cameraDirection * bounds.getRadius() * 1.7f;
    mMainCamera->SO()->setPosition(bounds.getCenter() + cameraOffset);
    mMainCamera->SO()->lookAt(Vector3(0, 0, 0));
  }

protected:
  bs::HFPSCamera mFPSCamera;
};

int main(int argc, char** argv)
{
  REGothCharacterViewer regoth;

  return REGoth::main(regoth, argc, argv);
}
