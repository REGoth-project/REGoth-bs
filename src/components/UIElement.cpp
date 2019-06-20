#include "UIElement.hpp"
#include <GUI/BsCGUIWidget.h>
#include <GUI/BsGUIPanel.h>
#include <RTTI/RTTI_UIElement.hpp>
#include <exception/Throw.hpp>
#include <gui/skin_gothic.hpp>

namespace REGoth
{
  UIElement::UIElement(const bs::HSceneObject& parent, bs::HCamera camera)
      : bs::Component(parent)
  {
    setName("UIElement");

    auto guiWidget = SO()->addComponent<bs::CGUIWidget>(camera);
    guiWidget->setSkin(REGoth::GUI::createSkin_Gothic());

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

  REGOTH_DEFINE_RTTI(UIElement)

}  // namespace REGoth
