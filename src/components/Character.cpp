#include "Character.hpp"
#include <components/Inventory.hpp>
#include <Components/BsCCharacterController.h>
#include <RTTI/RTTI_Character.hpp>
#include <Scene/BsSceneObject.h>
#include <components/CharacterAI.hpp>
#include <components/CharacterEventQueue.hpp>
#include <components/GameWorld.hpp>
#include <components/StoryInformation.hpp>
#include <components/VisualCharacter.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>
#include <log/logging.hpp>
#include <scripting/ScriptVMForGameWorld.hpp>

namespace REGoth
{
  Character::Character(const bs::HSceneObject& parent, const bs::String& instance,
                       HGameWorld gameWorld)
      : ScriptBackedBy(parent, "C_NPC", instance, gameWorld)
  {
    setName("Character");
  }

  void Character::onInitialized()
  {
    // Only run this when we're not getting deserialized
    if (!hasInstantiatedScriptObject())
    {
      HCharacter thisCharacter = bs::static_object_cast<Character>(getHandle());

      auto controller = SO()->addComponent<bs::CCharacterController>();

      // FIXME: Assign the radius and height set via the visuals bounding box
      controller->setRadius(0.35f);
      controller->setHeight(0.5f);

      auto visual    = SO()->addComponent<VisualCharacter>();
      auto ai        = SO()->addComponent<CharacterAI>(gameWorld());
      auto inventory = SO()->addComponent<Inventory>();

      auto eventQueue =
          SO()->addComponent<CharacterEventQueue>(thisCharacter, ai, visual, gameWorld());

      ScriptBackedBy::onInitialized();

      // Needs a valid script object to initialize
      auto infos = SO()->addComponent<StoryInformation>(gameWorld(), thisCharacter);

      // If we don't init the routine now, the character won't have its default routine
      // Must also come *after* the script object has been created since this might run
      // some scripts.
      eventQueue->reinitRoutine();
    }
  }

  void Character::useAsHero()
  {
    gameWorld()->scriptVM().setHero(scriptObject());
  }

  void Character::useAsSelf()
  {
    gameWorld()->scriptVM().setSelf(scriptObject());
  }

  void Character::useAsVictim()
  {
    gameWorld()->scriptVM().setVictim(scriptObject());
  }

  void Character::useAsOther()
  {
    gameWorld()->scriptVM().setOther(scriptObject());
  }

  void Character::handleUnconsciousness()
  {
    // TODO: See how old REGoth implmented checkUnconscious()
  }

  bool Character::isReady()
  {
    // TODO: Implement
    return true;
  }

  bool Character::checkInfo(bool important)
  {
    return true;
  }

  void Character::removeInventoryItems(const bs::String& instance, bs::INT32 amount)
  {
  }

  void Character::equipItem(const bs::String& instance)
  {
  }

  void Character::createInventoryItem(const bs::String& instance, bs::INT32 num)
  {
  }

  void Character::giveItem(const bs::String& instance, bs::HSceneObject characterToSO)
  {
  }

  bool Character::hasItem(const bs::String& instance)
  {
    return true;
  }

  void Character::clearInventory()
  {
  }

  bool Character::knowsInfo(const bs::String& instance)
  {
    return true;
  }

  bool Character::hasEquippedMelleWeapon()
  {
    return true;
  }

  bool Character::isInState(const bs::INT32 state)
  {
    return true;
  }

  bool Character::isStateDriven()
  {
    return scriptObjectData().functionPointerValue("START_AISTATE") != 0;
  }

  const bs::String& Character::getStartAIState()
  {
    bs::UINT32 address = scriptObjectData().functionPointerValue("START_AISTATE");

    Scripting::SymbolIndex symbol = scriptVM().scriptSymbols().findFunctionByAddress(address);

    return scriptVM().scriptSymbols().getSymbolName(symbol);
  }

  bs::String Character::dailyRoutine()
  {
    bs::UINT32 address = scriptObjectData().functionPointerValue("DAILY_ROUTINE");

    Scripting::SymbolIndex symbol = scriptVM().scriptSymbols().findFunctionByAddress(address);

    if (symbol == Scripting::SYMBOL_INDEX_INVALID)
    {
      return "";
    }

    return scriptVM().scriptSymbols().getSymbolName(symbol);
  }

