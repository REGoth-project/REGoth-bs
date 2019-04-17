Setting up a Test-Application
=============================

REGoth comes with lots of small test utilities, which show specific parts of the game for easier
testing and development. For example, there are viewers for the World-Mesh, Characters, the Waynet
and some others.

All of those Test-Applications reside directly inside the ``src``-directory, with a name of ``main_something.cpp``.

To get started with your own "playgound", let's call it *MyThing*, create an empty ``cpp`` file ``src/main_MyThing.cpp`` and add the following skeleton to it:

.. code-block:: cpp
   
   #include "REGothEngine.hpp"
   
   class MyThing : public REGoth::REGothEngine
   {
   public:
     void setupMainCamera() override
     {
       REGoth::REGothEngine::setupMainCamera();
     }
   
     void setupScene() override
     {

     }
   
   protected:
   };
   
   int main(int argc, char** argv)
   {
     MyThing regoth;
   
     return REGoth::main(regoth, argc, argv);
   }

The ``REGoth::REGothEngine``-class handles setting up everything, so once you get to run your *MyThing*,
the engine will be running and display an empty window.

To run it, we need to *build* it first, of course. This is done by opening ``src/CMakeLists.txt`` and adding the following lines to the bottom of the file:

.. code-block:: cmake

   add_executable(MyThing main_MyThing.cpp)
   target_link_libraries(MyThing REGothEngine samples-common)

Then you can build your *MyThing* using CMake as usual.
