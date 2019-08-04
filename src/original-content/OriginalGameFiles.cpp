#include "OriginalGameFiles.hpp"
#include <FileSystem/BsFileSystem.h>
#include <exception/Throw.hpp>

namespace REGoth
{
  OriginalGameFiles::OriginalGameFiles(const bs::Path& root)
      : mRoot(root)
  {
    if (vdfsFileEntryPoint().isEmpty())
    {
      REGOTH_THROW(FileNotFoundException,
                   bs::StringUtil::format(
                       "Game-file directory {0} does not seem to have a '_work/data'-directory!",
                       mRoot.toString()));
    }

    if (dataDirectory().isEmpty())
    {
      REGOTH_THROW(
          FileNotFoundException,
          bs::StringUtil::format("Game-file directory {0} does not seem to have a 'data'-directory!",
                                 mRoot.toString()));
    }

    if (gothicDat().isEmpty())
    {
      REGOTH_THROW(
          FileNotFoundException,
          bs::StringUtil::format("Game-file directory {0} does not seem to contain GOTHIC.DAT",
                                 mRoot.toString()));
    }

    if (allVdfsPackages().empty())
    {
      REGOTH_THROW(
          FileNotFoundException,
          bs::StringUtil::format("Game-file directory {0} does not seem to contain any .vdf-files!",
                                 mRoot.toString()));
    }
  }

  bs::Path OriginalGameFiles::gothicDat() const
  {
    return findCaseSensitivePathOf("_work/Data/Scripts/_compiled/GOTHIC.DAT");
  }

  bs::Path OriginalGameFiles::dataDirectory() const
  {
    return findCaseSensitivePathOf("data/");
  }

  bs::Path OriginalGameFiles::modDirectory() const
  {
    return findCaseSensitivePathOf("data/modvdf/");
  }

  bs::Path OriginalGameFiles::vdfsFileEntryPoint() const
  {
    return findCaseSensitivePathOf("_work/data/");
  }

  bs::Vector<bs::Path> OriginalGameFiles::allVdfsPackages() const
  {
    return filterFilesInDirectoryByExt(dataDirectory(), ".vdf");
  }

  bs::Vector<bs::Path> OriginalGameFiles::allModPackages() const
  {
    return filterFilesInDirectoryByExtRecursive(modDirectory(), ".mod");
  }

  bs::Path OriginalGameFiles::system() const
  {
    return findCaseSensitivePathOf("system/");
  }

  GameType OriginalGameFiles::gameType() const
  {
    if (!findCaseSensitivePathOf("system/gothic.exe").isEmpty())
    {
      return GameType::Gothic1;
    }
    else if (!findCaseSensitivePathOf("system/gothic2.exe").isEmpty())
    {
      return GameType::Gothic2;
    }
    else
    {
      return GameType::Unidentified;
    }
  }

  bool OriginalGameFiles::isGameRoot(const bs::Path& path)
  {
    if (!bs::FileSystem::isDirectory(path)) return false;

    bs::Vector<bs::Path> files;
    bs::Vector<bs::Path> dirs;

    bs::FileSystem::getChildren(path, files, dirs);

    bool foundData   = false;
    bool foundSystem = false;
    bool foundWork   = false;

    for (const bs::Path& p : dirs)
    {
      bs::String lowercaseDir = p.getTail();
      bs::StringUtil::toLowerCase(lowercaseDir);

      if (lowercaseDir == "_work") foundWork = true;
      if (lowercaseDir == "system") foundSystem = true;
      if (lowercaseDir == "data") foundData = true;
    }

    if (!foundData) return false;
    if (!foundSystem) return false;
    if (!foundWork) return false;

    return true;
  }

  bs::Path OriginalGameFiles::findGameFilesRoot(const bs::Path& from)
  {
    bs::Path cwd = from;

    if (isGameRoot(cwd))
    {
      return cwd.makeAbsolute(from);
    }

    for (bs::UINT32 i = 0; i < cwd.getNumDirectories(); i++)
    {
      cwd = cwd.getParent();

      if (isGameRoot(cwd))
      {
        return cwd.makeAbsolute(from);
      }
    }

    return bs::Path::BLANK;
  }

  bs::Path OriginalGameFiles::findCaseSensitivePathOf(const bs::Path& path) const
  {
    bs::Path actual = mRoot;

    for (bs::UINT32 i = 0; i < path.getNumDirectories(); i++)
    {
      actual = appendCaseInsensitiveThing(actual, path.getDirectory(i));

      if (actual.isEmpty())
      {
        return bs::Path::BLANK;
      }
    }

    if (path.isFile())
    {
      return appendCaseInsensitiveThing(actual, path.getFilename());
    }
    else
    {
      return actual;
    }
  }

  bs::Path OriginalGameFiles::appendCaseInsensitiveThing(const bs::Path& path,
                                                         const bs::String& directory) const
  {
    bs::Vector<bs::Path> files;
    bs::Vector<bs::Path> dirs;

    bs::FileSystem::getChildren(path, files, dirs);

    enum
    {
      RespectCase = true,
      IgnoreCase  = false,
    };

    for (const bs::Path& p : dirs)
    {
      if (bs::StringUtil::compare(p.getTail(), directory, IgnoreCase) == 0)
      {
        return p;
      }
    }

    for (const bs::Path& p : files)
    {
      if (bs::StringUtil::compare(p.getTail(), directory, IgnoreCase) == 0)
      {
        return p;
      }
    }

    return bs::Path::BLANK;
  }

  bs::Vector<bs::Path> OriginalGameFiles::filterFilesInDirectoryByExt(const bs::Path& path,
                                                                      const bs::String& ext) const
  {
    bs::Vector<bs::Path> files;
    bs::Vector<bs::Path> dirs;

    bs::FileSystem::getChildren(path, files, dirs);

    enum
    {
      RespectCase = true,
      IgnoreCase  = false,
    };

    bs::Vector<bs::Path> matching;
    for (const bs::Path& p : files)
    {
      if (bs::StringUtil::compare(p.getExtension(), ext, IgnoreCase) == 0)
      {
        matching.push_back(p);
      }
    }

    return matching;
  }

  bs::Vector<bs::Path> OriginalGameFiles::filterFilesInDirectoryByExtRecursive(
      const bs::Path& path, const bs::String& ext) const
  {
    bs::Vector<bs::Path> files;
    bs::Vector<bs::Path> dirs;

    bs::FileSystem::getChildren(path, files, dirs);

    bs::Vector<bs::Path> matching = filterFilesInDirectoryByExt(path, ext);

    for (const bs::Path& p : dirs)
    {
      bs::Vector<bs::Path> matchingSubdir = filterFilesInDirectoryByExtRecursive(p, ext);

      matching.insert(matching.end(), matchingSubdir.begin(), matchingSubdir.end());
    }

    return matching;
  }
}  // namespace REGoth
