#include "Character.hpp"
#include <components/Waypoint.hpp>
#include <RTTI/RTTI_Character.hpp>
#include <Scene/BsSceneObject.h>
#include <components/GameWorld.hpp>
#include <components/VisualCharacter.hpp>
#include <components/Waynet.hpp>
#include <scripting/ScriptVMForGameWorld.hpp>

namespace REGoth
{
  Character::Character(const bs::HSceneObject& parent, const bs::String& instance)
      : ScriptBackedBy(parent, "C_NPC", instance)
  {
    setName("Character");
  }

  void Character::onInitialized()
  {
    ScriptBackedBy::onInitialized();
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

    auto wp = gameWorld()->waynet()->findSecondClosestWaypointTo(pos);

    return wp->SO()->getName();
  }

  bs::String Character::getNearestWaypoint()
  {
    const bs::Vector3& pos = SO()->getTransform().pos();

    auto wp = gameWorld()->waynet()->findClosestWaypointTo(pos);

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

  void Character::setToFistMode()
  {
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

  float Character::getDistToPlayer()
  {
    bs::INT32 b;

    return b;
  }

  float Character::getDistToItem(bs::HSceneObject item)
  {
    bs::INT32 b;

    return b;
  }

  float Character::getDistToWaypoint(const bs::String& waypoint)
  {
    bs::INT32 b;

    return b;
  }

  float Character::getDistToNPC(bs::HSceneObject to)
  {
    bs::INT32 b;

    return b;
  }

  REGOTH_DEFINE_RTTI(Character);
}  // namespace REGoth
