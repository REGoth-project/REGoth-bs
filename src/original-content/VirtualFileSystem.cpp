#include "VirtualFileSystem.hpp"
#include <FileSystem/BsFileSystem.h>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <vdfs/fileIndex.h>

using namespace REGoth;

class REGoth::InternalVirtualFileSystem
{
public:
  InternalVirtualFileSystem()          = default;
  virtual ~InternalVirtualFileSystem() = default;

  VDFS::FileIndex fileIndex;
  bool isFinalized = false;

  bool isReadyToReadFiles()
  {
    if (!isFinalized)
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  void finalizeFileIndex()
  {
    fileIndex.finalizeLoad();
    isFinalized = true;
  }
};

void VirtualFileSystem::setPathToEngineExecutable(const bs::String& argv0)
{
  VDFS::FileIndex::initVDFS(argv0.c_str());

  mInternal = bs::bs_shared_ptr_new<InternalVirtualFileSystem>();
}

void VirtualFileSystem::mountDirectory(const bs::Path& path)
{
  throwOnMissingInternalState();

  if (mInternal->isFinalized)
  {
    REGOTH_THROW(InvalidStateException, "Cannot mount directories on finalized file index.");
  }

  REGOTH_LOG(Info, Uncategorized, "[VDFS] Mounting directory (recursive): {0}", path.toString());

  auto onDirectory = [&](const bs::Path& p) {
    bs::Path relative = p.getRelative(path);
    REGOTH_LOG(Info, Uncategorized, "[VDFS]  - {0}", relative.toString());

    mInternal->fileIndex.mountFolder(p.toString().c_str());

    return true;
  };

  enum
  {
    Recursive    = true,
    NonRecursive = false,
  };

  bs::FileSystem::iterate(path, nullptr, onDirectory, Recursive);
}

bool VirtualFileSystem::loadPackage(const bs::Path& package)
{
  throwOnMissingInternalState();

  if (mInternal->isFinalized)
  {
    REGOTH_THROW(InvalidStateException, "Cannot load packages on finalized file index.");
  }

  return mInternal->fileIndex.loadVDF(package.toString().c_str());
}

bs::Vector<bs::String> VirtualFileSystem::listAllFiles()
{
  std::vector<std::string> allStl = mInternal->fileIndex.getKnownFiles();
  bs::Vector<bs::String> all(allStl.size());

  for (size_t i = 0; i < allStl.size(); i++)
  {
    all[i] = allStl[i].c_str();

    // Internal file index will return the files in the casing they were stored in.
    // To be consistent, convert them all to uppercase here.
    bs::StringUtil::toUpperCase(all[i]);
  }

  return all;
}

bs::Vector<bs::String> REGoth::VirtualFileSystem::listByExtension(const bs::String& ext)
{
  bs::Vector<bs::String> allFilesUpperCase = listAllFiles();

  // Convert extension to UPPERCASE since all files returned by listAllFiles() are also
  // uppercase. That way, we make the extension-parameter case insensitive.
  bs::String extUpper = ext;
  bs::StringUtil::toUpperCase(extUpper);

  enum
  {
    RespectCase = false,
    LowerCase   = true,
  };

  bs::Vector<bs::String> result;
  for (const auto& fileName : allFilesUpperCase)
  {
    // Respect case here since our only option is converting everything to lower case
    // with endsWith(). Since all our input strings are known to be uppercase, we can
    // just compare them without changing cases.
    if (bs::StringUtil::endsWith(fileName, extUpper, RespectCase))
    {
      result.push_back(fileName);
    }
  }

  return result;
}

bs::Vector<bs::UINT8> VirtualFileSystem::readFile(const bs::String& file) const
{
  throwOnMissingInternalState();

  if (!mInternal->isFinalized)
  {
    mInternal->finalizeFileIndex();
  }

  if (!mInternal->isReadyToReadFiles())
  {
    REGOTH_THROW(InvalidStateException, "VDFS is not ready to read files yet.");
  }

  std::vector<uint8_t> stlData;

  mInternal->fileIndex.getFileData(file.c_str(), stlData);

  // FIXME: Need some other way to get the file data so we don't have to copy the data here
  return bs::Vector<bs::UINT8>(stlData.begin(), stlData.end());
}

bool REGoth::VirtualFileSystem::hasFile(const bs::String& file) const
{
  if (!mInternal)
  {
    REGOTH_THROW(InvalidStateException,
                 "VDFS internal state not available, call setPathToEngineExecutable()");
  }

  return mInternal->fileIndex.hasFile(file.c_str());
}

void REGoth::VirtualFileSystem::throwIfFileIsMissing(const bs::String& file,
                                                     const bs::String& message) const
{
  if (!hasFile(file))
  {
    if (message.empty())
    {
      REGOTH_THROW(
          InvalidStateException,
          bs::StringUtil::format("Expected file {0} inside VDFS, but it could not be found!", file));
    }
    else
    {
      REGOTH_THROW(InvalidStateException, message);
    }
  }
}

bool REGoth::VirtualFileSystem::hasFoundGameFiles() const
{
  throwOnMissingInternalState();

  return mInternal->fileIndex.getKnownFiles().size() > 0;
}

const VDFS::FileIndex& VirtualFileSystem::getFileIndex()
{
  throwOnMissingInternalState();

  if (!mInternal->isFinalized)
  {
    mInternal->finalizeFileIndex();
  }

  return mInternal->fileIndex;
}

void VirtualFileSystem::throwOnMissingInternalState() const
{
  if (!mInternal)
  {
    REGOTH_THROW(InvalidStateException,
                 "VDFS internal state not available, call setPathToEngineExecutable()");
  }
}

VirtualFileSystem& REGoth::gVirtualFileSystem()
{
  static VirtualFileSystem vdfs;

  return vdfs;
}
