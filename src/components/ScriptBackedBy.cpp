#include "ScriptBackedBy.hpp"
#include <RTTI/RTTI_ScriptBackedBy.hpp>
#include <components/GameWorld.hpp>
#include <scripting/ScriptVMForGameWorld.hpp>

namespace REGoth
{
  ScriptBackedBy::ScriptBackedBy(const bs::HSceneObject& parent, const bs::String& className,
                                 const bs::String& instance)
      : mScriptClassName(className)
      , mScriptInstance(instance)
      , NeedsGameWorld(parent)
  {
    setName("ScriptBackedBy");
  }

  ScriptBackedBy::~ScriptBackedBy()
  {
  }

  void ScriptBackedBy::onInitialized()
  {
    // When loading a saved instance this will have been already set and we
    // already have a valid handle. Only create a new instance on a really new
    // object.
    if (!hasInstantiatedScriptObject())
    {
      instantiateScriptObject(mScriptClassName, mScriptInstance);
    }
  }

  void ScriptBackedBy::onDestroyed()
  {
    if (gameWorld()->scriptVM().scriptObjects().isValid(mScriptObject))
    {
      gameWorld()->scriptVM().mapping().unmap(mScriptObject, SO());
      gameWorld()->scriptVM().scriptObjects().destroy(mScriptObject);
    }
  }

  void ScriptBackedBy::instantiateScriptObject(const bs::String& className,
                                               const bs::String& instance)
  {
    mScriptObject = gameWorld()->scriptVM().instanciateClass(className, instance, SO());
  }

  bool ScriptBackedBy::hasInstantiatedScriptObject() const
  {
    return mScriptObject != Scripting::SCRIPT_OBJECT_HANDLE_INVALID;
  }

  Scripting::ScriptObject& ScriptBackedBy::scriptObjectData()
  {
    // Will throw if the handle is invalid
    return gameWorld()->scriptVM().scriptObjects().get(mScriptObject);
  }

  REGOTH_DEFINE_RTTI(ScriptBackedBy)
}  // namespace REGoth
