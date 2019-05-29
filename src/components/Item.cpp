#include "Item.hpp"
#include "Visual.hpp"
#include <RTTI/RTTI_Item.hpp>
#include <scripting/ScriptObject.hpp>

namespace REGoth
{
  Item::Item(const bs::HSceneObject& parent, const bs::String& instance, HGameWorld gameWorld)
      : ScriptBackedBy(parent, "C_ITEM", instance, gameWorld)
  {
    setName("Item");
  }

  void Item::onInitialized()
  {
    bool isNewScriptObject = !hasInstantiatedScriptObject();

    ScriptBackedBy::onInitialized();

    // When loading a world from a saved prefab, the visual will already have been created.
    if (isNewScriptObject)
    {
      createVisual();
    }
  }

  void Item::onDestroyed()
  {
    ScriptBackedBy::onDestroyed();
  }

  void Item::createVisual()
  {
    bs::String visual = scriptObjectData().stringValue("VISUAL");
    Visual::addToSceneObject(SO(), visual);
  }

  REGOTH_DEFINE_RTTI(Item)
}  // namespace REGoth
