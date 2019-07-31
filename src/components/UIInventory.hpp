#pragma once
#include "UIElement.hpp"
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class Inventory;
  using HInventory = bs::GameObjectHandle<Inventory>;

  /**
   * TODO: Documentation of UIInventory
   */
  class UIInventory : public UIElement
  {
  public:
    UIInventory(const bs::HSceneObject& parent, HUIElement parentUiElement);
    virtual ~UIInventory();

    /**
     * Set the inventory to be displayed by this UI component.
     */
    void setViewedInventory(HInventory inventory);

  protected:
    struct Item
    {
      bs::String name;
      bs::UINT32 count      = 0;
      bs::GUIButton* button = nullptr;
    };

    /**
     * Clears the complete list.
     *
     * Does not remove the items from the inventory, just from the UI
     */
    void removeAll();

    /**
     * Removes all items from the UI and regenerates them from the viewed inventory.
     */
    void forceUpdateAll();

    /**
     * Called when an item in the viewed inventory got updated.
     */
    void onInventoryItemUpdated(const bs::String& instance);

    /**
     * @return Whether at least one item of the given instance is currently being
     *         shown in the UI.
     */
    bool hasItem(const bs::String& instance) const;

    /**
     * @return Count of the given item instance currently shown in the UI.
     */
    bs::UINT32 itemCount(const bs::String& instance) const;

    /**
     * Adds a single item to the list.
     *
     * @param  instance  Script instance of the item. (Unique identifier)
     * @param  name      User-facing name of the item.
     * @oaram  count     How many of this item to add to the list.
     */
    void addItem(const bs::String& instance, const bs::String& name, bs::UINT32 count = 1);

    /**
     * Removes the given item from the list n times.
     *
     * @param  instance  Script instance of the item to remove.
     * @param  count     How many of this item should be removed. If the count
     *                   is higher than the count of items available, all of them are removed.
     */
    void removeItem(const bs::String& instance, bs::UINT32 count = 1);

    /**
     * Updates the gui thumbnail of the given item instance with a new count, image, etc.
     */
    void updateItemThumbnail(Item& item);
    void createUIElementsForItem(Item& item);
    void removeUIElementsFromItem(Item& item);
    void removeItemFromList(const bs::String& instance);
    bs::Vector<bs::String> allInstancesInInventory() const;

    /** Triggered once per frame. Allows the component to handle input and move. */
    void update() override;

  private:
    bs::GUIScrollArea* mScrollArea;

    bs::Map<const bs::String, Item> mItemsByInstance;

    /**
     * The inventory being viewed through this UI component.
     */
    HInventory mViewedInventory;
    bs::HEvent mRegisteredOnItemChangedEvent;

  public:
    REGOTH_DECLARE_RTTI(UIInventory)

  protected:
    UIInventory() = default;  // For RTTI
  };
}  // namespace REGoth
