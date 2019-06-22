#pragma once
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class UIElement;
  using HUIElement = bs::GameObjectHandle<UIElement>;

  /**
   * This component serves as a root for a set of UI-Elements. It also
   * does offers to create a tree-like structure of furhter UI-Elements.
   *
   * For example, the hud would have a single `UIElement` as a
   * root-note. Its scene-object would then have two children, one with
   * a `UIManaBar` and one with a `UIHealthBar` component attached.
   *
   * The root `UIElement` gives the `bs::GUIPanel` for the other components to
   * draw on. Components can also decide to register their own sub-layout.
   */
  class UIElement : public bs::Component
  {
  public:
    /**
     * Constructs a new root UIElement.
     */
    UIElement(const bs::HSceneObject& parent, bs::HCamera camera);

    virtual ~UIElement();

    /**
     * Makes this UI-Element and all children visible.
     *
     * To hide this UI-Element, see hide()
     */
    void show();

    /**
     * Makes this UI-Element and all children invisible.
     *
     * To show this UI-Element again, see show()
     */
    void hide();

  protected:
    /**
     * Adds an UIElement as child to this one.
     *
     * @tparam  T     Type of UIElement to add.
     * @tparam  ARGS  Constructor arguments to the UIElement after the `parentUiElement`.
     *
     * @param   soName  Name of the scene object the new UIElement will be attached to.
     *
     * @return  The newly created UIElement.
     */
    template <class T, typename... ARGS>
    bs::GameObjectHandle<T> addChildElement(const bs::String soName, ARGS&&... args)
    {
      static_assert((std::is_base_of<UIElement, T>::value),
                    "Specified type is not a valid UIElement.");

      auto so         = addChildSceneObject(soName);

      auto thisHandle = bs::static_object_cast<UIElement>(getHandle());

      return so->addComponent<T>(thisHandle, std::forward<ARGS>(args)...);
    }

    /**
     * Adds a scene object as child to the scene object holding this component. (Sets
     * the parent of the newly created SO to the SO of this component.)
     *
     * This is needed for the addChildElement() method so we don't need to include
     * `BsSceneObject.h` in this header to create the scene object.
     */
    bs::HSceneObject addChildSceneObject(const bs::String& name);

    /**
     * Constructs a new UIElement with an existing UIElement as parent.
     * The `parentUiElement` should be attached to the parent of this scene object.
     *
     * Use this to create a tree-like structure for your UI.
     *
     * @param  parentUiElement  Parent-UIElement this should be drawn in.
     * @param  pLayout          Layout of this new UI-Element. Should be created via
     *                          `new GUILayout()` or the type of layout you prefer.
     *                          The layout will be added to the parents layout.
     */
    UIElement(const bs::HSceneObject& parent, HUIElement parentUiElement, bs::GUILayout* layout);

    /**
     * Access to this components default gui components
     */
    bs::GUILayout& layout() const;
    bs::GUILayout& parentLayout() const;
    bs::Camera& camera() const;

    /**
     * Loads a texture into a sprite.
     *
     * @param  texture  Texture name as in the original game, e.g. `STONE.TGA`.
     * @return Sprite with the given texture. Empty handle if loading failed.
     */
    bs::HSpriteTexture loadSprite(const bs::String& texture);

  private:
    bs::GUILayout* mGuiLayout;
    HUIElement mParentUiElement;

  public:
    REGOTH_DECLARE_RTTI(UIElement)

  protected:
    UIElement() = default;  // For RTTI
  };
}  // namespace REGoth
