#include <core/RunEngine.hpp>

#include <iostream>
#include <string>

#include <BsPrerequisites.h>
#include <FileSystem/BsPath.h>

#include <core/Engine.hpp>
#include <log/logging.hpp>

using namespace REGoth;

int REGoth::runEngine(Engine& engine)
{
  engine.initializeBsf();

  REGOTH_LOG(Info, Uncategorized, "[Main] Running Engine");
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

  REGOTH_LOG(Info, Uncategorized, "[Engine] Load cached resource manifests");
  engine.loadCachedResourceManifests();

  REGOTH_LOG(Info, Uncategorized, "[Engine] Loading Shaders");
  engine.setShaders();

  REGOTH_LOG(Info, Uncategorized, "[Engine] Caching original resources");
  engine.populateResourceCache();

  REGOTH_LOG(Info, Uncategorized, "[Engine] Setting up input");
  engine.setupInput();

  REGOTH_LOG(Info, Uncategorized, "[Engine] Setting up Main Camera");
  engine.setupMainCamera();

  REGOTH_LOG(Info, Uncategorized, "[Engine] Setting up Scene");
  engine.setupScene();

  REGOTH_LOG(Info, Uncategorized, "[Engine] Save cached resource manifests");
  engine.saveCachedResourceManifests();

  REGOTH_LOG(Info, Uncategorized, "[Engine] Run");
  engine.run();

  REGOTH_LOG(Info, Uncategorized, "[Engine] Save cached resource manifests");
  engine.saveCachedResourceManifests();

  REGOTH_LOG(Info, Uncategorized, "[Engine] Shutdown");
  engine.shutdown();

  return EXIT_SUCCESS;
}
