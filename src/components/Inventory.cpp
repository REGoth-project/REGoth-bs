#include "Inventory.hpp"
#include <RTTI/RTTI_Inventory.hpp>
#include <log/logging.hpp>

namespace REGoth
{
  Inventory::Inventory(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("Inventory");
  }

  Inventory::~Inventory()
  {
  }

  bool Inventory::hasItem(const bs::String& instance) const
  {
    throwIfNotUpperCase(instance);

    return mItemCountsByInstance.find(instance) != mItemCountsByInstance.end();
  }

  bs::UINT32 Inventory::itemCount(const bs::String& instance) const
  {
    throwIfNotUpperCase(instance);

    auto it = mItemCountsByInstance.find(instance);

    if (it == mItemCountsByInstance.end()) return 0;

    return it->second;
  }

  void Inventory::giveItem(const bs::String& instance, bs::UINT32 count)
  {
    throwIfNotUpperCase(instance);

    REGOTH_LOG(Info, Uncategorized, "[Inventory] Add {0}x item {1} to Inventory of {2}", count,
               instance, SO()->getName());

    mItemCountsByInstance[instance] += 1;

    OnItemChanged(instance);
  }

  void Inventory::removeItem(const bs::String& instance, bs::UINT32 count)
  {
    throwIfNotUpperCase(instance);

    auto it = mItemCountsByInstance.find(instance);

    if (it == mItemCountsByInstance.end())
    {
      REGOTH_THROW(InvalidParametersException,
                   bs::StringUtil::format("Trying to remove items of instance {1}, but there "
                                          "are none of those in this inventory!",
                                          instance));
    }

    if (it->second < count)
    {
      REGOTH_THROW(InvalidParametersException,
                   bs::StringUtil::format("Trying to remove {0} items of instance {1}, but there "
                                          "are only {2} instances of those in this inventory!",
                                          count, instance, it->second));
    }

    it->second -= count;

    if (it->second == 0)
    {
      mItemCountsByInstance.erase(it);
    }

    OnItemChanged(instance);
  }

  const bs::Map<bs::String, bs::UINT32>& Inventory::allItems() const
  {
    return mItemCountsByInstance;
  }

  HInventory Inventory::thisHandle() const
  {
    return bs::static_object_cast<Inventory>(getHandle());
  }

  void Inventory::throwIfNotUpperCase(const bs::String& instance) const
  {
    bs::String upper = instance;
    bs::StringUtil::toUpperCase(upper);

    if (upper != instance)
    {
      REGOTH_THROW(
          InvalidParametersException,
          bs::StringUtil::format("Instance {0} should be UPPERCASE, but it's not! (Should be: {1})",
                                 instance, upper));
    }
  }

  REGOTH_DEFINE_RTTI(Inventory)

}  // namespace REGoth
