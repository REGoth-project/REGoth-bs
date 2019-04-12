#include "ScriptObjectMapping.hpp"

namespace REGoth
{
  namespace Scripting
  {
    bool ScriptObjectMapping::isMappedToSomething(ScriptObjectHandle scriptObject)
    {
      return mScriptToSceneObjectMapping.find(scriptObject) != mScriptToSceneObjectMapping.end();
    }

    bool ScriptObjectMapping::areMapped(ScriptObjectHandle scriptObject,
                                        bs::HSceneObject sceneObject)
    {
      auto reverseSceneObject = mScriptToSceneObjectMapping.find(scriptObject);

      if (reverseSceneObject->second != sceneObject) return false;

      return true;
    }

    bs::HSceneObject ScriptObjectMapping::getMappedSceneObject(ScriptObjectHandle scriptObject)
    {
      auto it = mScriptToSceneObjectMapping.find(scriptObject);

      if (it == mScriptToSceneObjectMapping.end())
      {
        REGOTH_THROW(InvalidParametersException, "Script Object is not mapped to anything!");
      }

      return it->second;
    }

    void ScriptObjectMapping::unmap(ScriptObjectHandle scriptObject, bs::HSceneObject sceneObject)
    {
      if (!areMapped(scriptObject, sceneObject))
      {
        REGOTH_THROW(InvalidParametersException,
                     "Those two objects were not mapped, so unmapping is impossible!");
      }

      mScriptToSceneObjectMapping.erase(scriptObject);
    }

    void ScriptObjectMapping::map(ScriptObjectHandle scriptObject, bs::HSceneObject sceneObject)
    {
      if (isMappedToSomething(scriptObject))
      {
        REGOTH_THROW(InvalidParametersException,
                     "Script Object already has a mapping, can't map twice!");
      }

      mScriptToSceneObjectMapping[scriptObject] = sceneObject;
    }
  }  // namespace Scripting
}  // namespace REGoth
