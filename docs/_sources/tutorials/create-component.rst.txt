Creating a new component
========================

Multiple things need to be done when creating a new component.  This file describes the canonical
way and templates to use.

For the sake of making *Replace All*-operations easier, all examples will use a component named
`NewThing`.


Files to create
---------------

Each component needs a

 - Header file, ``src/components/NewThing.hpp``,
 - Source file, ``src/components/NewThing.cpp``,
 - and an RTTI-implementation, ``src/RTTI/RTTI_NewThing.hpp``.


Header File
-----------

Use the following template for ``NewFile.hpp``:

.. code-block:: cpp

   #pragma once
   #include <Scene/BsComponent.h>
   #include <RTTI/RTTIUtil.hpp>

   namespace REGoth
   {
     /**
      * TODO: Documentation of NewThing
      */
     class NewThing : public bs::Component
     {
     public:
       NewThing(const bs::HSceneObject& parent);
       virtual ~NewThing();

     private:

     public:
       REGOTH_DECLARE_RTTI(NewThing)

     protected:
       NewThing() = default;  // For RTTI
     };
   }  // namespace REGoth


Source File
-----------

Use the following template for ``NewThing.cpp``:

.. code-block:: cpp

   #include "NewThing.hpp"
   #include <RTTI/RTTI_NewThing.hpp>

   namespace REGoth
   {
     NewThing::NewThing(const bs::HSceneObject& parent)
         : bs::Component(parent)
     {
       setName("NewThing");
     }

     NewThing::~NewThing()
     {
     }

     REGOTH_DEFINE_RTTI(NewThing)

   }  // namespace REGoth


RTTI File
---------

Use the following template for ``RTTI_NewThing.hpp``:

.. code-block:: cpp

   #include "RTTIUtil.hpp"
   #include <components/NewThing.hpp>

   namespace REGoth
   {
     class RTTI_NewThing
         : public bs::RTTIType<NewThing, bs::Component, RTTI_NewThing>
     {
       BS_BEGIN_RTTI_MEMBERS
       // TODO: Fill RTTI Members
       BS_END_RTTI_MEMBERS

     public:
       RTTI_NewThing()
       {
       }

       REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(NewThing)
     };
   }  // namespace REGoth


RTTI Type ID
------------

As a last step, add the RTTI-Types ID to ``src/RTTI/RTTI_TypeIDs.hpp``:

.. code-block:: cpp

   enum TypeID_REGoth
   {
     TID_REGOTH_VisualCharacter              = 600000,

     // ... <snip> ...

     TID_REGOTH_ScriptObjectMapping          = 600036,
     TID_REGOTH_GameClock                    = 600037,
     TID_REGOTH_NewThing                     = 600038,
   };


CMake
-----

Don't forget to add all files you created to ``src/CMakeLists.txt``.

.. note:: Also add all header files so they show up in IDEs.
