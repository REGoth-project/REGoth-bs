#include "ScriptBackedBy.hpp"
#include <scripting/ScriptVMInterface.hpp>

namespace REGoth
{
  ScriptBackedBy::ScriptBackedBy(const bs::HSceneObject& parent, const bs::String& className,
                                 const bs::String& instance)
      : mScriptClassName(className)
      , mScriptInstance(instance)
      , bs::Component(parent)
  {
  }

  ScriptBackedBy::~ScriptBackedBy()
  {
  }

  void ScriptBackedBy::onCreated()
  {
    instanciateScriptObject(mScriptClassName, mScriptInstance);
  }

  void ScriptBackedBy::onDestroyed()
  {
    if (gGameScript().scriptObjects().isValid(mScriptObject))
    {
      gGameScript().mapping().unmap(mScriptObject, SO());
      gGameScript().scriptObjects().destroy(mScriptObject);
    }
  }

  void ScriptBackedBy::instanciateScriptObject(const bs::String& className,
                                               const bs::String& instance)
  {
    mScriptObject = gGameScript().instanciateClass(className, instance, SO());
  }

  Scripting::ScriptObject& ScriptBackedBy::scriptObjectData()
  {
    // Will throw if the handle is invalid
    return gGameScript().scriptObjects().get(mScriptObject);
  }

}  // namespace REGoth
