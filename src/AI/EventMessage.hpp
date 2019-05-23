#pragma once

#include <BsCorePrerequisites.h>
#include <AI/WalkMode.hpp>
#include <Math/BsVector3.h>
#include <RTTI/RTTIUtil.hpp>
#include <scripting/ScriptTypes.hpp>

namespace REGoth
{
  namespace AI
  {
    struct EventMessage;
    using SharedEMessage = bs::SPtr<EventMessage>;

    class Waypoint;
    using HWaypoint = bs::GameObjectHandle<Waypoint>;

    class Character;
    using HCharacter = bs::GameObjectHandle<Character>;

    class Item;
    using HItem = bs::GameObjectHandle<Item>;

    enum class EventMessageType
    {
      Event,
      Npc,
      Damage,
      Weapon,
      Movement,
      Attack,
      UseItem,
      State,
      Manipulate,
      Conversation,
      Magic,
      Mob,

      SndPlay,
      SndStarted,
      SndStopp
    };

    /**
     * Basic event-message. Contains type.
     */
    struct EventMessage : public bs::IReflectable
    {
      using SharedEMessage = std::shared_ptr<EventMessage>;

      EventMessage()
      {
        subType        = 0;
        messageType    = EventMessageType::Event;
        isJob          = false;
        deleted        = false;
        isHighPriority = false;
        inUse          = false;
        isOverlay      = false;
      }

      virtual ~EventMessage()
      {
      }

      /**
       * Type of class this can be casted to
       */
      EventMessageType messageType;

      /**
       * Object this was targeted at
       */
      bs::HSceneObject targetObject;

      /**
       * Object this came from
       */
      bs::HSceneObject sourceObject;

      /**
       * Message subtype defined by the child-classes
       */
      unsigned int subType;

      /**
       * Whether this should wait for other messages to complete before triggering
       */
      bool isJob;

      /**
       * Flag used for cleanup
       */
      bool deleted;

      /**
       * Whether this should overwrite existing messages
       */
      bool isHighPriority;

      /**
       * Whether this is currently playing
       */
      bool inUse;

      /**
       * Whether this message should not block further execution of the message-queue
       */
      bool isOverlay;

      /**
       * Whether this is the first time this message is being handled
       */
      bool isFirstRun;

      /**
       * External callbacks to trigger if this message gets processed.
       */
      bs::Event<void(SharedEMessage)> onMessageDone;

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(EventMessage)
    };

    struct NpcMessage : public EventMessage
    {
      NpcMessage()
      {
        messageType = EventMessageType::Npc;
        isJob       = true;
      }

      virtual ~NpcMessage()
      {
      }

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(NpcMessage)
    };

    struct DamageMessage : public NpcMessage
    // FIXME: Reimplement that using a sound handle
    {
      enum DamageSubType : bs::UINT32
      {
        ST_Once,
        ST_PerFrame,
        ST_Max
      };

      DamageMessage()
      {
        messageType = EventMessageType::Damage;
      }

      // TODO: Implement DamageDescriptor!

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(DamageMessage)
    };

    struct WeaponMessage : public NpcMessage
    {
      enum WeaponSubType : bs::UINT32
      {
        ST_DrawWeapon,
        ST_DrawWeapon1,
        ST_DrawWeapon2,
        ST_RemoveWeapon,
        ST_RemoveWeapon1,
        ST_RemoveWeapon2,
        ST_ChooseWeapon,
        ST_ForceRemoveWeapon,
        ST_Attack,
        ST_EquipBestWeapon,
        ST_EquipBestArmor,
        ST_UnequipWeapons,
        ST_UnequipArmor,
        ST_EquipArmor,
        ST_WeaponMax
      };

      WeaponMessage()
      {
        messageType = EventMessageType::Weapon;
      }

      /**
       * Type of weapon to go to
       */
      // TODO: Need EWeapon-Mode!
      // EWeaponMode targetMode;

      // TODO: There is an animation-field here, find out what it does!

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(WeaponMessage)
    };

    struct MovementMessage : public NpcMessage
    {
      enum MovementSubType : bs::UINT32
      {
        ST_RobustTrace,
        ST_GotoPos,
        ST_GotoObject,
        ST_GoRoute,
        ST_Turn,
        ST_TurnToPos,
        ST_TurnToObject,
        ST_TurnAway,
        ST_Jump,
        ST_SetWalkMode,
        ST_WhirlAround,
        ST_Standup,
        ST_CanSeeNpc,
        ST_Strafe,
        ST_GotoFP,
        ST_Dodge,
        ST_BeamTo,
        ST_AlignToFP,
        ST_MoveMax
      };

