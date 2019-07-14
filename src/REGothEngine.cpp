#include "REGothEngine.hpp"
#include <BsApplication.h>
#include <assert.h>
#include <BsZenLib/ImportMaterial.hpp>
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ResourceManifest.hpp>
#include <Components/BsCCamera.h>
#include <FileSystem/BsFileSystem.h>
#include <Importer/BsImporter.h>
#include <Input/BsVirtualInput.h>
#include <Scene/BsSceneObject.h>
#include <engine-content/EngineContent.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameFiles.hpp>
#include <original-content/VirtualFileSystem.hpp>

using namespace REGoth;

/**
 * Name of REGoth's own content directory
 */
const bs::String REGOTH_CONTENT_DIR_NAME = "content";

REGothEngine::~REGothEngine()
{
}

void REGothEngine::loadGamePackages(const bs::Path& executablePath, const bs::Path& gameDirectory)
{
  OriginalGameFiles files = OriginalGameFiles(gameDirectory);

  gVirtualFileSystem().setPathToEngineExecutable(executablePath.toString());

  REGOTH_LOG(Info, Uncategorized, "[VDFS] Indexing packages: ");

  for (auto p : files.allVdfsPackages())
  {
    REGOTH_LOG(Info, Uncategorized, "[VDFS]  - {0}", p.getFilename());
    gVirtualFileSystem().loadPackage(p);
  }

  gVirtualFileSystem().mountDirectory(files.vdfsFileEntryPoint());

  loadModPackages(files);
}

void REGothEngine::loadModPackages(const OriginalGameFiles& files)
{
  // Don't load mod-files by defaults
}

void REGothEngine::saveCachedResourceManifests()
{
  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Saving resource manifests:");

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine]   - Gothic Cache");
  BsZenLib::SaveResourceManifest();

  // REGothEngine-Content manifest is saved after every resource load since
  // there are only a few resources to handle. If that ever takes too long
  // the manifest should be saved here.
}

bool REGothEngine::hasFoundGameFiles()
{
  return gVirtualFileSystem().hasFoundGameFiles();
}

void REGothEngine::findEngineContent(const bs::Path& executablePath)
{
  mEngineContent = bs::bs_shared_ptr_new<EngineContent>(executablePath);

  if (!mEngineContent->hasFoundContentDirectory())
  {
    REGOTH_THROW(InvalidStateException, "Did not find REGoth content directory!");
  }

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Found REGoth-content directory at: {0}",
             mEngineContent->contentPath().toString());
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

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Loading cached resource manifests");

  if (!mEngineContent)
  {
    REGOTH_THROW(InvalidStateException,
                 "Engine Content not initialized, has findEngineContent() been called?");
  }

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine]   - REGoth Assets");
  mEngineContent->loadResourceManifest();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine]   - Original Gothic Assets");
  BsZenLib::LoadResourceManifest();
}

void REGothEngine::setupInput()
{
  using namespace bs;

  auto inputConfig = gVirtualInput().getConfiguration();

  // Camera controls for buttons (digital 0-1 input, e.g. keyboard or gamepad button)
  inputConfig->registerButton("MoveForward", BC_W);
  inputConfig->registerButton("MoveBack", BC_S);
  inputConfig->registerButton("MoveForward", BC_UP);
  inputConfig->registerButton("MoveBack", BC_DOWN);
  inputConfig->registerButton("StrafeLeft", BC_Q);
  inputConfig->registerButton("StrafeRight", BC_E);
  inputConfig->registerButton("TurnLeft", BC_A);
  inputConfig->registerButton("TurnRight", BC_D);
  inputConfig->registerButton("TurnLeft", BC_LEFT);
  inputConfig->registerButton("TurnRight", BC_RIGHT);
  inputConfig->registerButton("FastMove", BC_LSHIFT);
  inputConfig->registerButton("ToggleWalking", BC_RSHIFT);
  inputConfig->registerButton("ToggleSneaking", BC_X);
  inputConfig->registerButton("Jump", BC_LMENU);
  inputConfig->registerButton("Rotate", BC_MOUSE_LEFT);
  inputConfig->registerButton("ToggleMeleeWeapon", BC_1);
  inputConfig->registerButton("Action", BC_LCONTROL);
  inputConfig->registerButton("QuickSave", BC_F5);

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
  HCamera sceneCamera        = sceneCameraSO->addComponent<CCamera>();
  sceneCamera->setMain(true);
  sceneCamera->setMSAACount(1);

  // Disable some fancy rendering
  auto rs = sceneCamera->getRenderSettings();

  rs->screenSpaceReflections.enabled = false;
  rs->ambientOcclusion.enabled       = false;
  rs->enableIndirectLighting         = false;
  rs->enableFXAA                     = false;
  rs->enableHDR                      = false;
  rs->enableTonemapping              = false;
  rs->enableAutoExposure             = false;
  rs->enableSkybox                   = false;
  rs->exposureScale                  = 0.f;
  rs->gamma                          = 2.f;
  rs->cullDistance                   = 100.0f;

  sceneCamera->setRenderSettings(rs);

  mMainCamera = sceneCamera;
}

