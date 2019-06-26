#include "RTTIUtil.hpp"
#include <components/Sky.hpp>

namespace REGoth
{
  class RTTI_Sky
      : public bs::RTTIType<Sky, bs::Component, RTTI_Sky>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mGameWorld, 0)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_Sky()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(Sky)
  };
}  // namespace REGoth
