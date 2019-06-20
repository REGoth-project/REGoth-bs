#include "skin_gothic.hpp"
#include <BsZenLib/ImportFont.hpp>
#include <BsZenLib/ImportTexture.hpp>
#include <GUI/BsGUISkin.h>
#include <GUI/BsGUITexture.h>
#include <Text/BsFont.h>
#include <original-content/VirtualFileSystem.hpp>
#include <Importer/BsImporter.h>

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
      bs::gDebug().logDebug("Loading .fnt: " + fontFile);
      if (BsZenLib::HasCachedFont(fontFile))
      {
        return BsZenLib::LoadCachedFont(fontFile);
      }
      else
      {
        return BsZenLib::ImportAndCacheFont(fontFile, REGoth::gVirtualFileSystem().getFileIndex());
      }
    }
    else
    {
      return bs::gImporter().import<bs::Font>(fontFile);
    }
  }

  bs::HGUISkin GUI::createSkin_Gothic()
  {
    if (s_SkinGothic)
    {
      // Was already created before, use the cached instance
      return s_SkinGothic;
    }

    bs::HGUISkin skin = bs::GUISkin::create();

    bs::HFont fontGothicDefault = loadFont("FONT_DEFAULT.FNT");
    bs::HFont fontGothic10Book = loadFont("FONT_10_BOOK.FNT");
    bs::HFont fontGothic20Book = loadFont("FONT_20_BOOK.FNT");
    bs::HFont fontGothicOld10White = loadFont("FONT_OLD_10_WHITE.FNT");
    bs::HFont fontGothicOld10WhiteHi = loadFont("FONT_OLD_10_WHITE_HI.FNT");
    bs::HFont fontGothicOld20White = loadFont("FONT_OLD_20_WHITE.FNT");
    bs::HFont fontGothicOld20WhiteHi = loadFont("FONT_OLD_20_WHITE_HI.FNT");

    bs::GUIElementStyle baseStyle;
    baseStyle.font = fontGothicDefault;
    baseStyle.fontSize = 17;
    baseStyle.wordWrap = true;
    baseStyle.normal.textColor = bs::Color::White;
    skin->setStyle("Label", baseStyle);

    bs::GUIElementStyle labelDefault = baseStyle;
    labelDefault.font = fontGothicOld10White;
    labelDefault.fontSize = 17;
    skin->setStyle("GothicLabel", labelDefault);

    bs::GUIElementStyle labelLarge = baseStyle;
    labelLarge.font = fontGothicOld20White;
    labelLarge.fontSize = 36;
    skin->setStyle("GothicLabelLarge", labelLarge);

    bs::GUIElementStyle labelDefaultHighlighted = baseStyle;
    labelDefaultHighlighted.font = fontGothicOld10WhiteHi;
    labelDefaultHighlighted.fontSize = 17;
    skin->setStyle("GothicLabelHighlighted", labelDefaultHighlighted);

    bs::GUIElementStyle labelLargeHighlighted = baseStyle;
    labelLargeHighlighted.font = fontGothicOld20WhiteHi;
    labelLargeHighlighted.fontSize = 36;
    skin->setStyle("GothicLabelLargeHighlighted", labelLargeHighlighted);

    bs::GUIElementStyle labelObjectFocus = baseStyle;
    labelObjectFocus.font = fontGothicOld10WhiteHi;
    labelObjectFocus.fontSize = 17;
    labelObjectFocus.wordWrap = false;
    // labelObjectFocus.textHorzAlign = bs::TextHorzAlign::THA_Center;
    // labelObjectFocus.textVertAlign = bs::TextVertAlign::TVA_Center;
    skin->setStyle("GothicLabelObjectFocus", labelObjectFocus);

    // Cache the skin for later use
    s_SkinGothic = skin;

    return skin;
  }
}
