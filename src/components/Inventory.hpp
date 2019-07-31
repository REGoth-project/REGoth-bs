#pragma once
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>
#include <Utility/BsEvent.h>

namespace REGoth
{
  class Inventory;
  using HInventory = bs::GameObjectHandle<Inventory>;

  /**
   * This component represents a characters inventory.
   *
   * When an item is laying on the ground, it will have a script-object attached
   * to it. This script object stores the name among some other properties about
   * the item, however, it seems like it no item in the original games changes
   * any of those properties after the item was created. While storing the
   * instance name only would be enough from that point of view, the script
   * object does contain some information we will need later, like the visual
   * the item is rendered with.
   *
   * Therefore, we might need to create a script object for at least one
   * of the items when rendering the inventorys UI.
   */
  class Inventory : public bs::Component
  {
  public:
    Inventory(const bs::HSceneObject& parent);
    virtual ~Inventory();

    /**
     * Checks whether at least one item of the given instance is inside the inventory.
     *
     * @param  instance  UPPERCASE Script instance name of the item to check.
     *
     * @return Whether there is at least one item of the given instance in the inventory.
     */
    bool hasItem(const bs::String& instance) const;

    /**
     * @param  instance  UPPERCASE Script instance name of the item to look for.
     *
     * @return How many items of the given instance are inside the inventory. Returns 0
     *         if there are none.
     */
    bs::UINT32 itemCount(const bs::String& instance) const;

    /**
     * Creates items of the given instance and adds them to the inventory. Multiple items
     * can be added at once by modifying the `count` parameter.
     *
     * @param  instance  UPPERCASE Script instance name of the item to create.
     * @param  count     (Optional) How many instances of the item to add to the inventory.
     */
    void giveItem(const bs::String& instance, bs::UINT32 count = 1);

    /**
     * Removes items of the given instance from the inventory. Multiple items can be removed
     * at once by modifying the `count` parameter.
     *
     * Throws if no such item exists, so you should check that first.
     * Throws if not enough items exist.
     *
     * @param  instance  UPPERCASE Script instance name of the item to remove.
     * @param  count     (Optional) How many instances of the item to remove from the inventory.
     */
    void removeItem(const bs::String& instance, bs::UINT32 count = 1);

    /**
     * @return All item instances held by this inventory. Does not contain any
     *         count information. All instances of which there is at least one
     *         item of in the inventory will be included.
     */
    const bs::Map<bs::String, bs::UINT32>& allItems() const;

    using OnItemCallback = void(const bs::String& instance);
 
    /** Events triggered when items got added or removed */
    bs::Event<OnItemCallback> OnItemChanged;

  private:

    void throwIfNotUpperCase(const bs::String& instance) const;

    /**
     * Map of Item Instance to how many of those we have in this inventory.
     * There should not be any instance with an amount of 0 in here, so if you
     * just want to know whether there is at least one instance of an item in
     * this inventory, it's enough to check whether such a key exists.
     */
    bs::Map<bs::String, bs::UINT32> mItemCountsByInstance;

  public:
    REGOTH_DECLARE_RTTI(Inventory)

  protected:
    Inventory() = default;  // For RTTI
  };
}  // namespace REGoth
