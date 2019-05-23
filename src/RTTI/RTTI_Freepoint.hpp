#include "RTTIUtil.hpp"
#include <components/Freepoint.hpp>

namespace REGoth
{
  class RTTI_Freepoint : public bs::RTTIType<Freepoint, bs::Component, RTTI_Freepoint>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Freepoint()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Freepoint)
  };
}  // namespace REGoth
