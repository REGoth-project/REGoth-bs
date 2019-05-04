#pragma once

#include "RTTIUtil.hpp"
#include <components/NodeVisuals.hpp>

namespace REGoth
{
  class RTTI_NodeVisuals : public bs::RTTIType<NodeVisuals, bs::Component, RTTI_NodeVisuals>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

  public:
    RTTI_NodeVisuals()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(NodeVisuals)
  };

}  // namespace REGoth
