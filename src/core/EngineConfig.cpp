#include <core/EngineConfig.hpp>

#include <FileSystem/BsFileSystem.h>
#include <FileSystem/BsPath.h>

#include <cxxopts.hpp>

#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameFiles.hpp>

using namespace REGoth;

std::stringstream& operator>>(std::stringstream& str, bs::Path& path)
{
  path.assign(bs::Path{str.str().c_str()});
  return str;
}

EngineConfig::~EngineConfig()
{
  // pass
}

void EngineConfig::registerCLIEngineOptions(cxxopts::Options& options)
{
  // Configure positional handling
  options.positional_help("[GAME ASSETS PATH]");
  options.show_positional_help();

  const std::string grp = "Core engine";

  // Define engine options
  options.add_option(grp, "g", "game-assets", "Path to a Gothic or Gothic II installation",
                     cxxopts::value<bs::Path>(originalAssetsPath), "[PATH]");
  options.add_option(grp, "", "video-x-res", "X resolution",
                     cxxopts::value<unsigned int>(resolutionX), "[PX]");
  options.add_option(grp, "", "video-y-res", "Y resolution",
                     cxxopts::value<unsigned int>(resolutionY), "[PX]");
  options.add_option(grp, "", "video-fullscreen", "Run in fullscreen mode",
                     cxxopts::value<bool>(isFullscreen), "");

  // Allow game-assets to also be a positional
  options.parse_positional({"game-assets"});
}

void EngineConfig::verifyCLIEngineOptions()
{
  // Assert that engineExecutablePath is not empty.
  if (engineExecutablePath.isEmpty())
  {
    REGOTH_THROW(InvalidStateException, "Engine executable path could not be determined.");
  }

  // Resolve engineExecutablePath.
  engineExecutablePath.makeAbsolute(bs::FileSystem::getWorkingDirectoryPath());

  // If originalAssetsPath is unset, try to derive possible fallbacks.
  if (originalAssetsPath.isEmpty())
  {
    REGOTH_LOG(Info, Uncategorized, "No game asset path given. Trying executable path...");

    // Use engineExecutablePath as first choice.
    originalAssetsPath = OriginalGameFiles::findGameFilesRoot(engineExecutablePath);

    // If originalAssetsPath is still unset, try current working directory.
    if (originalAssetsPath.isEmpty() &&
        engineExecutablePath != bs::FileSystem::getWorkingDirectoryPath())
    {
      REGOTH_LOG(Info, Uncategorized, "Failed. Trying current working directory...");

      originalAssetsPath =
          OriginalGameFiles::findGameFilesRoot(bs::FileSystem::getWorkingDirectoryPath());

      if (originalAssetsPath.isEmpty())
      {
        REGOTH_LOG(Info, Uncategorized, "Failed. Giving up.");
        REGOTH_THROW(InvalidStateException,
                     "Could not find a game asset folder. Try specifying it with `--game-assets`.");
      }
    }

    REGOTH_LOG(Info, Uncategorized, "Succeded");
  }
  // Otherwise verify and resolve user-supplied input.
  else
  {
    bs::String userInput = originalAssetsPath.toString();

    // Probe user-supplied directory.
    originalAssetsPath = OriginalGameFiles::findGameFilesRoot(originalAssetsPath);

    if (originalAssetsPath.isEmpty())
    {
      REGOTH_THROW(InvalidStateException,
                   "Could not find a Gothic or Gothic II installation at the supplied path `" +
                       userInput + "`.");
    }
  }
}

void EngineConfig::registerCLIOptions(cxxopts::Options& /* options */)
{
  // pass
}

void EngineConfig::verifyCLIOptions()
{
  // pass
}
