#include "FindEngineContent.hpp"
#include <FileSystem/BsFileSystem.h>

const bs::String REGOTH_CONTENT_DIR_NAME = "content";

namespace REGoth
{
  namespace Internal
  {
    static bs::Path findEngineContentNextToExecutable(const bs::Path& executableDir);
    static bs::Path findEngineContentAtWorkingDirectory();
    static bs::Path findEngineContentUpFromExecutable(const bs::Path& executableDir);
    static bool isContentDirectory(const bs::Path& path);

    bs::Path findEngineContentLocation(const bs::Path& executablePath)
    {
      bs::Path result;

      BS_LOG(Info, Uncategorized, "[FindEngineContent] Trying current working directory");
      result = findEngineContentAtWorkingDirectory();

      if (result != bs::Path::BLANK) return result;

      // Most of the time, this should be equal to the current working directory
      // but it may be not, so check here again...
      BS_LOG(Info, Uncategorized, "[FindEngineContent] Trying executable path");

      result = findEngineContentNextToExecutable(executablePath.getDirectory());

      if (result != bs::Path::BLANK) return result;

      // We could be also running inside the repositry, so search upwards
      BS_LOG(Info, Uncategorized, "[FindEngineContent] Trying to search the directory tree upwards");

      result = findEngineContentUpFromExecutable(executablePath.getDirectory());

      if (result != bs::Path::BLANK) return result;

      return bs::Path::BLANK;
    }

    static bs::Path findEngineContentNextToExecutable(const bs::Path& executableDir)
    {
      if (!isContentDirectory(executableDir + REGOTH_CONTENT_DIR_NAME)) return bs::Path::BLANK;

      return executableDir + REGOTH_CONTENT_DIR_NAME;
    }

    static bs::Path findEngineContentAtWorkingDirectory()
    {
      if (!isContentDirectory(bs::FileSystem::getWorkingDirectoryPath() + REGOTH_CONTENT_DIR_NAME))
        return bs::Path::BLANK;

      return bs::FileSystem::getWorkingDirectoryPath() + REGOTH_CONTENT_DIR_NAME;
    }

    /**
     * Tries to find REGoth's `content` directory by going up the directory tree
     * from the executable location.
     *
     * @return Path to the `content`-dir, or BLANK if not found.
     */
    static bs::Path findEngineContentUpFromExecutable(const bs::Path& executableDir)
    {
      bs::Path cwd = executableDir;

      for (bs::UINT32 i = 0; i < cwd.getNumDirectories(); i++)
      {
        cwd += "..";

        if (isContentDirectory(cwd + REGOTH_CONTENT_DIR_NAME))
        {
          return cwd + REGOTH_CONTENT_DIR_NAME;
        }
      }

      return bs::Path::BLANK;
    }

    static bool isContentDirectory(const bs::Path& path)
    {
      BS_LOG(Info, Uncategorized, "[FindEngineContent]   Looking at: " + path.toString());

      if (path.getTail() != REGOTH_CONTENT_DIR_NAME) return false;
      if (!bs::FileSystem::isDirectory(path)) return false;

      return true;
    }
  }  // namespace Internal
}  // namespace REGoth
