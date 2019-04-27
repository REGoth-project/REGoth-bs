#pragma once
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>
#include <scripting/ScriptTypes.hpp>
#include <RTTI/RTTIUtil.hpp>
#include "NeedsGameWorld.hpp"

namespace REGoth
{
  namespace Scripting
  {
    struct ScriptObject;
  }

  /**
   * Base-component for components which need to be closely related to a script object.
   *
   * For example, an Item would have a component, a visual, and so on, but also have
   * a script object which is initialized by scripts. So in order to a safe interface
   * between the component and the script object, this class links them together.
   *
   * The script object linked to the ScriptBackedBy-component will be called *Backing
   * Script Object*.
   *
   * Initialization
   * ==============
   *
   * Via the constructor, the class inheriting from the ScriptBackedBy-component
   * defines the class the backing script object should have. A script object of that
   * class is then instantiated inside the `onInitialized()`-method, so be sure to
   * call it if you're overriding it in the inheriting class!
   *
   *
   * Serialization Strategy
   * ======================
   *
   * To have this component properly serialized, we need to also make sure that the
   * script object handle will stay valid after loading. Therefore, it is crucial that
   * the ScriptObjectStorage is also serialized and all handles stay the same.
   *
   * When loading the object again, we have to hope our handle stayed the same and will
   * skip creating a new instance of the backing script object inside `onInitialized()`.
   */
  class ScriptBackedBy : public NeedsGameWorld
  {
  public:
    ScriptBackedBy(const bs::HSceneObject& parent, const bs::String& className,
                   const bs::String& instance);
    virtual ~ScriptBackedBy();

  protected:

    void onInitialized() override;
    void onDestroyed() override;

    /**
     * @return Script object backing this component.
     *
     * Throws if it does not exist.
     */
    Scripting::ScriptObject& scriptObjectData();

    /**
     * @return Handle of the script object backing this component.
     */
    Scripting::ScriptObjectHandle scriptObject() const { return mScriptObject; }

    /**
     * @return Whether we already have instantiated a script object.
     */
    bool hasInstantiatedScriptObject() const;

  private:

    /**
     * Instanciates the class backing this component. Must be called early, otherwise
     * exceptions will be thrown when accessing the script object data.
     *
     * Throws if the class or instance doesn't exist.
     *
     * @param  className  Class to instanciate the object from, e.g. `C_ITEM`.
     * @param  instance   Instance-name to instanciate, e.g. `ITFO_APPLE`.
     */
    void instantiateScriptObject(const bs::String& className, const bs::String& instance);

    /**
     * Script object backing this item
     */
    Scripting::ScriptObjectHandle mScriptObject = Scripting::SCRIPT_OBJECT_HANDLE_INVALID;

    bs::String mScriptClassName;
    bs::String mScriptInstance;

  public:
    REGOTH_DECLARE_RTTI(ScriptBackedBy)

  public:
    ScriptBackedBy() = default; // For RTTI
  };
}  // namespace REGoth
