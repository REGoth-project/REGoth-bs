#pragma once

#include "RTTIUtil.hpp"
#include <components/VisualStaticMesh.hpp>

namespace REGoth
{
  class RTTI_VisualStaticMesh
      : public bs::RTTIType<VisualStaticMesh, bs::Component, RTTI_VisualStaticMesh>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mVisual, 0)
    BS_END_RTTI_MEMBERS

  public:
    RTTI_VisualStaticMesh()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(VisualStaticMesh)
  };

}  // namespace REGoth