  void Character::setDailyRoutine(const bs::String& newDailyRoutine)
  {
    if (newDailyRoutine.empty())
    {
      scriptObjectData().functionPointerValue("DAILY_ROUTINE") = 0;
    }
    else
    {
      bs::UINT32 id = scriptObjectData().intValue("ID");

      bs::String functionName = "RTN_" + newDailyRoutine + "_" + bs::toString(id);

      REGOTH_LOG(Info, Uncategorized, "[Character] Set Routine of {0} to {1}", SO()->getName(),
                 functionName);

      const auto& fn =
          scriptVM().scriptSymbols().getSymbol<Scripting::SymbolScriptFunction>(functionName);

      scriptObjectData().functionPointerValue("DAILY_ROUTINE") = fn.address;
    }
  }

  void Character::refuseTalk()
  {
  }

  void Character::setRefuseTalk(bs::INT32 durationSeconds)
  {
  }

  void Character::setCurrentWaypoint(const bs::String& waypoint)
  {
    scriptObjectData().stringValue("WP") = waypoint;
  }

  const bs::String& Character::currentWaypoint() const
  {
    return scriptObjectData().stringValue("WP");
  }

  bs::String Character::getNextWaypoint()
  {
    const bs::Vector3& pos = SO()->getTransform().pos();

    auto wp = gameWorld()->waynet()->findClosestWaypointTo(pos).secondClosest;

    return wp->SO()->getName();
  }

  bs::String Character::getNearestWaypoint()
  {
    const bs::Vector3& pos = SO()->getTransform().pos();

    auto wp = gameWorld()->waynet()->findClosestWaypointTo(pos).closest;

    return wp->SO()->getName();
  }

  void Character::exchangeRoutine(const bs::String& routineName)
  {
  }

  void Character::clearAiQueue()
  {
  }

  bool Character::canSeeItem(bs::HSceneObject itemSO)
  {
    return true;
  }

  bool Character::canSeeNpcFreeLOS(bs::HSceneObject targetCharacterSO)
  {
    return true;
  }

  bool Character::canSeeNPC(bs::HSceneObject targetCharacterSO)
  {
    return true;
  }

  bool Character::isOnFreepoint(const bs::String& namePart)
  {
    return true;
  }

  bool Character::isDead()
  {
    return true;
  }

  bool Character::isPlayer()
  {
    return scriptVM().heroInstance() == scriptObject();
  }

  void Character::setToFightMode(const bs::String& weapon)
  {
  }

  bs::INT32 Character::setTrueGuild(bs::INT32 guild)
  {
    bs::INT32 b;

    return b;
  }

  bs::INT32 Character::getTrueGuild()
  {
    bs::INT32 b;

    return b;
  }

  float Character::getDistanceToHero() const
  {
    HCharacter hero = gameWorld()->hero();

    return getDistanceToObject(hero->SO());
  }

  float Character::getDistanceToWaypoint(const bs::String& waypoint) const
  {
    HWaypoint wp = gameWorld()->waynet()->findWaypoint(waypoint);

    if (!wp)
    {
      REGOTH_LOG(Warning, Uncategorized,
                 "[Character] Waypoint {0} does not exist! (getDistanceToWaypoint)", waypoint);
      return -1.0f;
    }

    return getDistanceToObject(wp->SO());
  }

  bool Character::isNearCharacter(HCharacter other) const
  {
    // 3 meters is expected by the scripts, according to `externals.d`
    return getDistanceToObject(other->SO()) < 3.0f;
  }

  float Character::getDistanceToObject(bs::HSceneObject object) const
  {
    auto& p1 = SO()->getTransform().pos();
    auto& p2 = object->getTransform().pos();

    return p1.distance(p2);
  }

  const bs::Vector<Scripting::ScriptObjectHandle>& Character::allInfosForThisCharacter() const
  {
    return scriptVM().allInfosOfNpc(scriptObjectData().instanceName);
  }

  bs::Vector<HCharacter> Character::findCharactersInRange(float range) const
  {
    return gameWorld()->findCharactersInRange(range, SO()->getTransform().pos());
  }

  REGOTH_DEFINE_RTTI(Character);
}  // namespace REGoth
