#include "RTTIUtil.hpp"
#include <components/UIFocusText.hpp>

namespace REGoth
{
  class RTTI_UIFocusText : public bs::RTTIType<UIFocusText, UIElement, RTTI_UIFocusText>
  {
    BS_BEGIN_RTTI_MEMBERS
    // This class should not be serialized
    BS_END_RTTI_MEMBERS

  public:
    RTTI_UIFocusText()
    {
    }

    REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(UIFocusText)
  };
}  // namespace REGoth
