World
=====

This chapter will discuss how to load and use the game world in REGoth.

Most world-related operations can be found inside the ``src/world``-directory,
inside ``GameWorld.hpp``, more specifically.


Loading a World
---------------

There are multiple options for loading a world in REGoth:

1. load everything and run the init-script,
2. load everything but don't run the init-script,
3. load only the world-mesh.

We will go through all of them.

As starting point, we will use this application template:

.. code-block:: cpp

   #include "BsFPSCamera.h"
   #include "REGothEngine.hpp"
   #include <Components/BsCCamera.h>
   
   class REGothWorldViewer : public REGoth::REGothEngine
   {
   public:
     void setupMainCamera() override
     {
       REGoth::REGothEngine::setupMainCamera();
   
       mMainCamera->SO()->addComponent<bs::FPSCamera>();
     }
   
     void setupScene() override
     {

     }
   
   protected:
   };
   
   int main(int argc, char** argv)
   {
     REGothWorldViewer regoth;
   
     return REGoth::main(regoth, argc, argv);
   }

This will set up an empty application with a flying FPS-style camera.


Loading everything and run the init-script
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is the option used to initialize a full game world as you would expect from
playing Gothic. The world-mesh will be there, all supported Vobs are loaded,
decoration, lights, basically everything possible to load from a ZEN.  However
loading only the ZEN will not get you NPCs, some items and other objects you
might expect to find, as those are placed via the worlds init-script.
Therefore, this init-script is executed as well so that those are put into the
world as well.
Since scripts need to be executed, you need to also initialize the script-VM with
a valid ``GOTHIC.DAT`` file.

You will need to include the following headers:

.. code-block:: cpp

   #include <scripting/ScriptVMInterface.hpp>
   #include <world/GameWorld.hpp>
   #include <daedalus/DATFile.h>


Then, inside the applications ``setupScene``-method, go like this:

.. code-block:: cpp

   void setupScene() override
   {
     Daedalus::DATFile dat("path/to/gothic/Data/Scripts/_compiled/GOTHIC.DAT");
     REGoth::Scripting::loadGothicDAT(dat);
 
     REGoth::World::loadWorldFromZEN("ADDONWORLD.ZEN");
   }

This will set you up with everything you need. All world-related functions in
``REGoth::gWorld()`` should work.


Load everything but don't run the init-script
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Since running the init-script can take a long time, or you might not want to
deal with all the NPCs and a full game experience, you can skip running the
worlds init-script. To do that, add the following flag to your call to
``REGoth::World::loadWorldFromZEN``:

.. code-block:: cpp

   REGoth::World::loadWorldFromZEN("ADDONWORLD.ZEN",
                                   World::GameWorld::Init::NoInitScripts);

Otherwise the process is the same as described in the previous chapter.

.. note::

   You also need to initialize the script-VM here, even when you don't want to execute
   the init-scripts as the objects for Items laying on the floor need to run their
   script-constructor to get the visual they should display.
   

Load only the world-mesh
~~~~~~~~~~~~~~~~~~~~~~~~

If you really only need the worldmesh, you don't need to initialize the script-VM. You will however lose
support for using ``REGoth::gWorld()``. This is really only useful for development related testing.

To only load the worldmesh from a ZEN, you need to include the following header:

.. code-block:: cpp

   #include <world/internals/ConstructFromZEN.hpp>

And do this inside ``setupScene``:

.. code-block:: cpp

   void setupScene() override
   {
     REGoth::World::loadWorldMeshFromZEN("ADDONWORLD.ZEN");
   }

As you can see, this is using an internal header, so handle it with care.


Using the world
---------------

Once you have successfully initialized a world, you can use ``REGoth::gWorld()``
to access it. See ``src/world/GameWorld.hpp`` for the ``GameWorld``-class.

The ``GameWorld``-class allows you to create your own NPCs, Items and also gives
you access to the Waynet among other things.
