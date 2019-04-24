#include "VisualSkeletalAnimation.hpp"
#include <RTTI/RTTI_VisualSkeletalAnimation.hpp>

#include "VisualSkeletalAnimation.hpp"
#include "original-content/VirtualFileSystem.hpp"
#include <Animation/BsAnimationClip.h>
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ImportSkeletalMesh.hpp>
#include <Components/BsCAnimation.h>
#include <Components/BsCRenderable.h>
#include <Debug/BsDebug.h>
#include <Mesh/BsMesh.h>
#include <RTTI/RTTI_VisualCharacter.hpp>
#include <Scene/BsSceneObject.h>
#include <animation/Animation.hpp>
#include <animation/StateNaming.hpp>
#include <components/NodeVisuals.hpp>
#include <excepction/Throw.hpp>

namespace REGoth
{
  VisualSkeletalAnimation::VisualSkeletalAnimation(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("VisualSkeletalAnimation");
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

    for (bs::HAnimationClip clip : mModelScript->getAnimationClips())
    {
      mAnimationClips[clip->getName()] = clip;
      // bs::gDebug().logDebug(clip->getName());
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
    BsZenLib::Res::HModelScriptFile modelScript;

    // FIXME: Loading these from cache does work in the character viewer, but not in the world
    // viewer?! This set is just a workaround so we always use model scripts from this run
    // while caching only one of it. It should be removed once that is figured out.
    static bs::Set<bs::String> s_cachedVisuals;
    bool hasCachedItThisRun = s_cachedVisuals.find(visual) != s_cachedVisuals.end();

    if (hasCachedItThisRun && BsZenLib::HasCachedMDS(visual))
    {
      modelScript = BsZenLib::LoadCachedMDS(visual);
    }
    else
    {
      modelScript = BsZenLib::ImportAndCacheMDS(visual, REGoth::gVirtualFileSystem().getFileIndex());
      s_cachedVisuals.insert(visual);
    }

    if (!modelScript)
    {
      REGOTH_THROW(InvalidParametersException, "Model Script " + visual + " could not be loaded!");
    }

    setModelScript(modelScript);

    // Using the first registered mesh as the default seems to be like the original is doing it
    useFirstMeshOfModelScript();
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

    mSubRenderable  = renderSO->addComponent<bs::CRenderable>();
    mSubAnimation   = renderSO->addComponent<bs::CAnimation>();
    mSubNodeVisuals = renderSO->addComponent<NodeVisuals>();
  }

  bs::HSceneObject VisualSkeletalAnimation::createAndRegisterSubObject(const bs::String& name)
  {
    bs::HSceneObject subSO = bs::SceneObject::create(name);

    bool dontKeepWorldTransform = false;

    subSO->setParent(SO(), dontKeepWorldTransform);

    mSubObjects.push_back(subSO);

    return subSO;
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

    // We do manual looping
    // mSubAnimation->setWrapMode(AnimWrapMode::Clamp);
    // FIXME: Animation Events are broken for clamped animations (in bsf)
    mSubAnimation->setWrapMode(AnimWrapMode::Loop);

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

    bs::String command = string.substr(0, string.find_first_of(':'));
    bs::String action  = string.substr(command.length() + 1);

    bs::AnimationClipState state;
    mSubAnimation->getState(clip, state);

    // gDebug().logDebug(bs::StringUtil::format(
    //     "[VisualSkeletalAnimation] Got animation event: {0}:{1} while playing {2} at {3}",
    //     command, action, clip->getName(), state.time));

    // gDebug().logDebug("Animation has the following events: ");
    // for (auto& event : clip->getEvents())
    // {
    //   gDebug().logDebug(bs::StringUtil::format(" - {0}: {1}", event.time, event.name));
    // }

    if (command == "PLAYCLIP")
    {
      HAnimationClip clip = findAnimationClip(action);

      if (clip)
      {
        playAnimation(clip);
      }
      else
      {
        gDebug().logWarning("[VisualSkeletalAnimation] Unknown next animation: " + action);
      }
    }
    else
    {
      gDebug().logWarning("[VisualSkeletalAnimation] Unknown animation event: " + string);
    }
  }

  void VisualSkeletalAnimation::playAnimation(bs::HAnimationClip clip)
  {
    using namespace bs;

    throwIfNotReadyForRendering();

    if (clip)
    {
      // bs::gDebug().logDebug(clip->getName());
      mSubAnimation->play(clip);
    }
    else
    {
      mSubAnimation->stopAll();
    }
  }

  void REGoth::VisualSkeletalAnimation::playDefaultIdleAnimation()
  {
    auto possibleAnims = {"S_RUN", "S_FISTRUN", "S_S0"};

    for (auto anim : possibleAnims)
    {
      bs::HAnimationClip clip = findAnimationClip(anim);

      if (clip)
      {
        playAnimation(clip);
        return;
      }
    }

    // No animation found, stop all animations instead
    playAnimation({});
  }

  bs::Vector<bs::String> VisualSkeletalAnimation::listPossibleDefaultAnimations() const
  {
    return {};
  }

  bool VisualSkeletalAnimation::tryPlayTransitionAnimationTo(const bs::String& state)
  {
    bs::HAnimationClip clip;

    if (Animation::isTransitionNeeded(state))
    {
      bs::String from = getStateFromPlayingAnimation();
      bs::String to   = Animation::getStateName(state);

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

  bs::HAnimationClip VisualSkeletalAnimation::findAnimationClip(const bs::String& name) const
  {
    using namespace bs;

    bs::String actualName = mModelScript->getName() + "-" + name;

    auto result = mAnimationClips.find(actualName);

    if (result == mAnimationClips.end()) return {};

    return result->second;
  }

  bool VisualSkeletalAnimation::isAnimationPlaying(bs::HAnimationClip clip) const
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
    return Animation::getStateName(getPlayingAnimationName());
  }

  bs::Vector3 VisualSkeletalAnimation::resolveFrameRootMotion()
  {
    if (!mSubAnimation) return bs::Vector3(bs::BsZero);

    bs::HAnimationClip clipNow = mSubAnimation->getClip(0);

    bs::Vector3 motion = bs::Vector3(bs::BsZero);

    if (mRootMotionLastClip != clipNow)
    {
      // Make sure to get the last bits of the last clip too
      if (mRootMotionLastClip)
      {
        bs::AnimationClipState state;
        mSubAnimation->getState(mRootMotionLastClip, state);

        float then = mRootMotionLastTime;
        float now  = mRootMotionLastClip->getLength();

        motion += Animation::getRootMotionSince(mRootMotionLastClip, then, now);
      }

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
      motion += Animation::getRootMotionSince(clipNow, then, now);

      // bs::gDebug().logDebug(bs::StringUtil::format("RootMotion {0} -> {1}: {2}", then, now,
      // bs::toString(motion)));
    }

    mRootMotionLastTime = now;
    mRootMotionLastClip = clipNow;

    return motion;
  }

  void VisualSkeletalAnimation::addDefaultAttachments()
  {
    throwIfNotReadyForRendering();

    for (const auto& attachment : mMesh->getNodeAttachments())
    {
      mSubNodeVisuals->attachMeshToNode(attachment.first, attachment.second);
    }
  }

  bs::RTTITypeBase* VisualSkeletalAnimation::getRTTIStatic()
  {
    return RTTI_VisualSkeletalAnimation::instance();
  }

  bs::RTTITypeBase* VisualSkeletalAnimation::getRTTI() const
  {
    return VisualSkeletalAnimation::getRTTIStatic();
  }

}  // namespace REGoth
