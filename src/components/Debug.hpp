#pragma once
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>
#include <RTTI/RTTIUtil.hpp>
#include <Input/BsVirtualInput.h>

namespace REGoth
{
  /**
   * This components handles all secondary debug functionality such as debug input or
   * instantiation of the Debug functions and UI elements.
   */
  class Debug : public bs::Component
  {
  public:
    Debug(const bs::HSceneObject& parent);
    virtual ~Debug();

  protected:
    void onInitialized() override;
    void update() override;

  private:
    bs::VirtualButton mFastMove;

  public:
    REGOTH_DECLARE_RTTI(Debug)

  protected:
    Debug() = default;  // For RTTI
  };
}  // namespace REGoth
