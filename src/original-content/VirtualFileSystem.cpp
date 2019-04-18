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

bool VirtualFileSystem::loadPackage(const bs::Path& package)
{
  using namespace bs;

  throwOnMissingInternalState();

  if (mInternal->isFinalized)
  {
    BS_EXCEPT(InvalidStateException, "Cannot load packages on finalized file index.");
  }

  return mInternal->fileIndex.loadVDF(package.toString().c_str());
}

bs::Vector<bs::String> VirtualFileSystem::listAllFiles()
{
  std::vector<std::string> allStl = mInternal->fileIndex.getKnownFiles();
  bs::Vector<bs::String> all(allStl.size());

  for(size_t i = 0; i < allStl.size(); i++)
  {
    all[i] = allStl[i].c_str();
  }

  return all;
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
