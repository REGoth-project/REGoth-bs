#pragma once

namespace REGoth
{
  enum class GameType
  {
    /**
     * Used if a Gothic.exe was detected.
     */
    Gothic1,

    /**
     * Used if a Gothic2.exe was detected.
     */
    Gothic2,

    /**
     * Used if the game files don't contain Gothic.exe or Gothic2.exe.
     */
    Unidentified,
  };
}
