#include "Freepoint.hpp"
#include <RTTI/RTTI_Freepoint.hpp>

namespace REGoth
{
  Freepoint::Freepoint(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("Freepoint");
  }

  Freepoint::~Freepoint()
  {
  }

  REGOTH_DEFINE_RTTI(Freepoint)

}  // namespace REGoth
