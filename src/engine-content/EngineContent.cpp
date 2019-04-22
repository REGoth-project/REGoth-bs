#include "EngineContent.hpp"
#include "internal/FindEngineContent.hpp"
#include <BsZenLib/ImportPath.hpp>
#include <FileSystem/BsFileSystem.h>
#include <Importer/BsImporter.h>
#include <Resources/BsResourceManifest.h>
#include <Resources/BsResources.h>
#include <excepction/Throw.hpp>

const bs::String REGOTH_CONTENT_MANIFEST_NAME = "engine-cache";

namespace REGoth
{
  EngineContent::EngineContent(const bs::Path& executablePath)
  {
    mContentPath = Internal::findEngineContentLocation(executablePath);
  }

  bool EngineContent::hasFoundContentDirectory() const
  {
    return mContentPath != bs::Path::BLANK;
  }

  const bs::Path& EngineContent::contentPath() const
  {
    if (!hasFoundContentDirectory())
    {
      REGOTH_THROW(InvalidStateException, "Did not find content directory!");
    }

    return mContentPath;
  }

  void EngineContent::loadResourceManifest()
  {
    bs::Path manifestPath = BsZenLib::GothicPathToCachedManifest(REGOTH_CONTENT_MANIFEST_NAME);

    if (bs::FileSystem::exists(manifestPath))
    {
      mResourceManifest = bs::ResourceManifest::load(manifestPath, BsZenLib::GetCacheDirectory());

      bs::gResources().registerResourceManifest(mResourceManifest);
    }
    else
    {
      mResourceManifest = bs::ResourceManifest::create(REGOTH_CONTENT_MANIFEST_NAME);
    }
  }

  EngineContent::Shaders EngineContent::loadShaders()
  {
    throwOnMissingResourceManifest();

    Shaders r;

    r.opaque = loadOrImportShader("World.bsl");

    return r;
  }

  bs::HShader EngineContent::loadOrImportShader(const bs::String& name)
  {
    if (bs::FileSystem::isFile(BsZenLib::GothicPathToCachedShader(name)))
    {
      return bs::gResources().load<bs::Shader>(BsZenLib::GothicPathToCachedShader(name));
    }
    else
    {
      bs::HShader shader = bs::gImporter().import<bs::Shader>(contentPath() + "shaders" + name);

      if (shader)
      {
        addResourceToManifestAndSave(shader, BsZenLib::GothicPathToCachedShader(name));
      }

      return shader;
    }
  }

  void EngineContent::throwOnMissingResourceManifest()
  {
    if (!mResourceManifest)
    {
      REGOTH_THROW(InvalidStateException,
                   "ResourceManifest has not been loaded or created yet. Has loadResourceManifest() "
                   "been called?");
    }

  }

  void EngineContent::addResourceToManifestAndSave(bs::HResource resource, const bs::Path& path)
  {
    enum
    {
      Overwrite    = true,
      KeepExisting = false,
    };

    bs::gResources().save(resource, path, Overwrite);

    mResourceManifest->registerResource(resource.getUUID(), path);

    saveResourceManifest();
  }

  void EngineContent::saveResourceManifest()
  {
    bs::Path manifestPath = BsZenLib::GothicPathToCachedManifest(REGOTH_CONTENT_MANIFEST_NAME);
    bs::ResourceManifest::save(mResourceManifest, manifestPath, BsZenLib::GetCacheDirectory());
  }

}  // namespace REGoth
