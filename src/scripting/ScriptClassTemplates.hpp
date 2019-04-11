#pragma once
#include <BsPrerequisites.h>
#include <scripting/ScriptSymbolStorage.hpp>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * In the original game, the classes used by the scripts are backed by actual
     * native structures embedded into their game-object counterparts. The scripts
     * would then access the objects memory through some raw pointer trickery. This
     * is rather unsafe (but also led to the creation of the Ikarus scripting framework).
     *
     * Since we don't want to go through raw pointers and don't want to hardcode the
     * script objects structures in the engine, we have to take a more dynamic approach.
     * By analyzing the symbol-table, we can easily figure out the structure of all
     * classes used by the scripts. For these, we create simple key/value pairs which
     * store the data for the scripts.
     *
     * This class will go through all symbols to find those of type `Class`. Then
     * it will find all symbols describing the member variables of that class and build
     * a template with all member variables for new objects to use.
     *
     * The templates are then used to create new script objects to they have all the
     * member variables they need.
     *
     *
     * # What exactly is a class template?
     *
     * Our script objects are basically simple key/value-pairs. With a key for each
     * member variable. The blank script object would have not one single key, but
     * other code will expect an instance of a class to provide all member variables
     * of that class!
     *
     * To be able to provide script objects which look like they were created from
     * a certain class, we gather all member variables of that class and create
     * keys for them in a script object (With default values). That script object
     * will be then used as a template: When someone wants to instanciate a class
     * the template is copied to the new script object.
     */
    class ScriptClassTemplates
    {
    public:
      /**
       * Goes through all classes and gathers their member variables to
       * create all class templates.
       *
       * @param  scriptSymbols  Script symbol storage containing all symbols to search.
       */
      void createClassTemplates(const ScriptSymbolStorage& scriptSymbols);

      /**
       * Returns the class template for the given class name.
       *
       * Throws if the class does not exist.
       *
       * @param  className  Name of the class to get the template from.
       *
       * @return Requested Class template.
       */
      const ScriptObject& getClassTemplate(const bs::String& className) const;

    private:
      /**
       * Creates a single script class template. See createClassTemplates().
       */
      ScriptObject createClassTemplate(const bs::String& className,
                                       const bs::Vector<SymbolIndex>& members,
                                       const ScriptSymbolStorage& scriptSymbols);

      /**
       * Converts a symbol name to the actual member variable name.
       *
       * Inside the symbol-table the symbols for member variables are named
       * like `C_ITEM.SOMETHING`. In our templates, we don't want to repeat
       * the class name in all member variables, so we can just strip the
       * part before and including the `.` to clean it.
       *
       * If there is no `.` or if there is more than one, then the whole name is returned.
       *
       * @param  memberSymbolName  Name of the symbol from the member variable, e.g.
       *                           `C_ITEM.SOMETHING`.
       *
       * @return Actual member variable name.
       */
      bs::String demangleMemberName(const bs::String& memberSymbolName);

      /**
       * Script object initialized with all member variables for each type
       * of class. The actual values of the variables will also have their
       * default values.
       */
      bs::Map<bs::String, ScriptObject> mClassTemplates;
    };
  }  // namespace Scripting
}  // namespace REGoth
