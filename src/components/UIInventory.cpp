#include "UIInventory.hpp"
#include <GUI/BsGUIButton.h>
#include <GUI/BsGUILabel.h>
#include <GUI/BsGUILayoutY.h>
#include <GUI/BsGUIPanel.h>
#include <GUI/BsGUIScrollArea.h>
#include <RTTI/RTTI_UIInventory.hpp>
#include <components/Inventory.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>

namespace REGoth
{
  UIInventory::UIInventory(const bs::HSceneObject& parent, HUIElement parentUiElement)
      : UIElement(parent, parentUiElement, new bs::GUIPanel())
  {
    setName("UIInventory");

    mScrollArea = layout().addNewElement<bs::GUIScrollArea>(bs::ScrollBarType::ShowIfDoesntFit,
                                                            bs::ScrollBarType::NeverShow);
  }

  UIInventory::~UIInventory()
  {
  }

  void UIInventory::setViewedInventory(HInventory inventory)
  {
    mViewedInventory = inventory;

    if (mRegisteredOnItemChangedEvent)
    {
      mRegisteredOnItemChangedEvent.disconnect();
    }

    if (mViewedInventory)
    {
      mRegisteredOnItemChangedEvent =
          mViewedInventory->OnItemChanged.connect([this](const bs::String& instance) {
            // Get notified when the viewed inventory changes
            onInventoryItemUpdated(instance);
          });

      forceUpdateAll();
    }
  }

  void UIInventory::onInventoryItemUpdated(const bs::String& instance)
  {
    bs::UINT32 countUI        = itemCount(instance);
    bs::UINT32 countInventory = mViewedInventory->itemCount(instance);

    if (countUI < countInventory)
    {
      addItem(instance, instance, countInventory - countUI);
    }
    else if (countUI > countInventory)
    {
      removeItem(instance, countUI - countInventory);
    }
    else
    {
      // Count is the same, do nothing
    }
  }

  void UIInventory::update()
  {
    UIElement::update();

    bs::Rect2I bounds = parentLayout().getBounds();

    bounds.y      = 0;
    bounds.height = parentLayout().getBounds().height;
    bounds.width  = 180;

    layout().setBounds(bounds);
    mScrollArea->setWidth(layout().getBounds().width);
    mScrollArea->setHeight(layout().getBounds().height);
  }

  void UIInventory::removeAll()
  {
    for (const auto& instance : allInstancesInInventory())
    {
      removeUIElementsFromItem(mItemsByInstance[instance]);
    }

    mItemsByInstance.clear();
  }

  void UIInventory::forceUpdateAll()
  {
    removeAll();

    for (const auto& p : mViewedInventory->allItems())
    {
      onInventoryItemUpdated(p.first);
    }
  }

  bs::Vector<bs::String> UIInventory::allInstancesInInventory() const
  {
    bs::Vector<bs::String> instances;

    instances.reserve(mItemsByInstance.size());

    for (const auto& p : mItemsByInstance)
    {
      instances.push_back(p.first);
    }

    return instances;
  }

  bool UIInventory::hasItem(const bs::String& instance) const
  {
    return itemCount(instance) != 0;
  }

  bs::UINT32 UIInventory::itemCount(const bs::String& instance) const
  {
    auto it = mItemsByInstance.find(instance);

    if (it == mItemsByInstance.end()) return 0;

    return it->second.count;
  }

  void UIInventory::addItem(const bs::String& instance, const bs::String& name, bs::UINT32 count)
  {
    auto it = mItemsByInstance.find(instance);

    if (count == 0)
    {
      REGOTH_THROW(InvalidParametersException, "Count cannot be 0");
    }

    if (it == mItemsByInstance.end())
    {
      auto& item = mItemsByInstance[instance];

      item.count = count;
      item.name  = name;

      createUIElementsForItem(item);

      updateItemThumbnail(item);
    }
    else
    {
      auto& item = it->second;

      item.count += count;

      updateItemThumbnail(item);
    }
  }

  void UIInventory::removeItem(const bs::String& instance, bs::UINT32 count)
  {
    auto it = mItemsByInstance.find(instance);

    if (it == mItemsByInstance.end())
    {
      return;
    }

    auto& item = it->second;

    item.count -= bs::Math::min(item.count, count);

    if (item.count == 0)
    {
      removeItemFromList(instance);
    }
    else
    {
      updateItemThumbnail(item);
    }
  }

  void UIInventory::removeItemFromList(const bs::String& instance)
  {
    auto it = mItemsByInstance.find(instance);

    if (it == mItemsByInstance.end())
    {
      return;
    }

    auto& item = it->second;

    removeUIElementsFromItem(item);

    mItemsByInstance.erase(it);
  }

  void UIInventory::updateItemThumbnail(Item& item)
  {
    auto content = bs::StringUtil::format("{0} ({1})", item.name, item.count);

    item.button->setContent(bs::GUIContent(bs::HString(content)));
  }

  void UIInventory::createUIElementsForItem(Item& item)
  {
    if (item.button) return;

    item.button = mScrollArea->getLayout().addNewElement<bs::GUIButton>(bs::HString("<none>"));
  }

  void UIInventory::removeUIElementsFromItem(Item& item)
  {
    mScrollArea->getLayout().removeElement(item.button);

    item.button = nullptr;
  }

  REGOTH_DEFINE_RTTI(UIInventory)

}  // namespace REGoth
