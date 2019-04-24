#include "VirtualFileSystem.hpp"
#include <FileSystem/BsFileSystem.h>
#include <excepction/Throw.hpp>
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

  bs::gDebug().logDebug("[VDFS] Mounting directory: " + path.toString() + " (recursive):");

  auto onDirectory = [&](const bs::Path& p) {
    bs::Path relative = p.getRelative(path);
    bs::gDebug().logDebug("[VDFS]  - " + relative.toString());

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
  }

  return all;
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
