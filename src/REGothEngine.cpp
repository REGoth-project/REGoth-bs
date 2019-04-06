#include "REGothEngine.hpp"
#include <BsApplication.h>
#include <assert.h>
#include "original-content/VirtualFileSystem.hpp"
#include <BsZenLib/ImportPath.hpp>
#include <Components/BsCCamera.h>
#include <FileSystem/BsFileSystem.h>
#include <Input/BsVirtualInput.h>
#include <Resources/BsResourceManifest.h>
#include <Resources/BsResources.h>
#include <Scene/BsSceneObject.h>

using namespace REGoth;

REGothEngine::~REGothEngine()
{
  shutdown();
}

bs::Vector<bs::String> REGothEngine::getVdfsPackagesToLoad()
{
  return {
      "Worlds.vdf",
      "Textures.vdf",
      "Meshes.vdf",
      "Anims.vdf",
  };
}

void REGothEngine::loadOriginalGamePackages(const bs::String& argv0, const bs::Path& gameDirectory)
{
  gVirtualFileSystem().setPathToEngineExecutable(argv0);
  gVirtualFileSystem().setGameDirectory(gameDirectory);

  bs::gDebug().logDebug("[VDFS] Indexing packages: ");

  for (auto p : getVdfsPackagesToLoad())
  {
    if (!gVirtualFileSystem().isPackageAvailable(p))
    {
      bs::gDebug().logDebug("[VDFS]  - " + bs::String(p) + " (not found)");
    }
    else
    {
      bs::gDebug().logDebug("[VDFS]  - " + bs::String(p));
      gVirtualFileSystem().loadPackage(p);
    }
  }
}

bool REGothEngine::hasFoundGameFiles()
{
  return gVirtualFileSystem().hasFoundGameFiles();
}

void REGothEngine::initializeBsf()
{
  using namespace bs;

  // TODO: Make video mode configurable
  VideoMode videoMode(1280, 720);
  Application::startUp(videoMode, "REGoth", false);
}

void REGothEngine::loadCachedResourceManifests()
{
  using namespace bs;

  gDebug().logDebug("[REGothEngine] Loading cached resource manifests");

  if (FileSystem::exists(BsZenLib::GothicPathToCachedManifest("resources")))
  {
    auto prevManifest = ResourceManifest::load(BsZenLib::GothicPathToCachedManifest("resources"),
                                               BsZenLib::GetCacheDirectory());

    gResources().registerResourceManifest(prevManifest);
  }
}

void REGothEngine::saveCachedResourceManifests()
{
  using namespace bs;

  SPtr<ResourceManifest> manifest = gResources().getResourceManifest("Default");
  ResourceManifest::save(manifest, BsZenLib::GothicPathToCachedManifest("resources"),
                         BsZenLib::GetCacheDirectory());
}

void REGothEngine::setupInput()
{
  using namespace bs;

  bs::gDebug().logDebug("[REGothEngine] Setting up default input scheme");

  auto inputConfig = gVirtualInput().getConfiguration();

  // Camera controls for buttons (digital 0-1 input, e.g. keyboard or gamepad button)
  inputConfig->registerButton("Forward", BC_W);
  inputConfig->registerButton("Back", BC_S);
  inputConfig->registerButton("Left", BC_A);
  inputConfig->registerButton("Right", BC_D);
  inputConfig->registerButton("Forward", BC_UP);
  inputConfig->registerButton("Back", BC_DOWN);
  inputConfig->registerButton("RotateLeft", BC_LEFT);
  inputConfig->registerButton("RotateRight", BC_RIGHT);
  inputConfig->registerButton("FastMove", BC_LSHIFT);
  inputConfig->registerButton("Rotate", BC_MOUSE_LEFT);
  inputConfig->registerButton("ToggleMeleeWeapon", BC_1);

  // Camera controls for axes (analog input, e.g. mouse or gamepad thumbstick)
  // These return values in [-1.0, 1.0] range.
  inputConfig->registerAxis("Horizontal", VIRTUAL_AXIS_DESC((UINT32)InputAxis::MouseX));
  inputConfig->registerAxis("Vertical", VIRTUAL_AXIS_DESC((UINT32)InputAxis::MouseY));
}

void REGothEngine::setupMainCamera()
{
  using namespace bs;

  // Add a scene object containing a camera component
  HSceneObject sceneCameraSO = SceneObject::create("SceneCamera");
  HCamera sceneCamera = sceneCameraSO->addComponent<CCamera>();
  sceneCamera->setMain(true);
  sceneCamera->setMSAACount(1);

  // Disable some fancy rendering
  auto rs = sceneCamera->getRenderSettings();

  rs->screenSpaceReflections.enabled = false;
  rs->ambientOcclusion.enabled = false;
  rs->enableIndirectLighting = true;
  rs->enableFXAA = false;
  rs->enableHDR = false;
  rs->enableTonemapping = false;
  rs->cullDistance = 100.0f;

  sceneCamera->setRenderSettings(rs);

  mMainCamera = sceneCamera;
}

void REGothEngine::setupScene()
{
  bs::gDebug().logDebug("[REGothEngine] Setting up scene");
}

void REGothEngine::run()
{
  bs::gDebug().logDebug("[REGothEngine] Running mainloop now!");

  bs::Application::instance().runMainLoop();
}

void REGothEngine::shutdown()
{
  if (bs::Application::isStarted())
  {
    bs::gDebug().logDebug("[REGothEngine] Shutting down bs::f");

    bs::Application::shutDown();
  }
  else
  {
    bs::gDebug().logWarning("[REGothEngine] Received shutdown request, but bs::f is not running!");
  }
}

int ::REGoth::main(REGothEngine& regoth, int argc, char** argv)
{
  regoth.initializeBsf();

  if (argc < 2)
  {
    std::cout << "Usage: REGoth <path/to/game>" << std::endl;
    return -1;
  }

  const bs::String engineExecutablePath = argv[0];
  const bs::String gameDirectory = argv[1];

  bs::gDebug().logDebug("[Main] Running REGothEngine");
  bs::gDebug().logDebug("[Main]  - Engine executable: " + engineExecutablePath);
  bs::gDebug().logDebug("[Main]  - Game directory:    " + gameDirectory);

  bs::gDebug().logDebug("[Main] Loading original game packages");

  regoth.loadOriginalGamePackages(engineExecutablePath, gameDirectory);

  if (!regoth.hasFoundGameFiles())
  {
    std::cout << "No files loaded into the VDFS - is the datapath correct?" << std::endl;
    return -1;
  }

  regoth.loadCachedResourceManifests();

  regoth.setupInput();
  regoth.setupMainCamera();
  regoth.setupScene();

  regoth.saveCachedResourceManifests();

  regoth.run();

  regoth.shutdown();

  return 0;
}
