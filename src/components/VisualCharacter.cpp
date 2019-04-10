#include "VisualCharacter.hpp"
#include <Animation/BsAnimationClip.h>
#include <Components/BsCAnimation.h>
#include <Components/BsCRenderable.h>
#include <Debug/BsDebug.h>
#include <Scene/BsSceneObject.h>
#include <animation/Animation.hpp>
#include <animation/StateNaming.hpp>
#include <components/NodeVisuals.hpp>

const bs::String MODEL_NODE_NAME_R_HAND = "BIP01 R HAND";
const bs::String MODEL_NODE_NAME_L_HAND = "BIP01 L HAND";
const bs::String MODEL_NODE_NAME_R_FOOT = "BIP01 R FOOT";
const bs::String MODEL_NODE_NAME_L_FOOT = "BIP01 L FOOT";
const bs::String MODEL_NODE_NAME_HEAD = "BIP01 HEAD";
const bs::String MODEL_NODE_NAME_FOOTSTEPS = " FOOTSTEPS";

namespace REGoth
{
  VisualCharacter::VisualCharacter(const bs::HSceneObject& parent)
      : Component(parent)
  {
    setName("VisualCharacter");
  }

  void VisualCharacter::setModelScript(BsZenLib::Res::HModelScriptFile modelScript)
  {
    mModelScript = modelScript;

    for (bs::HAnimationClip clip : mModelScript->getAnimationClips())
    {
      mAnimationClips[clip->getName()] = clip;
      bs::gDebug().logDebug(clip->getName());
    }
  }

  void VisualCharacter::setMesh(BsZenLib::Res::HMeshWithMaterials mesh)
  {
    using namespace bs;

    if (isMeshRegisteredInModelScript(mesh))
    {
      mMesh = mesh;

      deleteObjectSubtree();
      buildObjectSubtree();
    }
    else
    {
      gDebug().logError("[VisualCharacter] Mesh " + mesh->getName() +
                        " is not part of set model-script " +
                        (mModelScript ? mModelScript->getName() : "<none>"));
    }
  }

  bs::Bounds VisualCharacter::getBounds() const
  {
    return mSubRenderable ? mSubRenderable->getBounds() : bs::Bounds();
  }

  bool VisualCharacter::isMeshRegisteredInModelScript(BsZenLib::Res::HMeshWithMaterials mesh)
  {
    if (!mModelScript) return false;

    for (const auto& m : mModelScript->getMeshes())
    {
      if (m == mesh) return true;
    }

    return false;
  }

  void VisualCharacter::deleteObjectSubtree()
  {
    mSubRenderable = nullptr;
    mSubAnimation = nullptr;

    for (bs::HSceneObject h : mSubObjects)
    {
      h->destroy();
    }

    mSubObjects.clear();
  }

  void VisualCharacter::buildObjectSubtree()
  {
    createAndRegisterSubComponents();

    setupRenderableComponent();
    setupAnimationComponent();
  }

  void VisualCharacter::createAndRegisterSubComponents()
  {
    bs::HSceneObject renderSO = createAndRegisterSubObject("Renderable");

    mSubRenderable = renderSO->addComponent<bs::CRenderable>();
    mSubAnimation = renderSO->addComponent<bs::CAnimation>();
    mSubNodeVisuals = renderSO->addComponent<NodeVisuals>();
  }

  bs::HSceneObject VisualCharacter::createAndRegisterSubObject(const bs::String& name)
  {
    bs::HSceneObject subSO = bs::SceneObject::create(name);
    subSO->setParent(SO());

    mSubObjects.push_back(subSO);

    return subSO;
  }

  void VisualCharacter::setupRenderableComponent()
  {
    using namespace bs;

    mSubRenderable->setMesh(mMesh->getMesh());
    mSubRenderable->setMaterials(mMesh->getMaterials());
  }

  void VisualCharacter::setupAnimationComponent()
  {
    using namespace bs;

    // We do manual looping
    mSubAnimation->setWrapMode(AnimWrapMode::Loop);

    // Subscribe to animation events
    mSubAnimation->onEventTriggered.connect([this](auto clip, auto string) {
      // Call objects actual method
      this->onAnimationEvent(clip, string);
    });
  }

