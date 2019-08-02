Core
====

The core module features the main data structures representing an engine, as well as functions and
utility tools to bootstrap them.

The heart is the abstract class :cpp:class:`REGoth::Engine`, which already implements lots
of functionality to run the game.  An engine needs a configuration, which is represented by the
struct :cpp:class:`REGoth::EngineConfig`.  In most cases, the configuration options covered by
:cpp:class:`REGoth::EngineConfig` are enough, so the concrete class :cpp:class:`REGoth::EmptyGame`
(extends :cpp:class:`REGoth::Engine`) can be used.  A concrete example on how to use
:cpp:class:`REGoth::EmptyGame` can be found :ref:`here <setting_up_app_hello_regoth>`.

If the default configuration covered by :cpp:class:`REGoth::EngineConfig` is *not* enough, it is
best practice to extend directly from :cpp:class:`REGoth::Engine` and implement the function
:cpp:func:`REGoth::Engine::config()`.
Make sure that your specialised configuration object extends :cpp:class:`REGoth::EngineConfig`.
For a code example on how employ a custom configuration, see
:ref:`here <setting_up_app_custom_conf>`.

To construct an :cpp:class:`REGoth::EngineConfig` from command line parameters, the convenience
function :cpp:func:`REGoth::parseArguments()` is provided.  It is best practice to surrender the
ownership of an engine configuration object to the engine by ``std::move``-ing it into the
constructor call.

To run an engine, the convenience function :cpp:func:`REGoth::runEngine()` is provided.


Reference
---------

.. doxygengroup:: core
  :members:
  :protected-members:
  :private-members:
