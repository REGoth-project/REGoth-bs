#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  namespace Internal
  {
    /**
     * Find the location of REGoths own `content`-directory.
     *
     * This function will search for a directory named `content` at the following paths in the
     * following order:
     *
     *  - Current Working Directory
     *  - Directory where the REGoth executable is
     *  - All directories up the tree from the REGoth executable
     *
     * @param  executablePath  Path to the currently running REGoth-executable.
     *                         Can be retrieved from argv[0].
     *
     * @return Path to REGoth's `content`-directory, e.g. `/home/nameless/REGoth/content/`.
     *         BLANK if not found.
     */
    bs::Path findEngineContentLocation(const bs::Path& executablePath);
  }  // namespace Internal
}  // namespace REGoth
