#include "NeedsGameWorld.hpp"
#include <exception/Throw.hpp>
#include <RTTI/RTTI_NeedsGameWorld.hpp>
#include <components/GameWorld.hpp>

namespace REGoth
{
  NeedsGameWorld::NeedsGameWorld(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("NeedsGameWorld");
    findGameWorld();
  }

  NeedsGameWorld::~NeedsGameWorld()
  {
  }

  void NeedsGameWorld::findGameWorld()
  {
    bs::HSceneObject so = SO();

    // bs:f always puts its own "SceneRoot"-object into the scene which serves
    // as a base for every scene object created by the user.
    // That "SceneRoot" is also the only object which does not have a parent,
    // so the one *before* reaching that is our own root object.
    while (so->getParent() && so->getParent()->getParent())
    {
      so = so->getParent();
    }

    mGameWorld = so->getComponent<GameWorld>();

    if (!mGameWorld)
    {
      REGOTH_THROW(InvalidStateException, "No GameWorld-component found in scene root!");
    }
  }

  REGOTH_DEFINE_RTTI(NeedsGameWorld)
}  // namespace REGoth
