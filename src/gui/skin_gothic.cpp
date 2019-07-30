#include "skin_gothic.hpp"
#include <BsZenLib/ImportFont.hpp>
#include <BsZenLib/ImportTexture.hpp>
#include <GUI/BsGUISkin.h>
#include <GUI/BsGUITexture.h>
#include <Importer/BsImporter.h>
#include <Resources/BsBuiltinResources.h>
#include <Text/BsFont.h>
#include <original-content/OriginalGameResources.hpp>

namespace REGoth
{
  static bs::HGUISkin s_SkinGothic;
  /**
   * Loads a font from gothics original font format or a vector font format like ttf.
   */
  static bs::HFont loadFont(const bs::String& fontFile)
  {
    const bool lowercase = true;

    if (bs::StringUtil::endsWith(fontFile, ".fnt", lowercase))
    {
      return gOriginalGameResources().font(fontFile);
    }
    else
    {
      return bs::gImporter().import<bs::Font>(fontFile);
    }
  }

  static bs::HGUISkin cloneGuiSkin(bs::HGUISkin skin)
  {
    auto styleNames = skin->getStyleNames();
    auto newSkin    = bs::GUISkin::create();

    for (const bs::String& n : styleNames)
    {
      newSkin->setStyle(n, *skin->getStyle(n));
    }

    return newSkin;
  }

  bs::HGUISkin GUI::getGothicStyleSkin()
  {
    if (s_SkinGothic)
    {
      // Was already created before, use the cached instance
      return s_SkinGothic;
    }

    bs::HGUISkin skin = cloneGuiSkin(bs::gBuiltinResources().getGUISkin());

    bs::HFont fontGothicDefault      = loadFont("FONT_DEFAULT.FNT");
    bs::HFont fontGothic10Book       = loadFont("FONT_10_BOOK.FNT");
    bs::HFont fontGothic20Book       = loadFont("FONT_20_BOOK.FNT");
    bs::HFont fontGothicOld10White   = loadFont("FONT_OLD_10_WHITE.FNT");
    bs::HFont fontGothicOld10WhiteHi = loadFont("FONT_OLD_10_WHITE_HI.FNT");
    bs::HFont fontGothicOld20White   = loadFont("FONT_OLD_20_WHITE.FNT");
    bs::HFont fontGothicOld20WhiteHi = loadFont("FONT_OLD_20_WHITE_HI.FNT");

    bs::GUIElementStyle baseStyle;
    baseStyle.font             = fontGothicDefault;
    baseStyle.fontSize         = 17;
    baseStyle.wordWrap         = true;
    baseStyle.normal.textColor = bs::Color::White;
    skin->setStyle("Label", baseStyle);

    bs::GUIElementStyle labelDefault = baseStyle;
    labelDefault.font                = fontGothicOld10White;
    labelDefault.fontSize            = 17;
    skin->setStyle("GothicLabel", labelDefault);

    bs::GUIElementStyle labelLarge = baseStyle;
    labelLarge.font                = fontGothicOld20White;
    labelLarge.fontSize            = 36;
    skin->setStyle("GothicLabelLarge", labelLarge);

    bs::GUIElementStyle labelDefaultHighlighted = baseStyle;
    labelDefaultHighlighted.font                = fontGothicOld10WhiteHi;
    labelDefaultHighlighted.fontSize            = 17;
    skin->setStyle("GothicLabelHighlighted", labelDefaultHighlighted);

    bs::GUIElementStyle labelLargeHighlighted = baseStyle;
    labelLargeHighlighted.font                = fontGothicOld20WhiteHi;
    labelLargeHighlighted.fontSize            = 36;
    skin->setStyle("GothicLabelLargeHighlighted", labelLargeHighlighted);

    bs::GUIElementStyle labelObjectFocus = baseStyle;
    labelObjectFocus.font                = fontGothicOld10WhiteHi;
    labelObjectFocus.fontSize            = 17;
    labelObjectFocus.wordWrap            = false;
    // labelObjectFocus.textHorzAlign = bs::TextHorzAlign::THA_Center;
    // labelObjectFocus.textVertAlign = bs::TextVertAlign::TVA_Center;
    skin->setStyle("GothicLabelObjectFocus", labelObjectFocus);

    bs::GUIElementStyle subtitleBox = baseStyle;
    subtitleBox.normal.texture      = gOriginalGameResources().sprite("DLG_CHOICE.TGA");
    skin->setStyle("GothicDialogueBoxBackground", subtitleBox);

    skin->setStyle("GothicSubtitleBoxCharacterName", labelDefaultHighlighted);
    skin->setStyle("GothicSubtitleBoxText", labelDefault);

    auto button     = *skin->getStyle("Button");

    // Commented out: That font is hard to read
    // button.font     = fontGothicOld10WhiteHi;
    // button.fontSize = 17;
    skin->setStyle("Button", button);

    // Cache the skin for later use
    s_SkinGothic = skin;

    return skin;
  }
}  // namespace REGoth
