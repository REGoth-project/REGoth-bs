#include <core/Engine.hpp>

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
#include <original-content/OriginalGameResources.hpp>
#include <original-content/VirtualFileSystem.hpp>

using namespace REGoth;

/**
 * Name of REGoth's own content directory
 */
const bs::String REGOTH_CONTENT_DIR_NAME = "content";

Engine::~Engine()
{
  // pass
}

void Engine::loadGamePackages()
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

void Engine::loadModPackages(const OriginalGameFiles& /* files */)
{
  // Don't load mod-files by defaults
}

void Engine::saveCachedResourceManifests()
{
  REGOTH_LOG(Info, Uncategorized, "[Engine] Saving resource manifests:");

  REGOTH_LOG(Info, Uncategorized, "[Engine]   - Gothic Cache");
  BsZenLib::SaveResourceManifest();

  // Engine-Content manifest is saved after every resource load since
  // there are only a few resources to handle. If that ever takes too long
  // the manifest should be saved here.
}

bool Engine::hasFoundGameFiles()
{
  return gVirtualFileSystem().hasFoundGameFiles();
}

void Engine::findEngineContent()
{
  mEngineContent = bs::bs_shared_ptr_new<EngineContent>(config()->engineExecutablePath);

  if (!mEngineContent->hasFoundContentDirectory())
  {
    REGOTH_THROW(InvalidStateException, "Did not find REGoth content directory!");
  }

  REGOTH_LOG(Info, Uncategorized, "[Engine] Found REGoth-content directory at: {0}",
             mEngineContent->contentPath().toString());
}

void Engine::initializeBsf()
{
  using namespace bs;

  VideoMode videoMode{config()->resolutionX, config()->resolutionY};
  Application::startUp(videoMode, "REGoth", config()->isFullscreen);
}

void Engine::loadCachedResourceManifests()
{
  using namespace bs;

  REGOTH_LOG(Info, Uncategorized, "[Engine] Loading cached resource manifests");

  if (!mEngineContent)
  {
    REGOTH_THROW(InvalidStateException,
                 "Engine Content not initialized, has findEngineContent() been called?");
  }

  REGOTH_LOG(Info, Uncategorized, "[Engine]   - REGoth Assets");
  mEngineContent->loadResourceManifest();

  REGOTH_LOG(Info, Uncategorized, "[Engine]   - Original Gothic Assets");
  BsZenLib::LoadResourceManifest();
}

void REGoth::Engine::populateResourceCache()
{
  OriginalGameResources::populateCache();
}

void Engine::setupInput()
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

void Engine::setupMainCamera()
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

void Engine::setShaders()
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

void Engine::run()
{
  // FIXME: This is a workaround for the camera not being able to move caused by
  //        making bs::SceneManager::findComponent work in the zen loader.
  mMainCamera->SO()->setActive(false);
  mMainCamera->SO()->setActive(true);

  REGOTH_LOG(Info, Uncategorized, "[Engine] Running mainloop now!");

  bs::Application::instance().runMainLoop();
}

void Engine::shutdown()
{
  if (bs::Application::isStarted())
  {
    REGOTH_LOG(Info, Uncategorized, "[Engine] Shutting down bs::f");

    bs::Application::shutDown();
  }
  else
  {
    REGOTH_LOG(Warning, Uncategorized,
               "[Engine] Received shutdown request, but bs::f is not running!");
  }
}
