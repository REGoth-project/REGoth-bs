#include "UIElement.hpp"
#include <GUI/BsCGUIWidget.h>
#include <GUI/BsGUIPanel.h>
#include <Image/BsSpriteTexture.h>
#include <RTTI/RTTI_UIElement.hpp>
#include <exception/Throw.hpp>
#include <gui/skin_gothic.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameResources.hpp>

namespace REGoth
{
  UIElement::UIElement(const bs::HSceneObject& parent, bs::HCamera camera)
      : bs::Component(parent)
  {
    setName("UIElement");

    auto guiWidget = SO()->addComponent<bs::CGUIWidget>(camera);
    guiWidget->setSkin(REGoth::GUI::getGothicStyleSkin());

    mGuiLayout = guiWidget->getPanel();
  }

  UIElement::UIElement(const bs::HSceneObject& parent, HUIElement parentUiElement,
                       bs::GUILayout* layout)
      : bs::Component(parent)
      , mParentUiElement(parentUiElement)
      , mGuiLayout(layout)
  {
    setName("UIElement");

    if (parentUiElement->SO() != parent->getParent())
    {
      REGOTH_THROW(InvalidParametersException, "Parent UIElement must be attached to parent SO");
    }

    parentLayout().addElement(mGuiLayout);
  }

  UIElement::~UIElement()
  {
  }

  void UIElement::show()
  {
    layout().setVisible(true);
  }

  void UIElement::hide()
  {
    layout().setVisible(false);
  }

  bs::HSceneObject UIElement::addChildSceneObject(const bs::String& name)
  {
    auto so = bs::SceneObject::create(name);
    so->setParent(SO());

    return so;
  }

  bs::GUILayout& UIElement::layout() const
  {
    if (!mGuiLayout)
    {
      REGOTH_THROW(InvalidStateException, "No Layout available?");
    }

    return *mGuiLayout;
  }

  bs::GUILayout& UIElement::parentLayout() const
  {
    if (!mParentUiElement)
    {
      REGOTH_THROW(InvalidStateException, "No parent available?");
    }

    return mParentUiElement->layout();
  }

  bs::Camera& UIElement::camera() const
  {
    if (!layout()._getParentWidget())
    {
      REGOTH_THROW(InvalidStateException, "No parent widget available?");
    }

    auto camera = layout()._getParentWidget()->getCamera();

    if (!camera)
    {
      REGOTH_THROW(InvalidStateException, "No camera available?");
    }

    return *camera;
  }

  bs::HSpriteTexture UIElement::loadSprite(const bs::String& texture)
  {
    bs::HTexture t = gOriginalGameResources().texture(texture);

    if (!t)
    {
      REGOTH_LOG(Warning, Uncategorized, "[UIElement] Failed to load texture: {0}", texture);

      return {};
    }

    return bs::SpriteTexture::create(t);
  }

  REGOTH_DEFINE_RTTI(UIElement)

}  // namespace REGoth
