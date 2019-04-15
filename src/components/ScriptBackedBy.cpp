#include "ScriptBackedBy.hpp"
#include <scripting/ScriptVMInterface.hpp>
#include <RTTI/RTTI_ScriptBackedBy.hpp>

namespace REGoth
{
  ScriptBackedBy::ScriptBackedBy(const bs::HSceneObject& parent, const bs::String& className,
                                 const bs::String& instance)
      : mScriptClassName(className)
      , mScriptInstance(instance)
      , bs::Component(parent)
  {
    setName("ScriptBackedBy");
  }

  ScriptBackedBy::~ScriptBackedBy()
  {
  }

  void ScriptBackedBy::onInitialized()
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

  bs::RTTITypeBase* ScriptBackedBy::getRTTIStatic()
  {
    return RTTI_ScriptBackedBy::instance();
  }

  bs::RTTITypeBase* ScriptBackedBy::getRTTI() const
  {
    return ScriptBackedBy::getRTTIStatic();
  }
}  // namespace REGoth
