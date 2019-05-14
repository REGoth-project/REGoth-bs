#pragma once
#include "ScriptObject.hpp"
#include <RTTI/RTTIUtil.hpp>
#include <BsPrerequisites.h>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * Some script objects directly belong to a scene object. This is where the
     * mapping between them is stored.
     */
    class ScriptObjectMapping : public bs::IReflectable
    {
    public:
      ScriptObjectMapping() = default;

      /**
       * Some script objects directly belong to a scene object. This function
       * stores that mapping between the two.
       *
       * @note   The mapping can be undone by calling unmap().
       * @note   To query the mapping, use getMappedSceneObject() or getMappedScriptObject().
       *
       * @param  scriptObject  Handle to the script object
       * @param  sceneObject   Handle to the scene object
       */
      void map(ScriptObjectHandle scriptObject, bs::HSceneObject sceneObject);

      /**
       * Undos the mapping between the given script- and scene-object.
       *
       * Throws if the two objects were not mapped.
       *
       * @see map()
       *
       * @param  scriptObject  Handle to the script object to unmap
       * @param  sceneObject   Handle to the scene object to unmap
       */
      void unmap(ScriptObjectHandle scriptObject, bs::HSceneObject sceneObject);

      /**
       * @return the scene object mapped to the given script object.
       *
       * Throws if no such mapping exists.
       */
      bs::HSceneObject getMappedSceneObject(ScriptObjectHandle scriptObject);

      /**
       * @return Whether the given script object is mapped to a scene object.
       */
      bool isMappedToSomething(ScriptObjectHandle scriptObject);

      /**
       * @return Whether the two given objects are mapped to eachother.
       */
      bool areMapped(ScriptObjectHandle scriptObject, bs::HSceneObject sceneObject);

    private:
      bs::Map<ScriptObjectHandle, bs::HSceneObject> mScriptToSceneObjectMapping;

    public:
      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(ScriptObjectMapping)
    };
  }  // namespace Scripting
}  // namespace REGoth