      MovementMessage()
      {
        messageType = EventMessageType::Movement;
      }

      /**
       * Route the NPC has to go if this is a ST_GoRoute.
       * If empty, targetPos is used.
       */
      bs::Vector<HWaypoint> route;

      /**
       * Object we are going to if this is a ST_GotoVob
       */
      bs::HSceneObject targetObject;

      /**
       * Position we shall go to, if this is a ST_GotoPos or route and name are emtpy
       */
      bs::Vector3 targetPosition;

      /**
       * How we should go to our position
       */
      AI::WalkMode walkMode = AI::WalkMode::Run;

      /**
       * General purpose mode
       */
      int targetMode = 0;

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(MovementMessage)
    };

    struct AttackMessage : public NpcMessage
    {
      enum AttackSubType : bs::UINT32
      {
        ST_AttackForward,
        ST_AttackLeft,
        ST_AttackRight,
        ST_AttackRun,
        ST_AttackFinish,
        ST_Parade,
        ST_AimAt,
        ST_ShootAt,
        ST_StopAim,
        ST_Defend,
        ST_AttackBow,
        ST_AttackMagic,
        ST_AttackMax
      };

      AttackMessage()
      {
        messageType    = EventMessageType::Attack;
        startFrame     = 0.0f;
        animationIndex = -1;
      }

      /**
       * How far we are in our combo
       */
      bs::INT32 combo;

      /**
       * Object we are attacking
       */
      bs::HSceneObject targetObject;

      /**
       * Animation to use when attacking, -1 if none.
       */
      bs::INT32 animationIndex;

      /**
       * Frame to skip the animation to
       */
      float startFrame;

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(AttackMessage)
    };

    struct UseItemMessage : public NpcMessage
    {
      enum UseItemSubType : bs::UINT32
      {
        EV_Drink,
        EV_EquipItem,
        EV_UnequipItem,
        EV_UseItemMax
      };

      UseItemMessage()
      {
        messageType = EventMessageType::UseItem;
      }

      // TODO: We have no actual object for these right now, as they are explicitly in the
      // inventory.
      // TODO: Use the itemhandle here or something

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(UseItemMessage)
    };

    struct StateMessage : public NpcMessage
    {
      enum StateSubType : bs::UINT32
      {
        ST_StartState,
        ST_Wait,
        ST_SetNpcsToState,
        ST_SetTime,
        ST_ApplyTimedOverlay,
        ST_StateMax
      } ConversationMessage;

      StateMessage()
      {
        messageType    = EventMessageType::State;
        isPrgState     = false;
        isRoutineState = false;
      }

      /**
       * Symbol to the Setupfunction for this state (ZS_...)
       */
      Scripting::SymbolIndex functionSymbol;

      /**
       * Whether the old state should be ended properly (true), or just interrupted (false)
       */
      bool endOldState;

      /**
       * Symbols for other and victim
       */
      HCharacter other;
      HCharacter victim;

      /**
       * Whether this belongs to the daily routine
       */
      bool isRoutineState;

      /**
       * Whether this is a program-managed state. List of states in NpcScriptState.h
       */
      bool isPrgState;

      /**
       * If this is a wait-message, this is how long we should wait
       */
      float waitTime;

      /**
       * Waypoint name to got to, in case the state needs that
       */
      bs::String wpname;

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(StateMessage)
    };

    class ManipulateMessage : public NpcMessage
    {
    public:
      enum ManipulateSubType : bs::UINT32
      {
        ST_TakeObject = 0,
        ST_DropObject,
        ST_ThrowObject,
        ST_Exchange,
        ST_UseMob,
        ST_UseItem,
        ST_InsertInteractItem,
        ST_RemoveInteractItem,
        ST_CreateInteractItem,
        ST_DestroyInteractItem,
        ST_PlaceInteractItem,
        ST_ExchangeInteractItem,
        ST_UseMobWithItem,
        ST_CallScript,
        ST_EquipItem,
        ST_UseItemToState,
        ST_TakeMob,
        ST_DropMob,
        ST_ManipMax
      };

      ManipulateMessage()
      {
        messageType = EventMessageType::Manipulate;
        flag        = false;
        pickupAniY  = 0.0f;
        targetState = 0;
      }

      /**
       * Symbol of the item to use. If no item with this symbol can be found in the inventory,
       * nothing happens Alternatively, you can set "targetItem" to a valid handle.
       */
      bs::String itemInstance;

