#pragma once
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>
#include <scripting/ScriptTypes.hpp>

namespace REGoth
{
  namespace Scripting
  {
    struct ScriptObject;
  }

  /**
   * Component for an item laying on the floor. Will also create and handle
   * the visuals.
   */
  class ScriptBackedBy : public bs::Component
  {
  public:
    ScriptBackedBy(const bs::HSceneObject& parent, const bs::String& className,
                   const bs::String& instance);
    virtual ~ScriptBackedBy();

  protected:

    void onCreated() override;
    void onDestroyed() override;

    /**
     * @return Script object backing this component.
     *
     * Throws if it does not exist.
     */
    Scripting::ScriptObject& scriptObjectData();

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
    void instanciateScriptObject(const bs::String& className, const bs::String& instance);

    /**
     * Script object backing this item
     */
    Scripting::ScriptObjectHandle mScriptObject;

    bs::String mScriptClassName;
    bs::String mScriptInstance;
  };
}  // namespace REGoth
