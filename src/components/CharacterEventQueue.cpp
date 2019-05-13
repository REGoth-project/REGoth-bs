#include "CharacterEventQueue.hpp"
#include <exception/Throw.hpp>
#include <components/Character.hpp>
#include <components/CharacterAI.hpp>
#include <RTTI/RTTI_CharacterEventQueue.hpp>
#include <Scene/BsSceneObject.h>

namespace REGoth
{
  CharacterEventQueue::CharacterEventQueue(const bs::HSceneObject& parent)
      : EventQueue(parent)
  {
    setName("CharacterEventQueue");

    mCharacter   = SO()->getComponent<Character>();
    mCharacterAI = SO()->getComponent<CharacterAI>();

    if (!mCharacter)
    {
      REGOTH_THROW(InvalidStateException, "Character component expected!");
    }

    if (!mCharacterAI)
    {
      REGOTH_THROW(InvalidStateException, "CharacterAI component expected!");
    }
  }

  CharacterEventQueue::~CharacterEventQueue()
  {
  }

  void CharacterEventQueue::onExecuteEventAction(SharedEMessage message)
  {
    message->deleted = true;
  }

  void CharacterEventQueue::fixedUpdate()
  {
    EventQueue::fixedUpdate();
  }

  REGOTH_DEFINE_RTTI(CharacterEventQueue)
}  // namespace REGoth
