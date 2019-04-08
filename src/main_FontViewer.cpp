#include "BsFPSCamera.h"
#include "REGothEngine.hpp"
#include <BsZenLib/ImportFont.hpp>
#include <BsZenLib/ImportTexture.hpp>
#include <Components/BsCCamera.h>
#include <GUI/BsCGUIWidget.h>
#include <Image/BsSpriteTexture.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUIPanel.h>
#include <GUI/BsGUISkin.h>
#include <GUI/BsGUITexture.h>
#include <Resources/BsBuiltinResources.h>
#include <Scene/BsSceneObject.h>
#include <Text/BsFont.h>
#include <original-content/VirtualFileSystem.hpp>

class REGothFontViewer : public REGoth::REGothEngine
{
public:
  void setupMainCamera() override
  {
    REGoth::REGothEngine::setupMainCamera();

    mFPSCamera = mMainCamera->SO()->addComponent<bs::FPSCamera>();
  }

  void setupScene() override
  {
    const char* FONT = "FONT_DEFAULT.FNT";

    bs::HFont font;
    if (BsZenLib::HasCachedFont(FONT))
    {
      font = BsZenLib::LoadCachedFont(FONT);
    }
    else
    {
      font = BsZenLib::ImportAndCacheFont(FONT, REGoth::gVirtualFileSystem().getFileIndex());
    }

    if (!font)
    {
      bs::gDebug().logError("Failed to load Font: " + bs::String(FONT));
      exit(-1);
    }

    const char* BACKGROUND = "STARTSCREEN.TGA";

    bs::HTexture backgroundTexture =
        BsZenLib::ImportAndCacheTexture(BACKGROUND, REGoth::gVirtualFileSystem().getFileIndex());

    if (!backgroundTexture)
    {
      bs::gDebug().logError("Failed to load BackgroundTexture: " + bs::String(BACKGROUND));
      exit(-1);
    }

    bs::HSpriteTexture backgroundSprite = bs::SpriteTexture::create(backgroundTexture);

    // Add GUI
    bs::HSceneObject guiSO = bs::SceneObject::create("GUI");
    bs::HGUIWidget gui = guiSO->addComponent<bs::CGUIWidget>(mMainCamera);

    bs::HGUISkin skin = bs::GUISkin::create();

    bs::GUIElementStyle gothicLabelStyle;
    gothicLabelStyle.font = font;
    gothicLabelStyle.fontSize = 18;

    skin->setStyle("GothicLabelStyle", gothicLabelStyle);

    // gui->setSkin(bs::BuiltinResources::instance().getGUISkin());
    gui->setSkin(skin);

    bs::GUIPanel* mainPanel = gui->getPanel();
    bs::GUIPanel* backgroundPanel = mainPanel->addNewElement<bs::GUIPanel>(1);

    bs::GUITexture* background = backgroundPanel->addNewElement<bs::GUITexture>(backgroundSprite);
    background->setPosition(0,0);
    // background->setSize(backgroundPanel->getScreenBounds().x, backgroundPanel->getScreenBounds().y);

    const char* TEXT =
        "This is some bullshit text for testing. Does this thing handle newlines?\n"
        "I guess we'll see. What about umlauts like ä, ö, ü and ß? UPPERCASE ÄÖÜ?\n"
        "Do special characters work? Something like %, #, / or _?";

    bs::GUILabel* labelDefaultFont = mainPanel->addNewElement<bs::GUILabel>(bs::HString(TEXT));

    bs::GUILabel* labelGothicFont = mainPanel->addNewElement<bs::GUILabel>(bs::HString(TEXT));
    labelGothicFont->setStyle("GothicLabelStyle");

    labelDefaultFont->setPosition(10, 20);
    labelGothicFont->setPosition(10, 100);
  }

protected:
  bs::HFPSCamera mFPSCamera;
};

int main(int argc, char** argv)
{
  REGothFontViewer regoth;

  return REGoth::main(regoth, argc, argv);
}
