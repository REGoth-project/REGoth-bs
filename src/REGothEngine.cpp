#include "REGothEngine.hpp"

#include <cassert>
#include <iostream>
#include <memory>

#include <BsApplication.h>
#include <Components/BsCCamera.h>
#include <FileSystem/BsFileSystem.h>
#include <Importer/BsImporter.h>
#include <Input/BsVirtualInput.h>
#include <Scene/BsSceneObject.h>

#include <BsZenLib/ImportMaterial.hpp>
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ResourceManifest.hpp>

#include <cxxopts.hpp>

#include <engine-content/EngineContent.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameFiles.hpp>
#include <original-content/VirtualFileSystem.hpp>

using namespace REGoth;

std::stringstream& operator>>(std::stringstream& str, bs::Path& path)
{
  path.assign(bs::Path{str.str().c_str()});
  return str;
}

/**
 * Name of REGoth's own content directory
 */
const bs::String REGOTH_CONTENT_DIR_NAME = "content";

REGothEngine::~REGothEngine()
{
}

void REGothEngine::loadGamePackages()
{
  OriginalGameFiles files = OriginalGameFiles(config()->originalAssetsPath);

  gVirtualFileSystem().setPathToEngineExecutable(config()->engineExecutablePath.toString());

  REGOTH_LOG(Info, Uncategorized, "[VDFS] Indexing packages: ");

  for (auto p : files.allVdfsPackages())
  {
    REGOTH_LOG(Info, Uncategorized, "[VDFS]  - {0}", p.getFilename());
    gVirtualFileSystem().loadPackage(p);
  }

  gVirtualFileSystem().mountDirectory(files.vdfsFileEntryPoint());

  loadModPackages(files);
}

void REGothEngine::loadModPackages(const OriginalGameFiles& /* files */)
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

void REGothEngine::findEngineContent()
{
  mEngineContent = bs::bs_shared_ptr_new<EngineContent>(config()->engineExecutablePath);

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

  VideoMode videoMode{config()->resolutionX, config()->resolutionY};
  Application::startUp(videoMode, "REGoth", config()->isFullscreen);
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
  inputConfig->registerAxis("Horizontal", VIRTUAL_AXIS_DESC(static_cast<UINT32>(InputAxis::MouseX)));
  inputConfig->registerAxis("Vertical", VIRTUAL_AXIS_DESC(static_cast<UINT32>(InputAxis::MouseY)));
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

REGothEngineDefaultConfig::REGothEngineDefaultConfig(std::unique_ptr<const EngineConfig>&& config)
    : mConfig{std::move(config)}
{
  // pass
}

const EngineConfig* REGothEngineDefaultConfig::config() const
{
  return mConfig.get();
}

int ::REGoth::runEngine(REGothEngine& engine)
{
  engine.initializeBsf();

  REGOTH_LOG(Info, Uncategorized, "[Main] Running REGothEngine");
  REGOTH_LOG(Info, Uncategorized, "[Main]  - Engine executable: {0}",
             engine.config()->engineExecutablePath.toString());
  REGOTH_LOG(Info, Uncategorized, "[Main]  - Game directory:    {0}",
             engine.config()->originalAssetsPath.toString());

  REGOTH_LOG(Info, Uncategorized, "[Main] Finding REGoth content-directory");
  engine.findEngineContent();

  REGOTH_LOG(Info, Uncategorized, "[Main] Loading original game packages");
  engine.loadGamePackages();

  if (!engine.hasFoundGameFiles())
  {
    REGOTH_LOG(Fatal, Uncategorized,
               "No files loaded into the VDFS - is the game assets path correct?");
    return EXIT_FAILURE;
  }

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Load cached resource manifests");
  engine.loadCachedResourceManifests();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Loading Shaders");
  engine.setShaders();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Setting up input");
  engine.setupInput();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Setting up Main Camera");
  engine.setupMainCamera();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Setting up Scene");
  engine.setupScene();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Save cached resource manifests");
  engine.saveCachedResourceManifests();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Run");
  engine.run();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Save cached resource manifests");
  engine.saveCachedResourceManifests();

  REGOTH_LOG(Info, Uncategorized, "[REGothEngine] Shutdown");
  engine.shutdown();

  return EXIT_SUCCESS;
}
