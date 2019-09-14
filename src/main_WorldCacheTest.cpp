#include <memory>

#include <BsFPSCamera.h>
#include <Components/BsCCamera.h>
#include <Utility/BsTimer.h>

#include <BsZenLib/CacheUtility.hpp>

#include <core.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/VirtualFileSystem.hpp>

class REGothWorldCacheTest : public REGoth::EmptyGame
{
public:
  using REGoth::EmptyGame::EmptyGame;

  void setupScene() override
  {
    using namespace REGoth;

    BsZenLib::CacheWholeVDFS(gVirtualFileSystem().getFileIndex());

    REGOTH_LOG(Info, Uncategorized, "Finished caching");
  }
};

int main(int argc, char** argv)
{
  auto config = REGoth::parseArguments<REGoth::EngineConfig>(argc, argv);
  REGothWorldCacheTest engine{std::move(config)};

  return REGoth::runEngine(engine);
}
