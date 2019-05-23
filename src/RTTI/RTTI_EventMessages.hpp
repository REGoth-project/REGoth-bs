#include <AI/EventMessage.hpp>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  namespace AI
  {
    using UINT32 = bs::UINT32;

    class RTTI_EventMessage : public bs::RTTIType<EventMessage, bs::IReflectable, RTTI_EventMessage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(messageType, 0)
      BS_RTTI_MEMBER_REFL(targetObject, 1)
      BS_RTTI_MEMBER_REFL(sourceObject, 2)
      BS_RTTI_MEMBER_PLAIN(subType, 3)
      BS_RTTI_MEMBER_PLAIN(isJob, 4)
      BS_RTTI_MEMBER_PLAIN(deleted, 5)
      BS_RTTI_MEMBER_PLAIN(isHighPriority, 6)
      BS_RTTI_MEMBER_PLAIN(inUse, 7)
      BS_RTTI_MEMBER_PLAIN(isOverlay, 8)
      BS_RTTI_MEMBER_PLAIN(isFirstRun, 9)
      // FIXME: Can't serialize callback events!
      BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(EventMessage)
    };

    class RTTI_NpcMessage : public bs::RTTIType<NpcMessage, EventMessage, RTTI_NpcMessage>
    {
      // BS_BEGIN_RTTI_MEMBERS
      // BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(NpcMessage)
    };

    class RTTI_DamageMessage : public bs::RTTIType<NpcMessage, NpcMessage, RTTI_NpcMessage>
    {
      // BS_BEGIN_RTTI_MEMBERS
      // BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(DamageMessage)
    };

    class RTTI_WeaponMessage : public bs::RTTIType<WeaponMessage, NpcMessage, RTTI_WeaponMessage>
    {
      // BS_BEGIN_RTTI_MEMBERS
      // BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(WeaponMessage)
    };

    class RTTI_MovementMessage
        : public bs::RTTIType<MovementMessage, NpcMessage, RTTI_MovementMessage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_REFL_ARRAY(route, 0)
      BS_RTTI_MEMBER_REFL(targetObject, 1)
      BS_RTTI_MEMBER_PLAIN(targetPosition, 2)
      BS_RTTI_MEMBER_PLAIN(walkMode, 3)
      BS_RTTI_MEMBER_PLAIN(targetMode, 4)
      BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(MovementMessage)
    };

    class RTTI_AttackMessage : public bs::RTTIType<AttackMessage, NpcMessage, RTTI_AttackMessage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(combo, 0)
      BS_RTTI_MEMBER_REFL(targetObject, 1)
      BS_RTTI_MEMBER_PLAIN(animationIndex, 2)
      BS_RTTI_MEMBER_PLAIN(startFrame, 3)
      BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(AttackMessage)
    };

    class RTTI_UseItemMessage : public bs::RTTIType<UseItemMessage, NpcMessage, RTTI_UseItemMessage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(UseItemMessage)
    };

    class RTTI_StateMessage : public bs::RTTIType<StateMessage, NpcMessage, RTTI_StateMessage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(state, 0)
      BS_RTTI_MEMBER_PLAIN(interruptOldState, 1)
      BS_RTTI_MEMBER_REFL(other, 2)
      BS_RTTI_MEMBER_REFL(victim, 3)
      BS_RTTI_MEMBER_PLAIN(isRoutineState, 4)
      BS_RTTI_MEMBER_PLAIN(isPrgState, 5)
      BS_RTTI_MEMBER_PLAIN(waitTime, 6)
      BS_RTTI_MEMBER_PLAIN(wpname, 7)
      BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(StateMessage)
    };

    class RTTI_ManipulateMessage
        : public bs::RTTIType<ManipulateMessage, NpcMessage, RTTI_ManipulateMessage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(itemInstance, 0)
      BS_RTTI_MEMBER_REFL(targetItem, 1)
      BS_RTTI_MEMBER_PLAIN(slot, 2)
      BS_RTTI_MEMBER_PLAIN(flag, 3)
      BS_RTTI_MEMBER_PLAIN(pickupAniY, 4)
      BS_RTTI_MEMBER_PLAIN(targetState, 5)
      BS_RTTI_MEMBER_PLAIN(animation, 6)
      BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(ManipulateMessage)
    };

    class RTTI_ConversationMessage
        : public bs::RTTIType<ConversationMessage, NpcMessage, RTTI_ConversationMessage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(text, 0)
      BS_RTTI_MEMBER_PLAIN(name, 1)
      BS_RTTI_MEMBER_REFL(target, 2)
      BS_RTTI_MEMBER_PLAIN(targetPosition, 3)
      BS_RTTI_MEMBER_REFL(playingClip, 4)
      BS_RTTI_MEMBER_PLAIN(animation, 5)
      BS_RTTI_MEMBER_PLAIN(status, 6)
      BS_RTTI_MEMBER_PLAIN(canceled, 7)
      BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(ConversationMessage)
    };

    class RTTI_MagicMessage : public bs::RTTIType<MagicMessage, NpcMessage, RTTI_MagicMessage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(MagicMessage)
    };

    class RTTI_MobMessage : public bs::RTTIType<MobMessage, EventMessage, RTTI_MobMessage>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_REFL(npc, 0)
      BS_RTTI_MEMBER_PLAIN(stateFrom, 1)
      BS_RTTI_MEMBER_PLAIN(stateTo, 2)
      BS_RTTI_MEMBER_PLAIN(playAnimation, 3)
      BS_END_RTTI_MEMBERS

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(MobMessage)
    };
  }  // namespace AI
}  // namespace REGoth
