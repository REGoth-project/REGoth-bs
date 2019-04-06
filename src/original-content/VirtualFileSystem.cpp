#include "VirtualFileSystem.hpp"
#include <Error/BsException.h>
#include <FileSystem/BsFileSystem.h>
#include <vdfs/fileIndex.h>

using namespace REGoth;

class REGoth::InternalVirtualFileSystem
{
public:
  InternalVirtualFileSystem() = default;
  virtual ~InternalVirtualFileSystem() = default;

  bs::Path gameDirectory;
  VDFS::FileIndex fileIndex;
  bool isFinalized = false;

  bool isReadyToLoadPackages()
  {
    if (gameDirectory.isEmpty())
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  bool isReadyToReadFiles()
  {
    if (!isReadyToLoadPackages())
    {
      return false;
    }
    else if (!isFinalized)
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  bs::Path fullPackagePath(const bs::String& package)
  {
    return findCaseSensitivePath(gameDirectory + "Data" + package);
  }

  bs::Path findCaseSensitivePath(const bs::Path& path)
  {
    // TODO: Implement findCaseSensitivePath()!
    return path;
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

void VirtualFileSystem::setGameDirectory(const bs::Path& gameDirectory)
{
  using namespace bs;

  throwOnMissingInternalState();

  mInternal->gameDirectory = gameDirectory;
}

bool VirtualFileSystem::isPackageAvailable(const bs::String& package) const
{
  using namespace bs;

  throwOnMissingInternalState();

  auto fullPackagePath = mInternal->fullPackagePath(package);

  return bs::FileSystem::isFile(fullPackagePath);
}

bool VirtualFileSystem::loadPackage(const bs::String& package)
{
  using namespace bs;

  throwOnMissingInternalState();

  if (!mInternal->isReadyToLoadPackages())
  {
    BS_EXCEPT(InvalidStateException, "VDFS is not ready to load packages yet.");
  }

  if (mInternal->isFinalized)
  {
    BS_EXCEPT(InvalidStateException, "Cannot load packages on finalized file index.");
  }

  if (!isPackageAvailable(package))
  {
    return false;
  }

  auto fullPackagePath = mInternal->fullPackagePath(package);

  mInternal->fileIndex.loadVDF(fullPackagePath.toPlatformString().c_str());

  return true;
}

bs::Vector<bs::UINT8> VirtualFileSystem::readFile(const bs::String& file) const
{
  using namespace bs;

  throwOnMissingInternalState();

  if (!mInternal->isFinalized)
  {
    mInternal->finalizeFileIndex();
  }

  if (!mInternal->isReadyToReadFiles())
  {
    BS_EXCEPT(InvalidStateException, "VDFS is not ready to read files yet.");
  }

  std::vector<uint8_t> stlData;

  mInternal->fileIndex.getFileData(file.c_str(), stlData);

  // FIXME: Need some other way to get the file data so we don't have to copy the data here
  return bs::Vector<bs::UINT8>(stlData.begin(), stlData.end());
}

bool REGoth::VirtualFileSystem::hasFile(const bs::String& file) const
{
  using namespace bs;

  if (!mInternal)
  {
    BS_EXCEPT(InvalidStateException,
              "VDFS internal state not available, call setPathToEngineExecutable()");
  }

  return mInternal->fileIndex.hasFile(file.c_str());
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
  using namespace bs;

  if (!mInternal)
  {
    BS_EXCEPT(InvalidStateException,
              "VDFS internal state not available, call setPathToEngineExecutable()");
  }
}

VirtualFileSystem& REGoth::gVirtualFileSystem()
{
  static VirtualFileSystem vdfs;

  return vdfs;
}