      /**
       * Handle of the item to use. Alternatively you could set "name" to something found in the
       * inventory
       */
      HItem targetItem;

      /**
       * Slot the item should be put in, in case this is a ST_*InteractItem.
       * FIXME: What is a slot?
       */
      bs::String slot;

      /**
       * General purpose flag
       */
      bool flag;

      /**
       * Y-coord of the the pickup animation?
       */
      float pickupAniY;

      /**
       * State to go to when using the item
       */
      bs::INT32 targetState;

      /**
       * Animation to play, for example on pickup
       */
      bs::String animation;

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(ManipulateMessage)
    };

    struct ConversationMessage : public NpcMessage
    {
      // These need to be compatible with scripts
      enum ConversationSubType : bs::UINT32
      {
        ST_PlayAniSound = 0,
        ST_PlayAni,
        ST_PlaySound,
        ST_LookAt,
        ST_Output,
        ST_OutputMonolog,
        ST_OutputEnd,
        ST_Cutscene,
        ST_WaitTillEnd,
        ST_Ask,
        ST_WaitForQuestion,
        ST_StopLookAt,
        ST_StopPointAt,

        ST_PointAt,
        ST_QuickLook,
        ST_PlayAni_NoOverlay,
        ST_PlayAni_Face,

        ST_ProcessInfos,
        ST_StopProcessInfos,
        ST_OutputSVM_Overlay,
        ST_SndPlay,
        ST_SndPlay3d,
        ST_PrintScreen,
        ST_StartFx,
        ST_StopFx,
        ST_ConvMax
      };

      enum class Status
      {
        INIT,  // never seen by PlayerController yet
        PLAYING,
        FADING_OUT
      };

      ConversationMessage()
      {
        messageType = EventMessageType::Conversation;
        status      = Status::INIT;
        canceled    = false;
      }

      /**
       * Shortcut-function to play animations
       */
      static ConversationMessage playAnimation(const bs::String& anim)
      {
        ConversationMessage msg;
        msg.subType   = ST_PlayAni;
        msg.animation = anim;

        return msg;
      }

      /**
       * Text to be displayed in the subtitle box
       */
      bs::String text;

      /**
       * Name of the SFX to play
       */
      bs::String name;

      /**
       * Target-object to look at
       */
      bs::HSceneObject target;

      /**
       * Target-pos to look at
       */
      bs::Vector3 targetPosition;

      /**
       * Animation to play. -1 if none
       */
      bs::INT32 animationIndex;

      /**
       * Animation name to be used. If empty, index will be checked.
       */
      bs::String animation;

      /**
       * Stores the state of the message, to handle multiple stages/states inside the
       * Playercontroller
       */
      Status status;

      /**
       * Whether this message has been kindly canceled by the ouside (i.e. DialogManager).
       */
      bool canceled;

      /**
       * Ticket. Can be used to ask AudioWorld if sound is playing.
       */
      // FIXME: Reimplement that using a sound handle
      // Utils::Ticket<World::AudioWorld> soundTicket;

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(ConversationMessage)
    };

    struct MagicMessage : public NpcMessage
    {
      enum MagicSubType : bs::UINT32
      {
        ST_Open,
        ST_Close,
        ST_Move,
        ST_Invest,
        ST_Cast,
        ST_SetLevel,
        ST_ShowSymbol,
        ST_SetFrontSpell,
        ST_CastSpell,
        ST_Ready,
        ST_Unready,
        ST_MagicMax
      };

      MagicMessage()
      {
        messageType = EventMessageType::Magic;
      }

      // TODO: Implement

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(MagicMessage)
    };

    struct MobMessage : public EventMessage
    {
      enum MobSubType : bs::UINT32
      {
        ST_StartInteraction = 0,
        ST_StartSTateChange,
        ST_EndInteraction,
        ST_Unlock,
        ST_Lock,
        ST_Callscript
      };

      MobMessage()
      {
        messageType = EventMessageType::Mob;
        stateFrom   = 0;
        stateTo     = 0;
      }

      /** NPC this message is from */
      bs::HSceneObject npc;

      /**
       * State to change from.
       */
      bs::INT32 stateFrom;

      /**
       * State to change to.
       */
      bs::INT32 stateTo;

      /**
       * Whether to play an animation.
       */
      bool playAnimation;

      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(MobMessage)
    };
  }  // namespace AI
}  // namespace REGoth
