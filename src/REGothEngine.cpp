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
#include <original-content/OriginalGameFiles.hpp>
#include <original-content/VirtualFileSystem.hpp>

using namespace REGoth;

/**
 * Name of REGoth's own content directory
 */
const bs::String REGOTH_CONTENT_DIR_NAME = "content";

REGothEngine::~REGothEngine()
{
  shutdown();
}

void REGothEngine::loadOriginalGamePackages(const bs::Path& executablePath,
                                            const bs::Path& gameDirectory)
{
  OriginalGameFiles files = OriginalGameFiles(gameDirectory);

  gVirtualFileSystem().setPathToEngineExecutable(executablePath.toString());

  bs::gDebug().logDebug("[VDFS] Indexing packages: ");

  for (auto p : files.allVdfsPackages())
  {
    bs::gDebug().logDebug("[VDFS]  - " + p.getFilename());
    gVirtualFileSystem().loadPackage(p);
  }

  gVirtualFileSystem().mountDirectory(files.vdfsFileEntryPoint());
}

void REGoth::REGothEngine::saveCachedResourceManifests()
{
  bs::gDebug().logDebug("[REGothEngine] Saving resource manifests:");

  bs::gDebug().logDebug("[REGothEngine]   - Gothic Cache");
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

  bs::gDebug().logDebug("[REGothEngine] Found REGoth-content directory at: " +
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

  gDebug().logDebug("[REGothEngine] Loading cached resource manifests");

  if (!mEngineContent)
  {
    REGOTH_THROW(InvalidStateException,
                 "Engine Content not initialized, has findEngineContent() been called?");
  }

  gDebug().logDebug("[REGothEngine]   - REGoth Assets");
  mEngineContent->loadResourceManifest();

  gDebug().logDebug("[REGothEngine]   - Original Gothic Assets");
  BsZenLib::LoadResourceManifest();
}

void REGothEngine::setupInput()
{
  using namespace bs;

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
  rs->cullDistance                   = 100.0f;

  sceneCamera->setRenderSettings(rs);

  mMainCamera = sceneCamera;
}

void REGothEngine::setupScene()
{
  bs::gDebug().logDebug("[REGothEngine] Setting up scene");
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

  bs::Path engineExecutablePath = bs::Path(argv[0]);
  bs::Path gameDirectory        = bs::Path(argv[1]);

  engineExecutablePath.makeAbsolute(bs::FileSystem::getWorkingDirectoryPath());
  gameDirectory.makeAbsolute(bs::FileSystem::getWorkingDirectoryPath());

  bs::gDebug().logDebug("[Main] Running REGothEngine");
  bs::gDebug().logDebug("[Main]  - Engine executable: " + engineExecutablePath.toString());
  bs::gDebug().logDebug("[Main]  - Game directory:    " + gameDirectory.toString());

  bs::gDebug().logDebug("[Main] Finding REGoth content-directory");
  regoth.findEngineContent(engineExecutablePath);

  bs::gDebug().logDebug("[Main] Loading original game packages");
  regoth.loadOriginalGamePackages(engineExecutablePath, gameDirectory);

  if (!regoth.hasFoundGameFiles())
  {
    std::cout << "No files loaded into the VDFS - is the datapath correct?" << std::endl;
    return -1;
  }

  regoth.loadCachedResourceManifests();

  bs::gDebug().logDebug("[REGothEngine] Loading Shaders");

  regoth.setShaders();
  regoth.setupInput();

  bs::gDebug().logDebug("[REGothEngine] Setting up Main Camera");

  regoth.setupMainCamera();

  bs::gDebug().logDebug("[REGothEngine] Setting up Scene");

  regoth.setupScene();

  regoth.saveCachedResourceManifests();

  regoth.run();

  regoth.saveCachedResourceManifests();

  regoth.shutdown();

  return 0;
}
