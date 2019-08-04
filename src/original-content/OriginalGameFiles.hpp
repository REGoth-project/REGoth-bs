#pragma once

#include <BsPrerequisites.h>
#include <FileSystem/BsPath.h>

#include <core/GameType.hpp>

namespace REGoth
{
  /**
   * This class provides easy access to the original game files for all platforms.
   *
   * Since Gothic was developed on Windows-Systems, which uses a case-insensitive
   * file system, there are major differences in file naming between releases.
   * Sometimes a folder is called `Data`, `DATA` or `data`, you get the idea.
   *
   * This is troublesome for UNIX-systems which generally use case-sensitive
   * file systems.
   *
   * Another reason this class exists is so we can abstract away the paths to
   * certain files used by the engine.
   *
   * Generally, all paths you get back are in the correct case for your system.
   */

  class OriginalGameFiles
  {
  public:
    /**
     * Construct a new OriginalGameFiles Object given the root path
     * of the original game.
     *
     * @param  root  Root of the original game file directory,
     *               which contains `system`, `_work` and so on.
     */
    OriginalGameFiles(const bs::Path& root);

    /**
     * @return List of all .vdf-packages found in the `Data`-directory. With their
     *         full paths. This does NOT do a recursive search.
     */
    bs::Vector<bs::Path> allVdfsPackages() const;

    /**
     * @return List of all .mod-Packages found in the `Data/Modvdf`-directory. With their
     *         full paths. This does a recursive search.
     */
    bs::Vector<bs::Path> allModPackages() const;

    /**
     * @return Actual path to the GOTHIC.DAT file
     */
    bs::Path gothicDat() const;

    /**
     * @return Actual path the `_work/Data` directory where files outside
     *         .vdf-packages are stored but should still accessible via VDFS.
     */
    bs::Path vdfsFileEntryPoint() const;

    /**
     * @return Absolute path of the original games `system`-directory.
     */
    bs::Path system() const;

    /**
     * @return Which game REGoth is currently running. (e.g. Gothic I or Gothic II)
     */
    GameType gameType() const;

    /**
     * Given a path that is somewhere in an original gothic installation, this function
     * finds the root-directory of those game files, ie. the one which contains `_work`,
     * `data`, `system`, and so on. If the given path was found to be within a gothic
     * installation, the root of it is returned.
     *
     * If the path was NOT found to be part of a gothic installation, bs::Path::BLANK
     * is returned.
     */
    static bs::Path findGameFilesRoot(const bs::Path& from);

  private:
    /**
     * @return Whether the path given is a somewhat valid gothic installation root directory.
     */
    static bool isGameRoot(const bs::Path& path);

    /**
     * Outputs all files found in the given directory which match the given extension.
     *
     * The comparison is done case-insensitive. Only the given directory is searched,
     * subfolders are ignored.
     *
     * @see    See filterFilesInDirectoryByExtRecursive() for a recursive variant of
     *         this method.
     *
     * @param  path  Path to the directory to search.
     * @param  ext   Extension to search for (with .), e.g. `.vdf`
     *
     * @return List of all files in the given directory with the given extension.
     */
    bs::Vector<bs::Path> filterFilesInDirectoryByExt(const bs::Path& path,
                                                     const bs::String& ext) const;

    /**
     * Outputs all files found in the given directory and all its subdirectories,
     * which match the given extension.
     *
     * The comparison is done case-insensitive.
     *
     * @see    See filterFilesInDirectoryByExt() for a non-recursive variant of this
     *         method.
     *
     * @param  path  Path to the directory to begin searching.
     * @param  ext   Extension to search for (with .), e.g. `.vdf`
     *
     * @return List of all files in the given directory and its subdirectories
     *         with the given extension.
     */
    bs::Vector<bs::Path> filterFilesInDirectoryByExtRecursive(const bs::Path& path,
                                                              const bs::String& ext) const;

    /**
     * @return Actual path to the data-directory
     */
    bs::Path dataDirectory() const;

    /**
     * @return Actual path to the data/modvdf-directory
     */
    bs::Path modDirectory() const;

    /**
     * Given a case-insensitive path, this tries to find the real, case-sensitive path.
     *
     * Should there exist two files with the same name, it is undefined which one you get,
     * so you should avoid that.
     *
     * If the file wasn't found, an empty path is returned.
     *
     * @param  path  Case-insensitive path to look at relative to the game root.
     *               Should have a trailing "/" if the path is pointing to a directory!
     *
     * @return First file that matched the case insensitive path.
     */
    bs::Path findCaseSensitivePathOf(const bs::Path& path) const;

    /**
     * Given a directory and the name of a directory or file, this will try to find
     * the correct cased name in the file system and append it to the path.
     *
     * Throws if the given path already is a file.
     * Returns an empty path if the thing didn't exist.
     *
     * @param  path  Absolute Path to append to. Should already be in the correct case.
     * @param  thing Name of the directory or file to append.
     *
     * @return Input path with the given directory or file appended in the correct case.
     *         bs::Path::BLANK if the given directory or file was not found.
     */
    bs::Path appendCaseInsensitiveThing(const bs::Path& path, const bs::String& thing) const;

    /**
     * Root of the original game directory
     */
    bs::Path mRoot;

    /**
     * Paths to all files stored inside the game directory
     */
    bs::Vector<bs::Path> mAllFiles;

    /**
     * Map which does case-insensitive path to index into mAllFiles,
     * which stores the case-sensitive path.
     */
    bs::Map<bs::Path, bs::UINT32> mCaseMap;
  };

}  // namespace REGoth
