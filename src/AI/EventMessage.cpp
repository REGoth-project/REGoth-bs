#include "EventMessage.hpp"
#include <RTTI/RTTI_EventMessages.hpp>

namespace REGoth
{
  namespace AI
  {
    REGOTH_DEFINE_RTTI(EventMessage)
    REGOTH_DEFINE_RTTI(NpcMessage)
    REGOTH_DEFINE_RTTI(DamageMessage)
    REGOTH_DEFINE_RTTI(WeaponMessage)
    REGOTH_DEFINE_RTTI(MovementMessage)
    REGOTH_DEFINE_RTTI(AttackMessage)
    REGOTH_DEFINE_RTTI(UseItemMessage)
    REGOTH_DEFINE_RTTI(StateMessage)
    REGOTH_DEFINE_RTTI(ManipulateMessage)
    REGOTH_DEFINE_RTTI(ConversationMessage)
    REGOTH_DEFINE_RTTI(MagicMessage)
    REGOTH_DEFINE_RTTI(MobMessage)
  }
}
