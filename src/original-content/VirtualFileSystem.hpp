/**
 * \file
 *
 * This module implements a wrapper around ZenLibs Virtual Systen, or short VDFS.
 *
 * A VDFS can be constructed by loading package-files or mounting a real directory on
 * the host file-system. All files found in either the packages or the real directory
 * are then added to a VDFS-FileIndex.
 *
 * To get the data of a file, the FileIndex can be queried. It will resolve where the
 * real file is and load the data from it.
 *
 * See BsZenLib or ZenLib for more information.
 *
 *
 * # Differences to Gothics implementation
 *
 * While Gothics VDFS kept the directory structure inside the VDFS, it did not make use
 * of it at any point. Keeping the directory structure even led to problems with threading,
 * as multiple threads were setting the current working directory at the same time.
 * This caused the infamous "Orc-Axe"-Bug in the temple of the sleeper.
 *
 * To simplify things in REGoth, the directory structure is not preserved inside our
 * implementation of the VDFS. All files gathered into a single list.
 *
 *
 * # Design decisions
 *
 * As in the original and the previous iteration of REGoth, it was found that there
 * is no need to ever have multiple VDFS-FileIndices around. Therefore, the API to
 * the FileIndex REGoth uses acts on one global file index, which saves us from
 * passing the FileIndex-object to everything.
 *
 */

#pragma once

#include <BsPrerequisites.h>
#include <FileSystem/BsPath.h>

namespace VDFS
{
  class FileIndex;
}

namespace REGoth
{
  class InternalVirtualFileSystem;
  class VirtualFileSystem
  {
  public:
    VirtualFileSystem() = default;
    virtual ~VirtualFileSystem() = default;

    /**
     * Before calling anything else, this function must be called.
     *
     * The PhysFS-Library used by our VDFS-implementation wants to know where the program
     * executable currently running is located.
     *
     * @param argv0 First value of the argv array passed to main (Path to executable).
     */
    void setPathToEngineExecutable(const bs::String& argv0);

    /**
     * Loads a package into the global file index.
     *
     * After loading, the files of the given package can be found inside the
     * file index and their data can be obtained using readFile().
     *
     * Also not that you *cannot* load more packages after you have read the
     * first file. Make sure to load all packages first.
     *
     * @param  package  Path of the package to load.
     *
     * @return Whether the package could be loaded.
     */
    bool loadPackage(const bs::Path& package);

    /**
     * Mounts the directory at the given path.
     *
     * The directory-structure itself will be flattened so basically the directory
     * is recursivly searched and all files are added to the index at root level.
     *
     * Throws if the given path does not end in a valid directory or if the VDFS
     * was already finalized (happens on first file read).
     *
     * @param  path  Path of the directory to add.
     */
    void mountDirectory(const bs::Path& path);

    /**
     * Returns a list of all files known to the index.
     *
     * This will go through all packages and assemble a list containing all
     * known files names which one could read using readFile().
     *
     * @return Names of all files known to the index, all UPPERCASE.
     */
    bs::Vector<bs::String> listAllFiles();

    /**
     * Returns a list of files with the given file extension.
     *
     * @param  ext  File extension to look for, with leading dot. E.g. `.3DS`,
     *              case insensitive.
     *
     * @return Names of all files with the given file extension, all UPPERCASE.
     */
    bs::Vector<bs::String> listByExtension(const bs::String& ext);


    /**
     * Reads the contents of a file and returns them as byte array.
     *
     * To load a package, which contains files, see loadPackage().
     *
     * @note   Check with hasFile() whether the file exists within the file index.
     *
     * @param  file  Case-insensitive name of the file found inside a package previously loaded
     *               using loadPackage(), eg. "STONE.3DS".
     *
     * @return The complete data from the given file. Empty if the file does not exist.
     */
    bs::Vector<bs::UINT8> readFile(const bs::String& file) const;

    /**
     * Searches through the file index to see if the given file has been registered
     * inside the file index.
     *
     * See loadPackage() on how to populat the file index with files.
     *
     * @param  file  Case-insensitive name of the file found inside a package previously loaded
     *               using loadPackage(), eg. "STONE.3DS".
     *
     * @return Whether the file is available and can be read using readFile().
     */
    bool hasFile(const bs::String& file) const;

    /**
     * Throws if the given file is missing in the file index.
     *
     * @param  file     File to search for, see hasFile().
     * @param  message  (Optional) Message to append to the exception text.
     */
    void throwIfFileIsMissing(const bs::String& file, const bs::String& message = "") const;

    /**
     * @return Whether (at least some) of the game files were found in the given data directory
     *         after loading packages.
     */
    bool hasFoundGameFiles() const;

    /**
     * Returns a reference to the internal file index for other modules to use (eg. BsZenLib).
     *
     * This will also cause the file index to be finalized, so no new packages can be loaded
     * afterwards.
     */
    const VDFS::FileIndex& getFileIndex();

  private:

    /**
     * Throws an exception if the mInternal-pointer is not set.
     */
    void throwOnMissingInternalState() const;

    bs::SPtr<InternalVirtualFileSystem> mInternal;
  };

  /**
   * Global access to the virtual file system.
   */
  VirtualFileSystem& gVirtualFileSystem();
}
