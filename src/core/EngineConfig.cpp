#include <core/EngineConfig.hpp>

#include <FileSystem/BsFileSystem.h>
#include <FileSystem/BsPath.h>

#include <cxxopts.hpp>

#include <components/Sky.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameFiles.hpp>

using namespace REGoth;

std::stringstream& bs::operator>>(std::stringstream& str, bs::Path& path)
{
  path.assign(bs::Path{str.str().c_str()});
  return str;
}

std::stringstream& REGoth::operator>>(std::stringstream& str, Sky::RenderMode& renderMode)
{
  bs::String mode{str.str().c_str()};
  bs::StringUtil::toLowerCase(mode);
  if (mode == "plane")
  {
    renderMode = Sky::RenderMode::Plane;
  }
  else if (mode == "dome")
  {
    renderMode = Sky::RenderMode::Dome;
  }
  else
  {
    REGOTH_THROW(InvalidParametersException, "Sky render mode cannot be \"" + mode +
                                                 "\".  Possible values: \"plane\", \"dome\".");
  }
  return str;
}

EngineConfig::~EngineConfig()
{
  // pass
}

void EngineConfig::registerCLIEngineOptions(cxxopts::Options& options)
{
  // Configure positional handling.
  options.positional_help("[GAME ASSETS PATH]");
  options.show_positional_help();

  // Define core options.
  options.add_option("", "g", "game-assets",
                     "Path to a Gothic or Gothic II installation.  Can also be the first positional "
                     "argument",
                     cxxopts::value<bs::Path>(originalAssetsPath), "[GAME ASSETS PATH]");

  // Video options.
  const std::string vidgrp = "Video";
  options.add_option(vidgrp, "", "video-x-res", "X resolution",
                     cxxopts::value<unsigned int>(resolutionX), "[PX]");
  options.add_option(vidgrp, "", "video-y-res", "Y resolution",
                     cxxopts::value<unsigned int>(resolutionY), "[PX]");
  options.add_option(vidgrp, "", "video-fullscreen", "If set, the game runs in fullscreen mode",
                     cxxopts::value<bool>(isFullscreen), "");
  options.add_option(vidgrp, "", "video-sky-mode",
                     "Sky render mode, either \"plane\" or \"dome\".  Note: \"dome\" can only be "
                     "used in Gothic II",
                     cxxopts::value<Sky::RenderMode>(skyRenderMode), "[plane|dome]");

  // Allow game-assets to also be a positional.
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

  // Now that originalAssetsPath is determined, try to derive the game type.
  gameType = OriginalGameFiles{originalAssetsPath}.gameType();

  // In Gothic 1, the sky render mode cannot be "dome".
  if (gameType == GameType::Gothic1 && skyRenderMode == Sky::RenderMode::Dome)
  {
    REGOTH_THROW(InvalidStateException, "Only Gothic II supports the \"dome\" sky render mode.");
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
