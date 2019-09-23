#pragma once
#include "UIElement.hpp"
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class Focusable;
  using HFocusable = bs::GameObjectHandle<Focusable>;

  /**
   * This class handles the label drawn above objects in Focus. For example,
   * if you stand close to an item laying on the floor, the name of the item
   * is displayed above.
   *
   * Using the function `putTextAbove()` you can give a handle `Focusable`-component
   * and a string to the class. This focusable is then registered as *target point*.
   * On update, the position of the focusable is projected onto the screen and GUI-Label
   * is moved to those projected coordinates.
   *
   * The focusable can also define a Y-Offset so the label can be drawn *above* the objects.
   */
  class UIFocusText : public UIElement
  {
  public:
    UIFocusText(const bs::HSceneObject& parent, HUIElement parentUiElement);
    virtual ~UIFocusText();

    /**
     * Displays the given text above the given scene object.
     */
    void putTextAbove(HFocusable focusable);

  protected:
    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;

  private:
    /**
     * Once the target is out of range or behind the camera, the label should not
     * be visible. If the target is behind the camera, it would still end up projecting
     * it's coordinates onto the screen which would look weird.
     */
    bool shouldLabelBeVisible() const;

    /**
     * @return Whether the target position is in front of the camera. If the target
     *         is behind, this returns false.
     */
    bool isTargetInFrontOfCamera() const;

    /**
     * Moves the label so that it is displayed over the 3D-Target coordinates.
     */
    void moveLabelToProjectedTargetPos();

    /**
     * Calculates where the label should be moved to in 2D UI coordinates to be over
     * the focused object.
     */
    bs::Vector2I calculateLabelScreenPosition() const;

    /**
     * @return Where the label should be in world 3D-coordinates
     */
    bs::Vector3 labelWorldPosition() const;

    /**
     * This will query the focusable-component of the focused object for the offset to
     * the Y axis which will place the label above the object.
     *
     * If there is no object in focus or its height cannot be calculated, 0 is returned.
     */
    float getLabelYOffset() const;

    bs::GUILabel* mLabelAboveObject;

    /** The Scene-Object to draw the text above (if valid) */
    HFocusable mFocusedObject;

  public:
    REGOTH_DECLARE_RTTI(UIFocusText)

  protected:
    UIFocusText() = default;  // For RTTI
  };
}  // namespace REGoth
