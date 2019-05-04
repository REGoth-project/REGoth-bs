#pragma once

#include "RTTIUtil.hpp"
#include <components/VisualMorphMesh.hpp>

namespace REGoth
{
  class RTTI_VisualMorphMesh
    : public bs::RTTIType<VisualMorphMesh, bs::Component, RTTI_VisualMorphMesh>
  {
    BS_BEGIN_RTTI_MEMBERS
    // TODO: Fill RTTI Members
    BS_END_RTTI_MEMBERS

    public:
    RTTI_VisualMorphMesh()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(VisualMorphMesh)
  };

}  // namespace REGoth
