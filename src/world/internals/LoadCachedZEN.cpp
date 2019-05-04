#include "LoadCachedZEN.hpp"
#include "ConstructFromZEN.hpp"
#include <BsZenLib/ImportPath.hpp>
#include <FileSystem/BsFileSystem.h>
#include <Resources/BsResources.h>
#include <Scene/BsPrefab.h>
#include <exception/Throw.hpp>

namespace REGoth
{
  namespace World
  {
    bs::HSceneObject loadCachedZEN(const bs::String& zenFile)
    {
      if (!hasCachedZEN(zenFile))
      {
        REGOTH_THROW(FileNotFoundException,
                     "Requested loading ZEN from cache, but no cache exists!");
      }

      bs::Path path      = BsZenLib::GothicPathToCachedWorld(zenFile);
      bs::HPrefab prefab = bs::gResources().load<bs::Prefab>(path);

      return prefab->instantiate();
    }

    void saveCacheForZEN(bs::HSceneObject root, const bs::String& zenFile)
    {
      bs::HPrefab cached = bs::Prefab::create(root);

      enum
      {
        Overwrite = true,
        KeepExisting = false,
      };

      bs::Path path = BsZenLib::GothicPathToCachedWorld(zenFile);
      bs::gResources().save(cached, path, Overwrite);
    }

    bool hasCachedZEN(const bs::String& zenFile)
    {
      return bs::FileSystem::exists(BsZenLib::GothicPathToCachedWorld(zenFile));
    }
  }  // namespace World
}  // namespace REGoth
