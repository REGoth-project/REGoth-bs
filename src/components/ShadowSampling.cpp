#include "ShadowSampling.hpp"
#include <RTTI/RTTI_ShadowSampling.hpp>
#include <components/CharacterAI.hpp>
#include <components/GameWorld.hpp>
#include <log/logging.hpp>
#include <visual/ShadowSampler.hpp>

#include <Components/BsCRenderable.h>
#include <Material/BsMaterial.h>
#include <Scene/BsSceneObject.h>

// TODO: Relocate
template <typename Predicate>
bs::HSceneObject findParentIf(const bs::HSceneObject& so, Predicate pred)
{
  for (bs::HSceneObject parent = so->getParent(); parent != nullptr; parent = parent->getParent())
  {
    if (pred(parent))
    {
      return parent;
    }
  }

  return bs::HSceneObject();
}

namespace REGoth
{
  ShadowSampling::ShadowSampling(const bs::HSceneObject& parent)
      : bs::Component(parent)
      , mNeedsUpdate(true)
  {
    setNotifyFlags(bs::TransformChangedFlags::TCF_Transform);
    setFlag(bs::ComponentFlag::AlwaysRun, true); // FIXME: Required due to the pause-run hack in ConstructFromZen::importWaynet
  }

  ShadowSampling::ShadowSampling()
      : mNeedsUpdate(true)
  {
    setNotifyFlags(bs::TransformChangedFlags::TCF_Transform); 
    setFlag(bs::ComponentFlag::AlwaysRun, true); // FIXME: Required due to the pause-run hack in ConstructFromZen::importWaynet
  }

  void ShadowSampling::onInitialized()
  {
    bs::Component::onInitialized();

    if (mSampler == nullptr)
    {
      updateSamplerFromAncestry();
    }
  }

  void ShadowSampling::update()
  {
    if (!mNeedsUpdate)
    {
      return;
    }

    if (!mSampler)
    {
      REGOTH_LOG(Warning, Uncategorized, "No sampler set!");
      return;
    }

    ///////////////
    // BEGIN HACK
    ///////////////

    // Check if we are a body mesh of a disabled character and if so, return early
    auto parent = SO()->getParent();
    if (parent && parent->getParent() && parent->getParent()->hasComponent<CharacterAI>())
    {
      parent           = parent->getParent();
      auto characterAi = parent->getComponent<CharacterAI>();
      if (!characterAi->isPhysicsActive())
      {
        return;
      }
    }

    // Check if we are a mesh attached to a bone of a disabled character and if so, return early
    parent = SO()->getParent();
    if (parent && parent->getParent())
    {
      parent = parent->getParent();
      if (parent && parent->getParent() && parent->getParent()->hasComponent<CharacterAI>())
      {
        parent           = parent->getParent();
        auto characterAi = parent->getComponent<CharacterAI>();
        if (!characterAi->isPhysicsActive())
        {
          return;
        }
      }
    }

    ///////////////
    // END HACK
    ///////////////

    ShadowSample sample;
    if (mSampler->sampleFor(SO(), sample))
    {
      auto renderable = SO()->getComponent<bs::CRenderable>();
      renderable->setColor(bs::Color(sample.brightness, sample.brightness, sample.brightness));

      mNeedsUpdate = false;
    }
  }

  void ShadowSampling::onTransformChanged(bs::TransformChangedFlags flags)
  {
    if ((flags & bs::TransformChangedFlags::TCF_Transform) != 0)
    {
      mNeedsUpdate = true;
    }
  }

  void ShadowSampling::updateSamplerFromAncestry()
  {
    // Find the GameWorld component by traversing the ancestry of this scene object
    bs::HSceneObject gameWorldSO =
        findParentIf(SO(), [](const bs::HSceneObject& p) { return p->hasComponent<GameWorld>(); });

    if (gameWorldSO)
    {
      mSampler = gameWorldSO->getComponent<GameWorld>()->shadowsampler();

      if (!mSampler)
      {
        REGOTH_LOG(Warning, Uncategorized,
                   "Found game world but no valid shadow sampler for scene object {0}",
                   SO()->getName());
      }
    }
    else
    {
      REGOTH_THROW(
          bs::InvalidStateException,
          "Could not find the game world by traversing the scene object's parent hierarchy.");
    }
  }

  REGOTH_DEFINE_RTTI(ShadowSampling)

}  // namespace REGoth
