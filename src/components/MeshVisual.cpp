#include "MeshVisual.hpp"
#include <RTTI/RTTI_MeshVisual.hpp>
#include <visual/GenericMaterialModifier.hpp>

#include <Components/BsCRenderable.h>
#include <Material/BsMaterial.h>
#include <Scene/BsSceneObject.h>

#include <random>

namespace REGoth
{
  MeshVisual::MeshVisual(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
  }

  void MeshVisual::onInitialized()
  {
    // Only create the renderable if we are not being deserialized
    if (mRenderable.isDestroyed())
    {
      mRenderable = SO()->addComponent<bs::CRenderable>();
    }

    setMesh(mOriginalMesh);

    setMaterials(mOriginalMaterials);
  }

  void MeshVisual::onDestroyed()
  {
    if (!mRenderable.isDestroyed())
    {
      SO()->destroyComponent(mRenderable.get());
    }
  }

  void MeshVisual::addModifier(const bs::SPtr<MaterialModifier>& modifier)
  {
    // Remove existing modifiers
    auto it = std::find_if(std::begin(mModifiers), std::end(mModifiers),
                           [&modifier](const bs::SPtr<MaterialModifier>& other) {
                             return other->getIdentifier() == modifier->getIdentifier();
                           });

    if (it != std::end(mModifiers))
    {
      (*it) = modifier;
    }
    else
    {
      mModifiers.push_back(modifier);
    }

    applyMaterialModifiers();
  }

  void MeshVisual::setMesh(bs::HMesh mesh)
  {
    // Just pass the mesh to the renderable
    mOriginalMesh = mesh;

    mRenderable->setMesh(mOriginalMesh);
  }

  void MeshVisual::setMaterials(const bs::Vector<bs::HMaterial>& materials)
  {
    // We assign only copies of the original materials to the renderable
    // to which our modifiers are applied
    mOriginalMaterials = materials;

    bs::Vector<bs::HMaterial> clonedMaterials;
    for (auto const& material : materials)
    {
      auto clonedMaterial = material->clone();
      clonedMaterials.push_back(clonedMaterial);
    }

    mRenderable->setMaterials(clonedMaterials);

    applyMaterialModifiers();
  }

  void MeshVisual::applyMaterialModifiers()
  {
    for (auto material : mRenderable->getMaterials())
    {
      for (auto modifier : mModifiers)
      {
        modifier->apply(material);
      }
    }
  }

  REGOTH_DEFINE_RTTI(MeshVisual)
}  // namespace REGoth