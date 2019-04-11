#include "ScriptBackedBy.hpp"
#include <scripting/ScriptVMInterface.hpp>

namespace REGoth
{
  ScriptBackedBy::ScriptBackedBy(const bs::HSceneObject& parent, const bs::String& className,
                                 const bs::String& instance)
      : bs::Component(parent)
  {
    instanciateScriptObject(className, instance);
  }

  ScriptBackedBy::~ScriptBackedBy()
  {
    if (gGameScript().scriptObjects().isValid(mScriptObject))
    {
      gGameScript().scriptObjects().destroy(mScriptObject);
    }
  }

  void ScriptBackedBy::instanciateScriptObject(const bs::String& className,
                                               const bs::String& instance)
  {
    mScriptObject = gGameScript().instanciateClass(className, instance);
  }

  Scripting::ScriptObject& ScriptBackedBy::scriptObjectData()
  {
    // Will throw if the handle is invalid
    return gGameScript().scriptObjects().get(mScriptObject);
  }

}  // namespace REGoth
