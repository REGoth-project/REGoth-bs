#include "CharacterEventQueue.hpp"
#include <AI/ScriptState.hpp>
#include <RTTI/RTTI_CharacterEventQueue.hpp>
#include <Scene/BsSceneObject.h>
#include <components/Character.hpp>
#include <components/CharacterAI.hpp>
#include <components/GameWorld.hpp>
#include <components/VisualCharacter.hpp>
#include <exception/Throw.hpp>
#include <scripting/ScriptVMForGameWorld.hpp>

namespace REGoth
{
  using SharedEMessage = CharacterEventQueue::SharedEMessage;

  CharacterEventQueue::CharacterEventQueue(const bs::HSceneObject& parent, HCharacter character,
                                           HCharacterAI characterAI,
                                           HVisualCharacter visualCharacter, HGameWorld world)
      : EventQueue(parent)
      , mCharacter(character)
      , mCharacterAI(characterAI)
      , mVisualCharacter(visualCharacter)
      , mWorld(world)
  {
    setName("CharacterEventQueue");
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
        BS_LOG(Warning, Uncategorized,
               "[CharacterEventQueue] Unhandled Event Type: " +
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
    bool isDone = false;
    switch ((AI::WeaponMessage::WeaponSubType)message.subType)
    {
      case AI::WeaponMessage::ST_ChooseWeapon:
        mCharacterAI->changeWeaponMode(message.targetMode);
        isDone = true;
        break;
      default:
        BS_LOG(Warning, Uncategorized,
               "[CharacterEventQueue] Unhandled WeaponMode-Sub Type: " +
                   bs::toString((int)message.subType));
        isDone = true;
        break;
    }

    return isDone;
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
        mCharacterAI->changeWalkMode(message.walkMode);
        isDone = true;
        break;

      default:
        BS_LOG(Warning, Uncategorized,
               "[CharacterEventQueue] Unhandled MovementMessage-Sub Type: " +
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
      case AI::StateMessage::ST_StartState:

        mCharacter->useAsSelf();

        if (message.other)
        {
          message.other->useAsOther();
        }

        if (message.victim)
        {
          message.victim->useAsVictim();
        }

        if (message.state.empty())
        {
          mScriptState->startDailyRoutine(true);
        }
        else
        {
          mScriptState->startScriptAIState(message.state);
        }

        if (message.interruptOldState)
        {
          mScriptState->interruptActiveState();
        }
        else
        {
          mScriptState->requestEndActiveState();
        }

        mScriptState->applyStateChange();

        isDone = true;
        break;

      case AI::StateMessage::ST_Wait:
        message.waitTime -= bs::gTime().getFixedFrameDelta();

        if (message.waitTime <= 0)
        {
          isDone = true;
        }
        break;

      default:
        BS_LOG(Warning, Uncategorized,
               "[CharacterEventQueue] Unhandled StateMessage-Sub Type: " +
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
    bool isDone = false;

    switch ((AI::ConversationMessage::ConversationSubType)message.subType)
    {
      case AI::ConversationMessage::ST_PlayAni:

        if (message.isFirstRun)
        {
          // BS_LOG(Info, Uncategorized, bs::StringUtil::format(
          //     "[CharacterEventQueue] {0} - PlayAni start: {1}", SO()->getName(),
          //     message.animation));

          message.playingClip = mVisualCharacter->findAnimationClip(message.animation);

          if (message.playingClip)
          {
            mVisualCharacter->playAnimationClip(message.playingClip);
          }
          else
          {
            isDone = true;
          }
        }
        else
        {
          isDone = !mVisualCharacter->isAnimationPlaying(message.playingClip);

          if (isDone)
          {
            // BS_LOG(Info, Uncategorized, 
            //     bs::StringUtil::format("[CharacterEventQueue] {0} - PlayAni done: {1}",
            //                            SO()->getName(), message.animation));
          }
        }
        break;

      default:
        BS_LOG(Warning, Uncategorized,
               "[CharacterEventQueue] Unhandled Conversation-Sub Type: " +
                   bs::toString((int)message.subType));
        isDone = true;
        break;
    }

    return isDone;
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

  SharedEMessage CharacterEventQueue::pushTalkToCharacter(HCharacter other)
  {
    return pushInterruptAndStartScriptState("ZS_TALK", "", other, {});
  }

  SharedEMessage CharacterEventQueue::pushStartScriptState(const bs::String& state,
                                                           const bs::String& waypoint,
                                                           HCharacter other, HCharacter victim)
  {
    AI::StateMessage msg;

    msg.isRoutineState    = false;
    msg.isPrgState        = false;
    msg.interruptOldState = false;

    msg.subType = AI::StateMessage::ST_StartState;
    msg.wpname  = waypoint;
    msg.other   = other;
    msg.victim  = victim;
    msg.state   = state;

    return onMessage(msg);
  }

  SharedEMessage CharacterEventQueue::pushInterruptAndStartScriptState(const bs::String& state,
                                                                       const bs::String& waypoint,
                                                                       HCharacter other,
                                                                       HCharacter victim)
  {
    AI::StateMessage msg;

    msg.isRoutineState    = false;
    msg.isPrgState        = false;
    msg.interruptOldState = true;

    msg.subType = AI::StateMessage::ST_StartState;
    msg.wpname  = waypoint;
    msg.other   = other;
    msg.victim  = victim;
    msg.state   = state;

    return onMessage(msg);
  }

  SharedEMessage CharacterEventQueue::pushPlayAnimation(const bs::String animation)
  {
    AI::ConversationMessage msg;

    msg.subType   = AI::ConversationMessage::ST_PlayAni;
    msg.animation = animation;

    return onMessage(msg);
  }

  SharedEMessage CharacterEventQueue::pushGoToFistModeImmediate()
  {
    AI::WeaponMessage msg;

    msg.subType        = AI::WeaponMessage::ST_ChooseWeapon;
    msg.targetMode     = AI::WeaponMode::Fist;
    msg.isJob          = false;
    msg.isHighPriority = true;

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

  float CharacterEventQueue::getCurrentStateRunningTime() const
  {
    return mScriptState->getCurrentStateRunningTime();
  }

  REGOTH_DEFINE_RTTI(CharacterEventQueue)
}  // namespace REGoth
