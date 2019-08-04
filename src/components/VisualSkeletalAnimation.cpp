#include "VisualSkeletalAnimation.hpp"
#include "original-content/VirtualFileSystem.hpp"
#include <Animation/BsAnimationClip.h>
#include <Components/BsCAnimation.h>
#include <Components/BsCRenderable.h>
#include <Debug/BsDebug.h>
#include <Mesh/BsMesh.h>
#include <RTTI/RTTI_VisualSkeletalAnimation.hpp>
#include <Scene/BsSceneObject.h>
#include <animation/Animation.hpp>
#include <animation/StateNaming.hpp>
#include <components/NodeVisuals.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameResources.hpp>

namespace REGoth
{
  VisualSkeletalAnimation::VisualSkeletalAnimation(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("VisualSkeletalAnimation");
  }

  void VisualSkeletalAnimation::onInitialized()
  {
    bs::Component::onInitialized();

    // If this is called after deserialization, we need register the event-callback in here
    if (mSubRenderable)
    {
      setupAnimationComponent();
    }
  }

  void VisualSkeletalAnimation::throwIfNotReadyForRendering() const
  {
    if (!mModelScript)
    {
      REGOTH_THROW(InvalidStateException, "Expected Model Script but none was set!");
    }

    if (!mMesh)
    {
      REGOTH_THROW(InvalidStateException, "Expected Mesh but none was set!");
    }

    if (!mSubRenderable || !mSubAnimation || !mSubNodeVisuals)
    {
      REGOTH_THROW(InvalidStateException, "Missing sub-component!");
    }
  }

  void VisualSkeletalAnimation::setModelScript(BsZenLib::Res::HModelScriptFile modelScript)
  {
    if (modelScript->getName().empty())
    {
      REGOTH_THROW(InvalidParametersException, "ModelScript should have a name!");
    }

    deleteObjectSubtree();
    mModelScript = modelScript;

    createAnimationMap();
  }

  void VisualSkeletalAnimation::createAnimationMap()
  {
    if (!mModelScript)
    {
      REGOTH_THROW(InvalidStateException, "No model script set!");
    }

    for (auto anim : mModelScript->getAnimations())
    {
      mAnimationClips[anim->getName()] = anim;
    }
  }

  void VisualSkeletalAnimation::setMesh(BsZenLib::Res::HMeshWithMaterials mesh)
  {
    using namespace bs;

    if (isMeshRegisteredInModelScript(mesh))
    {
      mMesh = mesh;

      deleteObjectSubtree();
      buildObjectSubtree();

      addDefaultAttachments();

      // TODO: This is only in while we don't have a proper animation controller/AI. Remove once
      //       that is in!
      playDefaultIdleAnimation();
    }
    else
    {
      REGOTH_THROW(InvalidParametersException,
                   "Mesh " + mesh->getName() + " is not part of set model-script " +
                       (mModelScript ? mModelScript->getName() : "<none>"));
    }
  }

  void VisualSkeletalAnimation::useFirstMeshOfModelScript()
  {
    if (!mModelScript)
    {
      REGOTH_THROW(InvalidStateException, "Expected Model Script but none was set!");
    }

    if (mModelScript->getMeshes().empty())
    {
      REGOTH_THROW(InvalidStateException, "Model Script seems to not have any meshes!");
    }

    setMesh(mModelScript->getMeshes()[0]);
  }

  void VisualSkeletalAnimation::setVisual(const bs::String& visual)
  {
    auto modelScript = gOriginalGameResources().modelScript(visual);

    if (!modelScript)
    {
      REGOTH_THROW(InvalidParametersException, "Model Script " + visual + " could not be loaded!");
    }

    setModelScript(modelScript);

    // Using the first registered mesh as the default seems to be like the original is doing it
    useFirstMeshOfModelScript();
  }

  bool VisualSkeletalAnimation::hasVisual() const
  {
    if (!mModelScript)
      return false;

    if (!mMesh)
      return false;

    return true;
  }

  bs::Bounds VisualSkeletalAnimation::getBounds() const
  {
    return mSubRenderable ? mSubRenderable->getBounds() : bs::Bounds();
  }

