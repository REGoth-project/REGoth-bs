#include "Item.hpp"
#include "Visual.hpp"
#include <scripting/ScriptObjects.hpp>

namespace REGoth
{
  Item::Item(const bs::HSceneObject& parent, const bs::String& instance)
      : ScriptBackedBy(parent, "C_ITEM", instance)
  {
  }

  void Item::onInitialized()
  {
    ScriptBackedBy::onInitialized();

    createVisual();
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
}  // namespace REGoth
