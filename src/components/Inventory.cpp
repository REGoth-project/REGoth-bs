#include "Inventory.hpp"
#include <RTTI/RTTI_Inventory.hpp>

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

    return mInventory.find(instance) != mInventory.end();
  }

  bs::UINT32 Inventory::itemCount(const bs::String& instance) const
  {
    throwIfNotUpperCase(instance);

    auto it = mInventory.find(instance);

    if (it == mInventory.end()) return 0;

    return it->second;
  }

  void Inventory::giveItem(const bs::String& instance, bs::UINT32 count)
  {
    throwIfNotUpperCase(instance);

    mInventory[instance] += 1;
  }

  void Inventory::removeItem(const bs::String& instance, bs::UINT32 count)
  {
    throwIfNotUpperCase(instance);

    auto it = mInventory.find(instance);

    if (it == mInventory.end())
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
      mInventory.erase(it);
    }
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
