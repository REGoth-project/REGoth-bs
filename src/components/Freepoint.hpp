#pragma once
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  /**
   * TODO: Documentation of Freepoint
   */
  class Freepoint : public bs::Component
  {
  public:
    Freepoint(const bs::HSceneObject& parent);
    virtual ~Freepoint();

  private:
  public:
    REGOTH_DECLARE_RTTI(Freepoint)

  protected:
    Freepoint() = default;  // For RTTI
  };
}  // namespace REGoth
