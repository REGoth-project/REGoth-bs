#pragma once
#include "ScriptClassTemplates.hpp"
#include "ScriptObjectMapping.hpp"
#include "ScriptObjectStorage.hpp"
#include "ScriptSymbolStorage.hpp"
#include "ScriptSymbols.hpp"
#include <BsPrerequisites.h>
#include <RTTI/RTTIUtil.hpp>
#include <Reflection/BsIReflectable.h>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * Abstract ScriptVM for REGoth.
     *
     * This virtual machine abstracts the daedalus scripts used by the original
     * game, to *maybe* get rid of Daedalus in the feature and to keep the software
     * clean of any scripting nonsense.
     */
    class ScriptVM : public bs::IReflectable
    {
    public:
      ScriptVM()          = default;
      virtual ~ScriptVM() = default;

      /**
       * Initializes the ScriptVM. To be called after the object is constructed.
       */
      void initialize();

      /**
       * Creates a blank script object and gives it all member variables needed for
       * the given class. This will NOT run the instance constructor function!
       *
       * Throws, if the requested class does not exist.
       *
       * @param  className  Script-Class of the object to create, e.g. `C_ITEM`.
       *
       * @return Handle of the created script object.
       */
      ScriptObjectHandle instanciateBlankObjectOfClass(const bs::String& className);

      /**
       * Creates a fully initialized script object of the given class by calling
       * the instances constructor.
       *
       * Throws, if the requested class does not exist.
       *
       * @param  classname          Script-class of the object to create, e.g. `C_ITEM`.
       * @param  instanceName       Name of the instance to instanciate, e.g. `ITFO_APPLE`.
       * @param  mappedSceneObject  Scene-object to map the created script object to.
       *
       * @return Handle of the create script object.
       */
      virtual ScriptObjectHandle instanciateClass(const bs::String& className,
                                                  const bs::String& instanceName,
                                                  bs::HSceneObject mappedSceneObject) = 0;

      /**
       * Runs the init-scripts for the given world.
       *
       * Throws, if the requested world has no init script.
       * Throws, if no hero character is in the world yet.
       *
       * @param  worldName  The world to init, e.g. `NEWWORLD`.
       */
      virtual void initializeWorld(const bs::String& worldName) = 0;

      /**
       * Access to the script object storage
       */
      ScriptObjectStorage& scriptObjects()
      {
        return mScriptObjects;
      }

      const ScriptObjectStorage& scriptObjectsConst() const
      {
        return mScriptObjects;
      }

      /**
       * Access to the script object mapping
       */
      ScriptObjectMapping& mapping()
      {
        return mScriptObjectMapping;
      }

      const ScriptObjectMapping& mappingConst() const
      {
        return mScriptObjectMapping;
      }

    protected:
      /**
       * Get a list of all symbols and move them into the symbol storage vector.
       */
      virtual void fillSymbolStorage() = 0;

    protected:
      // Storages for symbols and objects -----------------------------------------------------------
      ScriptSymbolStorage mScriptSymbols;
      ScriptObjectStorage mScriptObjects;
      ScriptClassTemplates mClassTemplates;
      ScriptObjectMapping mScriptObjectMapping;

    public:
      REGOTH_DECLARE_RTTI(ScriptVM);
    };
  }  // namespace Scripting
}  // namespace REGoth
