#pragma once

#include "RTTIUtil.hpp"
#include <components/GameUI.hpp>

namespace REGoth
{
  class RTTI_GameUI
      : public bs::RTTIType<GameUI, UIElement, RTTI_GameUI>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_GameUI()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(GameUI)
  };

}  // namespace REGoth
