#pragma once

#include "RTTIUtil.hpp"
#include <components/GameplayUI.hpp>

namespace REGoth
{
  class RTTI_GameplayUI
      : public bs::RTTIType<GameplayUI, UIElement, RTTI_GameplayUI>
  {
    BS_BEGIN_RTTI_MEMBERS
    // This class should not be serialized
    BS_END_RTTI_MEMBERS

  public:
    RTTI_GameplayUI()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(GameplayUI)
  };

}  // namespace REGoth
