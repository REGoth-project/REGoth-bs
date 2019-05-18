#pragma once

#include "RTTIUtil.hpp"
#include <components/MeshVisual.hpp>

using namespace bs;

namespace REGoth
{
  class RTTI_MeshVisual : public bs::RTTIType<MeshVisual, bs::Component, RTTI_MeshVisual>
  {
    BS_BEGIN_RTTI_MEMBERS
    BS_RTTI_MEMBER_REFL(mRenderable, 0)
    BS_RTTI_MEMBER_REFL(mOriginalMesh, 1)
    BS_RTTI_MEMBER_REFL_ARRAY(mOriginalMaterials, 2)
    BS_RTTI_MEMBER_REFLPTR_ARRAY(mModifiers, 3)
    BS_END_RTTI_MEMBERS

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(MeshVisual)
  };

}  // namespace REGoth
