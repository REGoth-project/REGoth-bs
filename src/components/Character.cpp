#include "Character.hpp"
#include <RTTI/RTTI_Character.hpp>
#include <Scene/BsSceneObject.h>
#include <components/GameWorld.hpp>
#include <components/VisualCharacter.hpp>
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

  void Character::refuseTalk()
  {
  }

  void Character::setRefuseTalk(bs::INT32 durationSeconds)
  {
  }

  bs::String Character::getNextWaypoint()
  {
    bs::String b;

    return b;
  }

  bs::String Character::getNearestWaypoint()
  {
    bs::String b;

    return b;
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
    return true;
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
