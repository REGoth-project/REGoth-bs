#include "CharacterEventQueue.hpp"
#include <AI/ScriptState.hpp>
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

  void CharacterEventQueue::onInitialized()
  {
    EventQueue::onInitialized();

    // Must create these here as we need the handle to this component, which is
    // not valid in the constructor. The null-checks are for when we're getting
    // deserialized.

    if (!mPathfinder)
    {
      mPathfinder = bs::bs_shared_ptr_new<AI::Pathfinder>(mWorld->waynet());
    }

    if (!mScriptState)
    {
      HCharacterEventQueue hthis = bs::static_object_cast<CharacterEventQueue>(getHandle());

      mScriptState = bs::bs_shared_ptr_new<AI::ScriptState>(mWorld, mCharacter, hthis, mCharacterAI);
    }
  }

  void CharacterEventQueue::startRouteToPosition(const bs::Vector3& target)
  {
    mPathfinder->startNewRouteTo(positionNow(), target);
  }

  void CharacterEventQueue::startRouteToObject(bs::HSceneObject target)
  {
    mPathfinder->startNewRouteTo(positionNow(), target);
  }

  const bs::Vector3& CharacterEventQueue::positionNow() const
  {
    return SO()->getTransform().pos();
  }

  void CharacterEventQueue::travelActiveRoute()
  {
    bs::Vector3 pos                  = positionNow();
    AI::Pathfinder::Instruction inst = mPathfinder->updateToNextInstructionToTarget(pos);

    if (!mPathfinder->isTargetReachedByPosition(pos, inst.targetPosition))
    {
      // TODO: Might want to smoothly turn instead
      mCharacterAI->instantTurnToPosition(inst.targetPosition);
    }

    mCharacterAI->goForward();
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
        if (message.isFirstRun)
        {
          startRouteToObject(message.targetObject);
        }

        if (!mPathfinder->hasActiveRouteBeenCompleted(positionNow()))
        {
          travelActiveRoute();
        }
        else
        {
          mCharacterAI->stopMoving();

          isDone = true;
        }
        break;

      case AI::MovementMessage::ST_GotoPos:
        if (message.isFirstRun)
        {
          startRouteToPosition(message.targetPosition);
        }

        if (!mPathfinder->hasActiveRouteBeenCompleted(positionNow()))
        {
          travelActiveRoute();
        }
        else
        {
          mCharacterAI->stopMoving();

          isDone = true;
        }
        break;

      case AI::MovementMessage::ST_SetWalkMode:
        mCharacterAI->setWalkMode(message.walkMode);
        isDone = true;
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
    bool isDone = false;

    switch ((AI::StateMessage::StateSubType)message.subType)
    {
      case AI::StateMessage::ST_Wait:
        message.waitTime -= bs::gTime().getFixedFrameDelta();

        if (message.waitTime <= 0)
        {
          isDone = true;
        }
        break;

      default:
        bs::gDebug().logWarning("[CharacterEventQueue] Unhandled StateMessage-Sub Type: " +
                                bs::toString((int)message.subType));
        isDone = true;
        break;
    }

    return isDone;
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

    if (mCharacterAI->isPhysicsActive())
    {
      mScriptState->doAIState(bs::gTime().getFixedFrameDelta());
    }
    else
    {
      mScriptState->doAIStateDuringShrink();
    }
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

  SharedEMessage CharacterEventQueue::pushSetWalkMode(AI::WalkMode walkMode)
  {
    AI::MovementMessage msg;
    msg.subType  = AI::MovementMessage::ST_SetWalkMode;
    msg.walkMode = walkMode;

    return onMessage(msg);
  }

  SharedEMessage CharacterEventQueue::pushWait(float seconds)
  {
    AI::StateMessage msg;

    msg.subType  = AI::StateMessage::ST_Wait;
    msg.waitTime = seconds;

    return onMessage(msg);
  }

  void CharacterEventQueue::insertRoutineTask(const AI::ScriptState::RoutineTask& task)
  {
    mScriptState->insertRoutineTask(task);
  }

  void CharacterEventQueue::reinitRoutine()
  {
    mScriptState->reinitRoutine();
  }

  REGOTH_DEFINE_RTTI(CharacterEventQueue)
}  // namespace REGoth
