#pragma once
#include <BsPrerequisites.h>
#include <FileSystem/BsPath.h>

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
    OriginalGameFiles() = default;

    /**
     * Set where the original files can be found. This must be set
     * before the other methods will return meaningful results.
     *
     * @param  root  Root of the original game file directory,
     *               which contains `system`, `_work` and so on.
     */
    void setOriginalFilesRoot(const bs::Path& root);

    /**
     * @return List of all .vdf-packages found in the `Data`-directory.
     */
    bs::Vector<bs::Path> allVdfsPackages() const;

    /**
     * @return Actual path to the GOTHIC.DAT file
     */
    bs::Path gothicDat() const;

    /**
     * @return Actual path the `_work/Data` directory where files outside
     *         .vdf-packages are stored but should still accessible via VDFS.
     */
    bs::Path vdfsFileEntryPoint() const;

  private:

    /**
     * Outputs all files found in the given directory which match the given extension.
     *
     * The comparison is done case-insensitive.
     *
     * @param  path  Path to the directory to search.
     * @param  ext   Extension to search for (with .), e.g. `.vdf`
     *
     * @return List of all files in the given directory with the given extension.
     */
    bs::Vector<bs::Path> filterFilesInDirectoryByExt(const bs::Path& path, const bs::String& ext) const;

    /**
     * @return Actual path to the data-directory
     */
    bs::Path dataDirectory() const;

    /**
     * Given a case-insensitive path, this tries to find the real, case-sensitive path.
     *
     * Should there exist two files with the same name, it is undefined which one you get,
     * so you should avoid that.
     *
     * If the file wasn't found, an empty path is returned.
     *
     * @param  path  Case-insensitive path to look at relative to the game root.
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

  /**
   * Access to the original game files.
   */
  OriginalGameFiles& gOriginalGameFiles();

}  // namespace REGoth
