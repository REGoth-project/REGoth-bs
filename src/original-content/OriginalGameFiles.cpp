#include "OriginalGameFiles.hpp"
#include <FileSystem/BsFileSystem.h>
#include <excepction/Throw.hpp>

namespace REGoth
{
  void OriginalGameFiles::setOriginalFilesRoot(const bs::Path& root)
  {
    mRoot = root;

    if (mRoot.getTail() == "")
    {
      
    }

    if (dataDirectory() == bs::Path::BLANK)
    {
      REGOTH_THROW(FileNotFoundException,
                   bs::StringUtil::format(
                       "Game-file directory {0} does not seem to have a 'data'-directory!", mRoot.toString()));
    }

    if (gothicDat() == bs::Path::BLANK)
    {
      REGOTH_THROW(FileNotFoundException,
                   bs::StringUtil::format(
                       "Game-file directory {0} does not seem to contain GOTHIC.DAT", mRoot.toString()));
    }

    if (allVdfsPackages().empty())
    {
      REGOTH_THROW(FileNotFoundException,
                   bs::StringUtil::format(
                     "Game-file directory {0} does not seem to contain any .vdf-files!", mRoot.toString()));
    }
  }

  bs::Path OriginalGameFiles::gothicDat()
  {
    return findCaseSensitivePathOf("_work/Data/Scripts/_compiled/GOTHIC.DAT");
  }

  bs::Path OriginalGameFiles::dataDirectory()
  {
    return findCaseSensitivePathOf("data/");
  }

  bs::Vector<bs::Path> OriginalGameFiles::allVdfsPackages()
  {
    return filterFilesInDirectoryByExt(dataDirectory(), ".vdf");
  }

  bs::Path OriginalGameFiles::findCaseSensitivePathOf(const bs::Path& path)
  {
    bs::Path actual = mRoot;

    for (bs::UINT32 i = 0; i < path.getNumDirectories(); i++)
    {
      actual = appendCaseInsensitiveThing(actual, path.getDirectory(i));

      if (actual == bs::Path::BLANK)
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
                                                         const bs::String& directory)
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
                                                                      const bs::String& ext)
  {
    bs::Vector<bs::Path> files;
    bs::Vector<bs::Path> dirs;

    bs::FileSystem::getChildren(dataDirectory(), files, dirs);

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

  OriginalGameFiles& gOriginalGameFiles()
  {
    static OriginalGameFiles s_Instance;
    return s_Instance;
  }
}  // namespace REGoth