  void VisualCharacter::onAnimationEvent(const bs::HAnimationClip& clip, bs::String string)
  {
    using namespace bs;

    bs::String command = string.substr(0, string.find_first_of(':'));
    bs::String action = string.substr(command.length() + 1);

    gDebug().logDebug("[VisualCharacter] Got animation event: " + command + ":" + action);

    if (command == "PLAYCLIP")
    {
      HAnimationClip clip = findAnimationClip(action);

      if (clip)
      {
        playAnimation(clip);
      }
      else
      {
        gDebug().logWarning("[VisualCharacter] Unknown next animation: " + action);
      }
    }
    else
    {
      gDebug().logWarning("[VisualCharacter] Unknown animation event: " + string);
    }
  }

  void VisualCharacter::playAnimation(bs::HAnimationClip clip)
  {
    using namespace bs;

    if (!mSubAnimation)
    {
      BS_EXCEPT(InvalidStateException, "Animation component not initialized!");
    }

    if (clip)
    {
      mSubAnimation->play(clip);
    }
    else
    {
      mSubAnimation->stopAll();
    }
  }

  bool VisualCharacter::tryPlayTransitionAnimationTo(const bs::String& state)
  {
    bs::HAnimationClip clip;

    if (Animation::isTransitionNeeded(state))
    {
      bs::String from = getStateFromPlayingAnimation();
      bs::String to = Animation::getStateName(state);

      if (from.empty()) return false;

      bs::String transition =
          Animation::constructTransitionAnimationName(REGoth::Animation::WeaponMode::None, from, to);

      if (transition.empty()) return false;

      clip = findAnimationClip(transition);
    }
    else
    {
      clip = findAnimationClip(state);
    }

    if (!clip) return false;

    if (!isAnimationPlaying(clip))
    {
      playAnimation(clip);
    }

    return true;
  }

  bs::HAnimationClip VisualCharacter::findAnimationClip(const bs::String& name) const
  {
    using namespace bs;

    bs::String actualName = "HUMANS-" + name;

    auto result = mAnimationClips.find(actualName);

    if (result == mAnimationClips.end()) return {};

    return result->second;
  }

  bool VisualCharacter::isAnimationPlaying(bs::HAnimationClip clip) const
  {
    for (bs::UINT32 i = 0; i < mSubAnimation->getNumClips(); i++)
    {
      if (mSubAnimation->getClip(i) == clip) return true;
    }

    return false;
  }

  bs::String VisualCharacter::getPlayingAnimationName() const
  {
    if (!mSubAnimation->isPlaying()) return "";
    if (!mSubAnimation->getClip(0)) return "";

    bs::String full = mSubAnimation->getClip(0)->getName();

    // Strip the "HUMANS-" part
    return full.substr(full.find_first_of('-') + 1);
  }

  bool VisualCharacter::isPlayingAnimationInterruptable() const
  {
    bs::String name = getPlayingAnimationName();

    return name.find("-T_") == bs::String::npos;
  }

  bs::String VisualCharacter::getStateFromPlayingAnimation() const
  {
    return Animation::getStateName(getPlayingAnimationName());
  }

  bs::Vector3 VisualCharacter::resolveFrameRootMotion()
  {
    if (!mSubAnimation) return bs::Vector3(bs::BsZero);

    bs::HAnimationClip clipNow = mSubAnimation->getClip(0);

    if (mRootMotionLastClip != clipNow)
    {
      mRootMotionLastClip = clipNow;
      mRootMotionLastTime = 0.0f;
      return bs::Vector3(bs::BsZero);
    }

    if (!clipNow)
    {
      return bs::Vector3(bs::BsZero);
    }

    bs::AnimationClipState state;
    mSubAnimation->getState(clipNow, state);

    float then = mRootMotionLastTime;
    float now = state.time;

    bs::Vector3 motion = Animation::getRootMotionSince(mRootMotionLastClip, then, now);

    mRootMotionLastTime = now;
    mRootMotionLastClip = clipNow;

    return motion;
  }

  void VisualCharacter::setHeadMesh(const bs::String& headmesh, bs::UINT32 headTextureIdx,
                                    bs::UINT32 teethTextureIdx)
  {
    mBodyState.headVisual = headmesh;
    mBodyState.headTextureIdx = headTextureIdx;
    mBodyState.teethTextureIdx = teethTextureIdx;

    updateHeadMesh();
  }

  void VisualCharacter::updateBodyMesh()
  {
  }

  void VisualCharacter::updateHeadMesh()
  {
    using namespace bs;

    if (!mSubAnimation)
    {
      BS_EXCEPT(InvalidStateException, "Animation component not initialized!");
    }

    mSubNodeVisuals->attachVisualToNode(MODEL_NODE_NAME_HEAD, mBodyState.headVisual);

    // TODO: Fix texture and color
  }

}  // namespace REGoth
