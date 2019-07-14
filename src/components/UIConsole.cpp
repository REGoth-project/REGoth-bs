#include "UIConsole.hpp"
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUIPanel.h>
#include <GUI/BsGUITexture.h>
#include <RTTI/RTTI_UIConsole.hpp>
#include <original-content/OriginalGameResources.hpp>

namespace REGoth
{
  UIConsole::UIConsole(const bs::HSceneObject& parent, HUIElement parentUiElement)
      : UIElement(parent, parentUiElement, new bs::GUIPanel())
  {
    setName("UIConsole");

    mBackgroundBox = layout().addNewElement<bs::GUITexture>("GothicDialogueBoxBackground");

    mText = layout().addNewElement<bs::GUILabel>(bs::HString("Hello World!"));
  }

  UIConsole::~UIConsole()
  {
  }

  REGOTH_DEFINE_RTTI(UIConsole)
}  // namespace REGoth
