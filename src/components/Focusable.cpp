#include "Focusable.hpp"
#include <Components/BsCRenderable.h>
#include <RTTI/RTTI_Focusable.hpp>

namespace REGoth
{
  Focusable::Focusable(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("Focusable");

    tryExtractHeightFromRenderable();
  }

  Focusable::~Focusable()
  {
  }

  void Focusable::setText(const bs::String& text)
  {
    mText = text;
  }

  bs::String Focusable::getText() const
  {
    return mText;
  }

  void Focusable::setTextHeight(float height)
  {
    mHeightOffset = height;
  }

  float Focusable::getTextHeight() const
  {
    return mHeightOffset;
  }

  float Focusable::tryExtractHeightFromRenderable() const
  {
    auto renderable = SO()->getComponent<bs::CRenderable>();

    if (!renderable) return 0.0;

    return renderable->getBounds().getBox().getSize().y / 2.0f;
  }

  REGOTH_DEFINE_RTTI(Focusable)

}  // namespace REGoth
