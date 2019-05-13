#include "RTTIUtil.hpp"
#include <components/EventQueue.hpp>

namespace REGoth
{
  class RTTI_EventQueue : public bs::RTTIType<EventQueue, bs::Component, RTTI_EventQueue>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_EventQueue()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_ABSTRACT(EventQueue)
  };
}  // namespace REGoth
