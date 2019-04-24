/** \file
 * Entry point for REGoth
 */

#include <string>
#include "BsApplication.h"
#include "BsFPSCamera.h"
#include "BsFPSWalker.h"
#include "REGothEngine.hpp"
#include <assert.h>
#include "components/VisualCharacter.hpp"
#include "original-content/VirtualFileSystem.hpp"
#include <BsZenLib/ImportAnimation.hpp>
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ImportSkeletalMesh.hpp>
#include <BsZenLib/ImportZEN.hpp>
#include <Components/BsCCamera.h>
#include <Components/BsCCharacterController.h>
#include <Components/BsCLight.h>
#include <Components/BsCRenderable.h>
#include <FileSystem/BsFileSystem.h>
#include <GUI/BsCGUIWidget.h>
#include <GUI/BsGUIPanel.h>
#include <GUI/BsGUISlider.h>
#include <Input/BsVirtualInput.h>
#include <Resources/BsBuiltinResources.h>
#include <Resources/BsResourceManifest.h>
#include <Resources/BsResources.h>
#include <Scene/BsPrefab.h>
#include <Scene/BsSceneObject.h>

int main(int argc, char** argv)
{
  using namespace bs;

  if (argc < 3)
  {
    std::cout << "Usage: zen-bs <path/to/gothic/data> <zenfile.zen>" << std::endl;
    return -1;
  }

  REGoth::REGothEngine regoth;

  regoth.initializeBsf();
  regoth.setupInput();

  gDebug().logDebug("[Main] Starting REGoth");

  const String engineExecutablePath = argv[0];
  const String gameDirectory = argv[1];
  const String zenFile = argv[2];

  gDebug().logDebug("[Main]  - Engine executable: " + engineExecutablePath);
  gDebug().logDebug("[Main]  - Game directory:    " + gameDirectory);
  gDebug().logDebug("[Main]  - Startup ZEN:       " + zenFile);

  gDebug().logDebug("[Main] Loading original game packages");
  regoth.loadOriginalGamePackages(engineExecutablePath, gameDirectory);

  if (!regoth.hasFoundGameFiles())
  {
    std::cout << "No files loaded into the VDFS - is the datapath correct?" << std::endl;
    return -1;
  }

  regoth.loadCachedResourceManifests();

  gDebug().logDebug("[Main] Setting up scene");

  // Add a scene object containing a camera component
  HSceneObject sceneCameraSO = SceneObject::create("SceneCamera");
  HCamera sceneCamera = sceneCameraSO->addComponent<CCamera>();
  sceneCamera->setMain(true);
  sceneCamera->setMSAACount(1);

  HSceneObject playerSO = SceneObject::create("Player");
  REGoth::HVisualCharacter playerVisual = playerSO->addComponent<REGoth::VisualCharacter>();

  // Load a model and its animations
  BsZenLib::Res::HModelScriptFile model;

  const String file = "HUMANS.MDS";
  const String visual = "HUM_BODY_NAKED0.ASC";

  if (BsZenLib::HasCachedMDS(file))
  {
    model = BsZenLib::LoadCachedMDS(file);
  }
  else
  {
    model = BsZenLib::ImportAndCacheMDS(file, REGoth::gVirtualFileSystem().getFileIndex());
  }

  if (!model || model->getMeshes().empty())
  {
    gDebug().logError("Failed to load model or animations: " + file + "/" + visual);
    return -1;
  }

  gDebug().logDebug("Num Meshes: " + bs::toString(model->getMeshes().size()));

  for (const auto& h : model->getMeshes())
  {
    // h.blockUntilLoaded();
    gDebug().logDebug(h->getName());
  }

  playerVisual->setModelScript(model);
  playerVisual->setMesh(model->getMeshes()[0]);
  // playerSO->setScale(Vector3(0.01f, 0.01f, 0.01f)); // FIXME: Scale should be done when importing
  // the mesh

  // Disable some fancy rendering
  auto rs = sceneCamera->getRenderSettings();

  rs->screenSpaceReflections.enabled = false;
  rs->ambientOcclusion.enabled = false;
  rs->enableIndirectLighting = true;
  rs->enableFXAA = false;
  rs->enableHDR = false;
  rs->enableTonemapping = false;

  sceneCamera->setRenderSettings(rs);

  HSceneObject worldSO = {};

  // Import a Gothic ZEN
  if (BsZenLib::HasCachedZEN(zenFile))
  {
    HPrefab worldPrefab = BsZenLib::LoadCachedZEN(zenFile);

    if (!worldPrefab)
    {
      gDebug().logError("Failed to load cached ZEN: " + zenFile);
      return -1;
    }

    worldPrefab.blockUntilLoaded();

    worldSO = worldPrefab->instantiate();
  }
  else
  {
    worldSO =
        BsZenLib::ImportAndCacheZEN(zenFile.c_str(), REGoth::gVirtualFileSystem().getFileIndex());

    if (!worldSO)
    {
      gDebug().logError("Failed to load uncached ZEN: " + zenFile);
      return -1;
    }
  }

  HSceneObject startSO = worldSO->findChild("zCVobStartpoint:zCVob");

  Vector3 startPosition = startSO ? startSO->getTransform().getPosition() : Vector3(bs::BsZero);
  Vector3 startLookAt = startSO ? startSO->getTransform().getForward() : Vector3(1.0, 0.0, 0.0);

  // Position the camera
  sceneCameraSO->setParent(playerSO);

  HCharacterController charController = playerSO->addComponent<CCharacterController>();
  playerSO->addComponent<FPSWalker>();

  // Make the character about 1.8m high, with 0.4m radius (controller represents a capsule)
  charController->setHeight(1.0f);  // + 0.4 * 2 radius = 1.8m height
  charController->setRadius(0.4f);

  HFPSCamera fpsCamera = sceneCameraSO->addComponent<FPSCamera>();
  fpsCamera->setCharacter(playerSO);

  sceneCameraSO->setPosition(Vector3(0.0f, 1.8f * 0.5f - 0.1f, 2.0));

  playerSO->setPosition(startPosition);
  // playerSO->lookAt(startLookAt);

  // setupInputConfig();

  // Add GUI
  HSceneObject guiSO = SceneObject::create("GUI");

  float guiScale = 1.0f;
  guiSO->setScale(Vector3(guiScale, guiScale, guiScale));
  HGUIWidget gui = guiSO->addComponent<CGUIWidget>(sceneCamera);
  gui->setSkin(BuiltinResources::instance().getGUISkin());

  GUIPanel* mainPanel = gui->getPanel();

  // Add draw distance slider
  GUISlider* drawDistanceSlider = mainPanel->addNewElement<GUISliderHorz>();

  drawDistanceSlider->setRange(1.0f, 100.0f);
  drawDistanceSlider->setPosition(5, 5);
  drawDistanceSlider->setWidth(200);

  drawDistanceSlider->onChanged.connect([&](float percent) {
    float value = drawDistanceSlider->getValue();

    auto rs = sceneCamera->getRenderSettings();

    rs->cullDistance = value;
    gDebug().logDebug("Set CullDistance to: " + toString(value));

    sceneCamera->setRenderSettings(rs);
  });

  regoth.run();

  return 0;
}