void REGothEngine::setupScene()
{
  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Setting up scene");
}

void REGothEngine::setShaders()
{
  if (!mEngineContent)
  {
    REGOTH_THROW(InvalidStateException,
                 "Has not found REGoth content yet, has findEngineContent() been called?");
  }

  EngineContent::Shaders shaders = mEngineContent->loadShaders();

  BsZenLib::SetShaderFor(BsZenLib::ShaderKind::Opaque, shaders.opaque);

  // FIXME: Use correct shader
  BsZenLib::SetShaderFor(BsZenLib::ShaderKind::AlphaMasked, shaders.opaque);

  // FIXME: Use correct shader
  BsZenLib::SetShaderFor(BsZenLib::ShaderKind::Transparent, shaders.opaque);
}

void REGothEngine::run()
{
  // FIXME: This is a workaround for the camera not being able to move caused by
  //        making bs::SceneManager::findComponent work in the zen loader.
  mMainCamera->SO()->setActive(false);
  mMainCamera->SO()->setActive(true);

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Running mainloop now!");

  bs::Application::instance().runMainLoop();
}

void REGothEngine::shutdown()
{
  if (bs::Application::isStarted())
  {
    REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Shutting down bs::f");

    bs::Application::shutDown();
  }
  else
  {
    REGOTH_LOG(Warning, Uncategorized,
               "[REGothEngine] Received shutdown request, but bs::f is not running!");
  }
}

int ::REGoth::main(REGothEngine& regoth, int argc, char** argv)
{
  regoth.initializeBsf();

  bs::Path engineExecutablePath = bs::Path(argv[0]);
  bs::Path gameDirectory;

  // No game path supplied. Check whether we are running from within a game directory.
  if (argc < 2)
  {
    std::cout << "Trying to find game installation root from engine executable upwards..."
              << std::endl;

    gameDirectory = OriginalGameFiles::findGameFilesRoot(engineExecutablePath);

    if (gameDirectory == bs::Path::BLANK &&
        engineExecutablePath != bs::FileSystem::getWorkingDirectoryPath())
    {
      std::cout << "Trying to find game installation root from working directory..." << std::endl;

      gameDirectory =
          OriginalGameFiles::findGameFilesRoot(bs::FileSystem::getWorkingDirectoryPath());
    }
  }
  else
  {
    gameDirectory = bs::Path(argv[1]);
  }

  if (gameDirectory == bs::Path::BLANK)
  {
    std::cout << "Could not find game installation!" << std::endl;
    std::cout << "Try to supply one by calling REGoth like this:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "    REGoth <path/to/game>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Or place REGoth in a subdirectory of your game installation." << std::endl;

    return -1;
  }
  else
  {
    std::cout << "Using game installation at: " << gameDirectory.toString() << std::endl;
  }

  engineExecutablePath.makeAbsolute(bs::FileSystem::getWorkingDirectoryPath());
  gameDirectory.makeAbsolute(bs::FileSystem::getWorkingDirectoryPath());

  REGOTH_LOG(Info, Uncategorized, "[Main] Running REGothEngine");
  REGOTH_LOG(Info, Uncategorized, "[Main]  - Engine executable: {0}",
             engineExecutablePath.toString());
  REGOTH_LOG(Info, Uncategorized, "[Main]  - Game directory:    {0}", gameDirectory.toString());

  REGOTH_LOG(Info, Uncategorized, "[Main] Finding REGoth content-directory");
  regoth.findEngineContent(engineExecutablePath);

  REGOTH_LOG(Info, Uncategorized, "[Main] Loading original game packages");
  regoth.loadGamePackages(engineExecutablePath, gameDirectory);

  if (!regoth.hasFoundGameFiles())
  {
    std::cout << "No files loaded into the VDFS - is the datapath correct?" << std::endl;
    return -1;
  }

  regoth.loadCachedResourceManifests();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Loading Shaders");

  regoth.setShaders();
  regoth.setupInput();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Setting up Main Camera");

  regoth.setupMainCamera();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Setting up Scene");

  regoth.setupScene();

  regoth.saveCachedResourceManifests();

  regoth.run();

  regoth.saveCachedResourceManifests();

  regoth.shutdown();

  return 0;
}
