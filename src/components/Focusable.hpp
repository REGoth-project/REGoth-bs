#pragma once
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class Focusable;
  using HFocusable = bs::GameObjectHandle<Focusable>;

  /**
   * This component marks the scene object it is attached to as *Focusable*.
   *
   * Think of standing near an item laying on the ground, then you would see
   * the name of the item pop up as text on screen and the item would start
   * blinking in the original game. This is the kind of *Focus* we're talking
   * about here.
   */
  class Focusable : public bs::Component
  {
  public:
    Focusable(const bs::HSceneObject& parent);
    virtual ~Focusable();

    /**
     * Sets the text to be displayed above this object.
     */
    void setText(const bs::String& text);

    /**
     * @see setText()
     */
    bs::String getText() const;

    /**
     * Set the height offset from the center of the object to where the text
     * should be displayed. By default the top of the renderables bounding box will
     * be used if possible. For more complex objects you may need to set this manually.
     *
     * Also note that the renderable is only queried at component creation. It its bounding box
     * changes afterwards, you will need to set the height here manually too.
     */
    void setTextHeight(float height);

    /**
     * @see setTextHeight()
     */
    float getTextHeight() const;

    /**
     * @see setTextHeight
     */
    float tryExtractHeightFromRenderable() const;

  private:

  public:
    REGOTH_DECLARE_RTTI(Focusable)

    bs::String mText;
    float mHeightOffset = 0.0f;

  protected:
    Focusable() = default;  // For RTTI
  };
}  // namespace REGoth
