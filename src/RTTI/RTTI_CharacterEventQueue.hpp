#include "RTTIUtil.hpp"
#include <components/CharacterEventQueue.hpp>

namespace REGoth
{
  class RTTI_CharacterEventQueue
      : public bs::RTTIType<CharacterEventQueue, EventQueue, RTTI_CharacterEventQueue>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_CharacterEventQueue()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(CharacterEventQueue)
  };
}  // namespace REGoth
