#include "RTTIUtil.hpp"
#include <components/EventQueue.hpp>

namespace REGoth
{
  class RTTI_EventQueue : public bs::RTTIType<EventQueue, bs::Component, RTTI_EventQueue>
  {
    using UINT32 = bs::UINT32;

    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFLPTR_ARRAY(mEventQueue, 0)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_EventQueue()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_ABSTRACT(EventQueue)
  };
}  // namespace REGoth
