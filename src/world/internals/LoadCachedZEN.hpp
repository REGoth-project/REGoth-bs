#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  namespace World
  {
    /**
     * Loads the cached ZEN with the given name.
     *
     * To create a cached ZEN, see importAndCacheZEN();
     *
     * A cached ZEN is just a fancy name for a scene stored in the format
     * of bs:f. To create one, the original ZEN-file is read and matching
     * scene objects are created for every vob. The resulting scene structure
     * is then saved as bs:f-scene so it can be loaded later.
     *
     * Loading worlds from cache is generally much faster than loading from
     * the original ZEN-file.
     *
     * Throws if there is no cache for the given ZEN-file.
     *
     * @param  zenFile  Name of the ZEN to load, e.g. `NEWWORLD.ZEN`.
     *
     * @return Scene root of the loaded ZEN.
     */
    bs::HSceneObject loadCachedZEN(const bs::String& zenFile);

    /**
     * This will the given scene-root as cache for the given ZEN-file.
     *
     * You most likely want to cache a ZEN in its starting configuration,
     * so with init-scripts already ran but you don't have to. Just be
     * careful of the scene looks that you are saving.
     *
     * @param  root     Root of the ZEN to cache.
     * @param  zenFile  Name of the ZEN to save for, e.g. `NEWWORLD.ZEN`.
     *                  This should be a valid ZEN-file name!
     */
    void saveCacheForZEN(bs::HSceneObject root, const bs::String& zenFile);

    /**
     * @return Whether a cache exists for the given zenFile.
     *
     * @See saveCacheForZEN() to create a cache.
     * @See loadCachedZEN() to load it.
     */
    bool hasCachedZEN(const bs::String& zenFile);
  }
}  // namespace REGoth
