Setting up a Test-Application
=============================

REGoth comes with lots of small test utilities, which show specific parts of the game for easier
testing and development. For example, there are viewers for the World-Mesh, Characters, the Waynet
and some others.

All of those Test-Applications reside directly inside the ``src``-directory, with a name of
``main_something.cpp``.


Hello REGoth!
-------------

To get started with your own "playgound", let's call it *HelloREGoth*, create an empty ``cpp`` file
``src/main_HelloREGoth.cpp`` and add the following skeleton to it:

.. code-block:: cpp

  #include <memory>

  #include <REGothEngine.hpp>

  class HelloREGoth : public REGoth::REGothEngineDefaultConfig
  {
  public:
    // Use the super class constructor for now.
    using REGoth::REGothEngineDefaultConfig::REGothEngineDefaultConfig;

    void setupMainCamera() override
    {
      REGoth::REGothEngine::setupMainCamera();
    }

    void setupScene() override
    {
      // Your code to setup the test environment goes here, but let's just greet REGoth for now. :)
      REGOTH_LOG(Info, Uncategorized, "Hello REGoth!");
    }
  };

  int main(int argc, char** argv)
  {
    // Create a default configuration object and parse the command line arguments.
    std::unique_ptr<const REGoth::EngineConfig> config =
        REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
    // Construct the engine and pass the configuration object.
    HelloREGoth engine{std::move(config)};

    // Run the engine.
    return REGoth::runEngine(engine);
  }

Additionally, add the following lines to ``src/CMakeLists.txt``:

.. code-block:: cmake

   add_executable(HelloREGoth main_HelloREGoth.cpp)
   target_link_libraries(HelloREGoth REGothEngine samples-common)

The ``REGoth::REGothEngineDefaultConfig``-class handles setting up everything, so once you get to
run *HelloREGoth*, the engine will be running and display an empty window.

Your code can then be build normally by just running ``cmake --build .`` in the ``build`` folder,
or also by explicitly specifying your target (``cmake --build . --target HelloREGoth``).

If you now proceed to call the executable via ``./HelloREGoth $GAME_PATH`` (and given that
``$GAME_PATH`` is the path to a valid Gothic or Gothic 2 installation), you should see the
greeting ``[INFO] Hello REGoth!`` in the logs.


Specifying a Custom Configuration
---------------------------------

You may reach a point where you'd like to add command line options to more efficiently or more
easily test certain aspects.  The following template shows what needs to be added in order to make
use of the internally used library, namely cxxopts_, for command line parsing.

.. _cxxopts: https://github.com/jarro2783/cxxopts

.. code-block:: cpp

  #include <memory>
  #include <string>

  #include <REGothEngine.hpp>

  struct HelloREGothConfig : public REGoth::EngineConfig
  {
    // Here we define the CLI options using cxxopts.
    virtual void registerCLIOptions(cxxopts::Options& opts) override
    {
      // We only define one option, namely `--greeting`, which will be written into the `greeting`
      // attribute in this struct.
      const std::string grp = "HelloREGoth";
      opts.add_option(grp, "", "greeting", "Specify your custom greeting",
                      cxxopts::value<bs::String>(greeting), "[GREETING]");
    }

    // Here we can verify the parsed options and modify the if need be.
    virtual void verifyCLIOptions() override
    {
      // Make sure to NOT greet JoTreeS!
      if (greeting == "JoTreeS")
      {
        REGOTH_THROW(InvalidStateException, "A real Gothic fan would never greet JoTreeS!");
      }

      // If nothing is set, just greet REGoth. :)
      if (greeting == "")
      {
        greeting = "REGoth";
      }
    }

    // Here the greeting will be stored after parsing or after modification in `verifyCLIOptions`.
    bs::String greeting;
  };

  // We don't derive from `REGoth::REGothEngineDefaultConfig` now for obvious reason.
  class HelloREGoth : public REGoth::REGothEngine
  {
  public:
    // We define a constructor to make sure that our engine can only be instantiated with
    // the correct configuration object type.
    HelloREGoth(std::unique_ptr<const HelloREGothConfig>&& config)
        : mConfig{std::move(config)}
    {
      // pass
    }

    // We override the `config` getter to return the specified configuration object type.
    const HelloREGothConfig* config() const override
    {
      return mConfig.get();
    }

    void setupMainCamera() override
    {
      REGoth::REGothEngine::setupMainCamera();
    }

    void setupScene() override
    {
      // This time we greet whoever was mentioned via the `--greeting` CLI argument.
      REGOTH_LOG(Info, Uncategorized, "Hello " + config()->greeting + "!");
    }

  private:
    // In this member, our configuration object will be stored.
    std::unique_ptr<const HelloREGothConfig> mConfig;
  };

  int main(int argc, char** argv)
  {
    // Create a default configuration object and parse the command line arguments.
    std::unique_ptr<const HelloREGothConfig> config =
        REGoth::parseArguments<HelloREGothConfig>(argc, argv);
    // Construct the engine and pass the configuration object.
    HelloREGoth engine{std::move(config)};

    // Run the engine.
    return REGoth::runEngine(engine);
  }

If you now call the executable ``./HelloREGoth $GAME_PATH``, you will see that still REGoth will be
greeted (this is the default behaviour). However, after specifying the greeting, for example with
``./HelloREGoth $GAME_PATH --greeting you``, you should see ``[INFO] Hello you!`` instead.
*(Don't greet JoTreeS tho.)*