  bool VisualSkeletalAnimation::isMeshRegisteredInModelScript(BsZenLib::Res::HMeshWithMaterials mesh)
  {
    if (!mModelScript) return false;

    for (const auto& m : mModelScript->getMeshes())
    {
      if (m == mesh) return true;
    }

    return false;
  }

  void VisualSkeletalAnimation::deleteObjectSubtree()
  {
    for (bs::HSceneObject h : mSubObjects)
    {
      h->destroy();
    }

    mSubObjects.clear();
  }

  void VisualSkeletalAnimation::buildObjectSubtree()
  {
    createAndRegisterSubComponents();

    setupRenderableComponent();
    setupAnimationComponent();
  }

  void VisualSkeletalAnimation::createAndRegisterSubComponents()
  {
    bs::HSceneObject renderSO = createAndRegisterSubObject("Renderable");

    // For some weird reason, the rotations don't match with the root motion.
    // Work around that here...
    bs::Quaternion rotate90Y = bs::Quaternion(bs::Radian(0),                // .
                                              bs::Radian(bs::Degree(-90)),  // .
                                              bs::Radian(0));
    renderSO->getParent()->rotate(rotate90Y);

    mSubRenderable  = renderSO->addComponent<bs::CRenderable>();
    mSubAnimation   = renderSO->addComponent<bs::CAnimation>();
    mSubNodeVisuals = renderSO->addComponent<NodeVisuals>();
  }

  bs::HSceneObject VisualSkeletalAnimation::createAndRegisterSubObject(const bs::String& name)
  {
    // Need to create that weird sub-sub-object setup here since it seems that you can't
    // rotate objects which have an animation component, but only their parents.
    // We need to fix the weird rotation-bug however, so we put one more sub-object we
    // can rotate in between.
    bs::HSceneObject subSO    = bs::SceneObject::create(name);
    bs::HSceneObject subsubSO = bs::SceneObject::create(name);

    bool dontKeepWorldTransform = false;

    subSO->setParent(SO(), dontKeepWorldTransform);
    subsubSO->setParent(subSO, dontKeepWorldTransform);

    mSubObjects.push_back(subSO);
    mSubObjects.push_back(subsubSO);

    return subsubSO;
  }

  void VisualSkeletalAnimation::setupRenderableComponent()
  {
    using namespace bs;

    mSubRenderable->setMesh(mMesh->getMesh());
    mSubRenderable->setMaterials(mMesh->getMaterials());
  }

  void VisualSkeletalAnimation::setupAnimationComponent()
  {
    using namespace bs;

    // Subscribe to animation events
    mSubAnimation->onEventTriggered.connect([this](auto clip, auto string) {
      // Call objects actual method
      this->onAnimationEvent(clip, string);
    });
  }

  void VisualSkeletalAnimation::onAnimationEvent(const bs::HAnimationClip& clip, bs::String string)
  {
    using namespace bs;

    throwIfNotReadyForRendering();

    // Commands could come in the form of "COMMAND:ACTION" or just "COMMAND"
    size_t separator = string.find_first_of(':');
    bs::String command;
    bs::String action;

    if (separator != bs::String::npos)
    {
      command = string.substr(0, separator);
      action  = string.substr(command.length() + 1);
    }
    else
    {
      command = string;
    }

    bs::AnimationClipState state;
    mSubAnimation->getState(clip, state);

    if (command == "STOP")
    {
      playAnimationClip({});
    }
    else if (command == "PLAYCLIP")
    {
      // gDebug().logDebug("[VisualSkeletalAnimation] " + SO()->getName() + " - " + clip->getName() +
      //                   ": event PLAYCLIP: " + action);

      auto clip = findAnimationClip(action);

      if (clip)
      {
        playAnimationClip(clip);
      }
      else
      {
        REGOTH_LOG(Warning, Uncategorized, "[VisualSkeletalAnimation] Unknown next animation: {0}",
                   action);
      }
    }
    else if (command == "MORPHMESHANI")
    {
      REGOTH_LOG(Warning, Uncategorized,
                 "[VisualSkeletalAnimation] Unimplemented morph-mesh ani: {0}", action);
    }
    else
    {
      REGOTH_LOG(Warning, Uncategorized, "[VisualSkeletalAnimation] Unknown animation event: {0}",
                 string);
    }
  }

  void VisualSkeletalAnimation::playAnimationClip(HZAnimationClip clip)
  {
    using namespace bs;
    using ZAnimationClip = BsZenLib::Res::ZAnimationClip;

    throwIfNotReadyForRendering();

    if (clip)
    {
      if (isClipLooping(clip))
      {
        mSubAnimation->setWrapMode(bs::AnimWrapMode::Loop);
      }
      else
      {
        mSubAnimation->setWrapMode(bs::AnimWrapMode::Clamp);
      }

      bs::INT32 layer = getClipLayer(clip);

      if (layer > 0)
      {
        // REGOTH_LOG(Info, Uncategorized, bs::StringUtil::format(
        //     "[VisualSkeletalAnimation] Layered animation {1} not implemented", clip->getName()));

        // Commented out: Doesn't work yet
        // mSubAnimation->blendAdditive(clip, 1.0f, 0.0f, (bs::UINT32)layer);
        mSubAnimation->play(clip->mClip);

        // TODO: Once blending is implemented, these won't be the main animation clips anymore
        mPlayingMainAnimation = clip;
      }
      else
      {
        mSubAnimation->play(clip->mClip);
        mPlayingMainAnimation = clip;
      }
    }
    else
    {
      mSubAnimation->stopAll();
      mPlayingMainAnimation = {};
    }
  }

  void REGoth::VisualSkeletalAnimation::playDefaultIdleAnimation()
  {
    auto possibleAnims = {"S_RUN", "S_FISTRUN", "S_S0"};

    for (auto anim : possibleAnims)
    {
      auto clip = findAnimationClip(anim);

      if (clip)
      {
        playAnimationClip(clip);
        return;
      }
    }

    // No animation found, stop all animations instead
    playAnimationClip({});
  }

  bs::Vector<bs::String> VisualSkeletalAnimation::listPossibleDefaultAnimations() const
  {
    return {};
  }

  bs::String VisualSkeletalAnimation::findAnimationToTransitionTo(const bs::String& stateAnim) const
  {
    bs::String stateNow = getStateFromPlayingAnimation();

    // getStateFromPlayingAnimation returns a string without the leading S_
    if (!stateNow.empty())
    {
      stateNow = "S_" + stateNow;
    }

    return findAnimationToTransitionTo(stateNow, stateAnim);
  }

  bs::String VisualSkeletalAnimation::findAnimationToTransitionTo(const bs::String& fromAnim,
                                                                  const bs::String& toAnim) const
  {
    // No animation being played should not happen during normal operation, but if it does,
    // don't hang up the visual here. I've only see this happen after deserialization but that
    // might have been an other issue.
    if (!mSubAnimation->isPlaying())
    {
      return toAnim;
    }

    // Some animations are directly reachable, like S_RUN -> T_JUMPB. Whether the transition makes
    // sense has to be checked elsewhere.
    if (!AnimationState::isTransitionNeeded(toAnim))
    {
      return toAnim;
    }

    bs::String from = AnimationState::getStateName(fromAnim);
    bs::String to   = AnimationState::getStateName(toAnim);

    bs::String transition =
        AnimationState::constructTransitionAnimationName(AI::WeaponMode::None, from, to);

    // Transition not possible since it wasn't meant to be possible
    if (transition.empty())
    {
      return "";
    }

    return transition;
  }

  HZAnimationClip VisualSkeletalAnimation::findAnimationClip(const bs::String& name) const
  {
    using namespace bs;

    bs::String actualName = mModelScript->getName() + "-" + name;

    auto result = mAnimationClips.find(actualName);

    if (result == mAnimationClips.end()) return {};

    return result->second;
  }

  bool VisualSkeletalAnimation::isAnimationPlaying(HZAnimationClip clip) const
  {
    throwIfNotReadyForRendering();

    for (bs::UINT32 i = 0; i < mSubAnimation->getNumClips(); i++)
    {
      if (mSubAnimation->getClip(i) == clip) return true;
    }

    return false;
  }

