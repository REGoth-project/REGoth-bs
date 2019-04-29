#pragma once

#include <RTTI/RTTIUtil.hpp>
#include <visual/ShadowSampler.hpp>

#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class ShadowSampler;

  class ShadowSampling : public bs::Component
  {
  public:
    ShadowSampling(const bs::HSceneObject& parent);

    virtual void onInitialized() override;

    virtual void update() override;

    virtual void onTransformChanged(bs::TransformChangedFlags flags) override;

  private:
    void updateSamplerFromAncestry();

    bs::SPtr<ShadowSampler> mSampler;
    bool mNeedsUpdate;

  public:
    REGOTH_DECLARE_RTTI(ShadowSampling);

  private:
    ShadowSampling();  // For RTTI
  };

  using HShadowSampling = bs::GameObjectHandle<ShadowSampling>;
}  // namespace REGoth