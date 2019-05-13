#include "CharacterEventQueue.hpp"
#include <RTTI/RTTI_CharacterEventQueue.hpp>
#include <Scene/BsSceneObject.h>
#include <components/Character.hpp>
#include <components/CharacterAI.hpp>
#include <components/GameWorld.hpp>
#include <exception/Throw.hpp>

namespace REGoth
{
  using SharedEMessage = CharacterEventQueue::SharedEMessage;

  CharacterEventQueue::CharacterEventQueue(const bs::HSceneObject& parent, HGameWorld world)
      : EventQueue(parent)
      , mWorld(world)
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

  void CharacterEventQueue::onExecuteEventAction(SharedEMessage message, bs::HSceneObject sender)
  {
    using namespace AI;

    bool done = false;
    switch (message->messageType)
    {
      case EventMessageType::Event:
        done = EV_Event(*message.get(), sender);
        break;

      case EventMessageType::Npc:
        done = EV_Npc(*reinterpret_cast<NpcMessage*>(message.get()), sender);
        break;

      case EventMessageType::Damage:
        done = EV_Damage(*reinterpret_cast<DamageMessage*>(message.get()), sender);
        break;

      case EventMessageType::Weapon:
        done = EV_Weapon(*reinterpret_cast<WeaponMessage*>(message.get()), sender);
        break;

      case EventMessageType::Movement:
        done = EV_Movement(*reinterpret_cast<MovementMessage*>(message.get()), sender);
        break;

      case EventMessageType::Attack:
        done = EV_Attack(*reinterpret_cast<AttackMessage*>(message.get()), sender);
        break;

      case EventMessageType::UseItem:
        done = EV_UseItem(*reinterpret_cast<UseItemMessage*>(message.get()), sender);
        break;

      case EventMessageType::State:
        done = EV_State(*reinterpret_cast<StateMessage*>(message.get()), sender);
        break;

      case EventMessageType::Manipulate:
        done = EV_Manipulate(*reinterpret_cast<ManipulateMessage*>(message.get()), sender);
        break;

      case EventMessageType::Conversation:
        done = EV_Conversation(*reinterpret_cast<ConversationMessage*>(message.get()), sender);
        break;

      case EventMessageType::Magic:
        done = EV_Magic(*reinterpret_cast<MagicMessage*>(message.get()), sender);
        break;

      case EventMessageType::Mob:
        // TODO handle this somehow?
        break;

      default:
        bs::gDebug().logWarning("[CharacterEventQueue] Unhandled Event Type: " +
                                bs::toString((int)message->messageType));
        done = true;
        break;
    }

    // Flag as deleted if this is done
    message->deleted = done;
  }

  bool CharacterEventQueue::EV_Event(AI::EventMessage& message, bs::HSceneObject sender)
  {
    bool done = false;
    done      = true;  // TODO: Implement events
    return done;
  }

  bool CharacterEventQueue::EV_Npc(AI::NpcMessage& message, bs::HSceneObject sender)
  {
    bool done = false;

    done = true;  // TODO: Implement events
    return done;
  }

  bool CharacterEventQueue::EV_Damage(AI::DamageMessage& message, bs::HSceneObject sender)
  {
    bool done = false;

    done = true;  // TODO: Implement events
    return done;
  }

  bool CharacterEventQueue::EV_Weapon(AI::WeaponMessage& message, bs::HSceneObject sender)
  {
    bool done = false;

    done = true;  // TODO: Implement events
    return done;
  }

  bool CharacterEventQueue::EV_Movement(AI::MovementMessage& message, bs::HSceneObject sender)
  {
    bool isDone = false;
    switch ((AI::MovementMessage::MovementSubType)message.subType)
    {
      case AI::MovementMessage::ST_GotoObject:
        mCharacterAI->gotoPositionStraight(message.targetObject->getTransform().pos());
        isDone = mCharacterAI->isAtPosition(message.targetObject->getTransform().pos());
        break;

      case AI::MovementMessage::ST_GotoPos:
        mCharacterAI->gotoPositionStraight(message.targetPosition);
        isDone = mCharacterAI->isAtPosition(message.targetPosition);
        break;

      default:
        bs::gDebug().logWarning("[CharacterEventQueue] Unhandled MovementMessage-Sub Type: " +
                                bs::toString((int)message.subType));
        isDone = true;
        break;
    }

    return isDone;
  }

  bool CharacterEventQueue::EV_Attack(AI::AttackMessage& message, bs::HSceneObject sender)
  {
    bool done = false;

    done = true;  // TODO: Implement events
    return done;
  }

  bool CharacterEventQueue::EV_UseItem(AI::UseItemMessage& message, bs::HSceneObject sender)
  {
    bool done = false;

    done = true;  // TODO: Implement events
    return done;
  }

  bool CharacterEventQueue::EV_State(AI::StateMessage& message, bs::HSceneObject sender)
  {
    bool done = false;

    done = true;  // TODO: Implement events
    return done;
  }

  bool CharacterEventQueue::EV_Manipulate(AI::ManipulateMessage& message, bs::HSceneObject sender)
  {
    bool done = false;

    done = true;  // TODO: Implement events
    return done;
  }

  bool CharacterEventQueue::EV_Conversation(AI::ConversationMessage& message,
                                            bs::HSceneObject sender)
  {
    bool done = false;

    done = true;  // TODO: Implement events
    return done;
  }

  bool CharacterEventQueue::EV_Magic(AI::MagicMessage& message, bs::HSceneObject sender)
  {
    bool done = false;

    done = true;  // TODO: Implement events
    return done;
  }

  void CharacterEventQueue::fixedUpdate()
  {
    EventQueue::fixedUpdate();
  }

  SharedEMessage CharacterEventQueue::pushGotoPosition(const bs::Vector3& position)
  {
    AI::MovementMessage msg;

    msg.subType        = AI::MovementMessage::ST_GotoPos;
    msg.targetPosition = position;

    return onMessage(msg);
  }

  SharedEMessage CharacterEventQueue::pushGotoObject(bs::HSceneObject object)
  {
    AI::MovementMessage msg;

    msg.subType      = AI::MovementMessage::ST_GotoObject;
    msg.targetObject = object;

    return onMessage(msg);
  }

  REGOTH_DEFINE_RTTI(CharacterEventQueue)
}  // namespace REGoth