  bs::String VisualSkeletalAnimation::getPlayingAnimationName() const
  {
    throwIfNotReadyForRendering();

    if (!mSubAnimation->isPlaying()) return "";
    if (!mSubAnimation->getClip(0)) return "";

    bs::String full = mSubAnimation->getClip(0)->getName();

    // Strip the "HUMANS-" part
    return full.substr(full.find_first_of('-') + 1);
  }

  bool VisualSkeletalAnimation::isPlayingAnimationInterruptable() const
  {
    bs::String name = getPlayingAnimationName();

    return name.find("-T_") == bs::String::npos;
  }

  bs::String VisualSkeletalAnimation::getStateFromPlayingAnimation() const
  {
    return AnimationState::getStateName(getPlayingAnimationName());
  }

  bs::Vector3 VisualSkeletalAnimation::resolveFrameRootMotion()
  {
    if (!mSubAnimation) return bs::Vector3(bs::BsZero);

    bs::HAnimationClip clipNow = mSubAnimation->getClip(0);

    bs::Vector3 motion = bs::Vector3(bs::BsZero);

    if (mRootMotionLastClip != clipNow)
    {
      // Make sure to get the last bits of the last clip too
      // Commented out: FIXME Doesn't seem to work, but it has to be something like this
      // if (mRootMotionLastClip)
      // {
      //   float then = fmod(mRootMotionLastTime, mRootMotionLastClip->getLength());
      //   float now  = mRootMotionLastClip->getLength();

      //   motion += AnimationState::getRootMotionSince(mRootMotionLastClip, then, now);
      // }

      mRootMotionLastTime = 0.0f;
      mRootMotionLastClip = clipNow;
    }

    if (!clipNow)
    {
      return motion;
    }

    bs::AnimationClipState state;
    mSubAnimation->getState(clipNow, state);

    float then = mRootMotionLastTime;
    float now  = state.time;

    // fixedUpdate might be called more often than the animation timing is updated
    // Comparing floats here is intentional, since we don't touch them in the meantime.
    if (then != now)
    {
      // Root motion has to be calculated using non-looping times since it needs to find the
      // first and last frames of animations.
      if (mSubAnimation->getWrapMode() == bs::AnimWrapMode::Loop)
      {
        then = fmod(then, clipNow->getLength());
        now  = fmod(now, clipNow->getLength());
      }

      if (now < then)
      {
        // Animation wrapped
        // FIXME: Commented out because it doesn't work yet. These lines cause NPCs with
        //        static animations to slide around. Some non-looped animations produce
        //        weird jumps (ie. T_JUMPB).
        // motion += AnimationState::getRootMotionSince(clipNow, 0.0f, now);
        // motion += AnimationState::getRootMotionSince(clipNow, then, clipNow->getLength());
      }
      else
      {
        motion += AnimationState::getRootMotionSince(clipNow, then, now);
      }

      // REGOTH_LOG(Info, Uncategorized, bs::StringUtil::format("RootMotion {0} -> {1}: {2}",
      // then, now, bs::toString(motion)));
    }

    mRootMotionLastTime = state.time;
    mRootMotionLastClip = clipNow;

    return motion;
  }

  bool VisualSkeletalAnimation::isPlayingIdleAnimation() const
  {
    if (!mPlayingMainAnimation) return false;

    return mPlayingMainAnimation->mIsIdleAnimation;
  }

  void VisualSkeletalAnimation::addDefaultAttachments()
  {
    throwIfNotReadyForRendering();

    for (const auto& attachment : mMesh->getNodeAttachments())
    {
      mSubNodeVisuals->attachMeshToNode(attachment.first, attachment.second);
    }
  }

  bool VisualSkeletalAnimation::isClipLooping(HZAnimationClip clip) const
  {
    return clip->mIsLooping;
  }

  bs::UINT32 VisualSkeletalAnimation::getClipLayer(HZAnimationClip clip) const
  {
    return clip->mLayer;
  }

  void VisualSkeletalAnimation::setDebugAnimationSpeedFactor(float factor)
  {
    if (mSubAnimation)
    {
      mSubAnimation->setSpeed(factor);
    }
  }

  REGOTH_DEFINE_RTTI(VisualSkeletalAnimation)

}  // namespace REGoth
