#pragma once

#include <RTTI/RTTIUtil.hpp>

#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class MaterialModifier;

  class MeshVisual : public bs::Component
  {
  public:
    MeshVisual(const bs::HSceneObject& parent);

    virtual void onInitialized() override;

    virtual void onDestroyed() override;

    void addModifier(const bs::SPtr<MaterialModifier>& modifier);

    void setMesh(bs::HMesh mesh);

    void setMaterials(const bs::Vector<bs::HMaterial>& materials);

    inline bs::HRenderable renderable()
    {
      return mRenderable;
    }

  private:
    void applyMaterialModifiers();

    bs::HRenderable mRenderable;

    bs::HMesh mOriginalMesh;

    bs::Vector<bs::HMaterial> mOriginalMaterials;

    bs::Vector<bs::SPtr<MaterialModifier>> mModifiers;

  public:
    REGOTH_DECLARE_RTTI(MeshVisual)

  private:
    MeshVisual() = default;
  };

  using HMeshVisual = bs::GameObjectHandle<MeshVisual>;
}  // namespace REGoth