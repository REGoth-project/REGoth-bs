#include "OriginalGameResources.hpp"

#include <BsZenLib/CacheUtility.hpp>
#include <BsZenLib/ImportFont.hpp>
#include <BsZenLib/ImportMorphMesh.hpp>
#include <BsZenLib/ImportSkeletalMesh.hpp>
#include <BsZenLib/ImportStaticMesh.hpp>
#include <BsZenLib/ImportTexture.hpp>

#include <log/logging.hpp>
#include <original-content/VirtualFileSystem.hpp>

#include <Image/BsSpriteTexture.h>
#include <Threading/BsTaskScheduler.h>

namespace REGoth
{
  void OriginalGameResources::populateCache()
  {
    BsZenLib::CacheWholeVDFS(gVirtualFileSystem().getFileIndex());
  }

  bs::HTexture OriginalGameResources::texture(const bs::String& originalFileName)
  {
    bs::HTexture htexture;

    if (BsZenLib::HasCachedTexture(originalFileName))
    {
      htexture = BsZenLib::LoadCachedTexture(originalFileName);
    }

    // This call also checks whether dependencies are loaded. Broken resources
    // can be fixed by importing them.
    if (!htexture.isLoaded())
    {
      htexture =
          BsZenLib::ImportAndCacheTexture(originalFileName, gVirtualFileSystem().getFileIndex());
    }

    return htexture;
  }

  BsZenLib::Res::HModelScriptFile OriginalGameResources::modelScript(
      const bs::String& originalFileName)
  {
    BsZenLib::Res::HModelScriptFile hmodelScript;

    if (BsZenLib::HasCachedMDS(originalFileName))
    {
      hmodelScript = BsZenLib::LoadCachedMDS(originalFileName);
    }

    // This call also checks whether dependencies are loaded. Broken resources
    // can be fixed by importing them.
    if (!hmodelScript.isLoaded())
    {
      hmodelScript =
          BsZenLib::ImportAndCacheMDS(originalFileName, gVirtualFileSystem().getFileIndex());
    }

    return hmodelScript;
  }

  BsZenLib::Res::HMeshWithMaterials OriginalGameResources::staticMesh(
      const bs::String& originalFileName)
  {
    BsZenLib::Res::HMeshWithMaterials hmesh;

    if (BsZenLib::HasCachedStaticMesh(originalFileName))
    {
      hmesh = BsZenLib::LoadCachedStaticMesh(originalFileName);
    }

    // This call also checks whether dependencies are loaded. Broken resources
    // can be fixed by importing them.
    if (!hmesh.isLoaded())
    {
      hmesh =
          BsZenLib::ImportAndCacheStaticMesh(originalFileName, gVirtualFileSystem().getFileIndex());
    }

    return hmesh;
  }

  BsZenLib::Res::HMeshWithMaterials OriginalGameResources::morphMesh(
      const bs::String& originalFileName)
  {
    BsZenLib::Res::HMeshWithMaterials hmesh;

    if (BsZenLib::HasCachedMorphMesh(originalFileName))
    {
      hmesh = BsZenLib::LoadCachedMorphMesh(originalFileName);
    }

    // This call also checks whether dependencies are loaded. Broken resources
    // can be fixed by importing them.
    if (!hmesh.isLoaded())
    {
      hmesh =
          BsZenLib::ImportAndCacheMorphMesh(originalFileName, gVirtualFileSystem().getFileIndex());
    }

    return hmesh;
  }

  bs::HFont OriginalGameResources::font(const bs::String& originalFileName)
  {
    bs::HFont hfont;

    if (BsZenLib::HasCachedFont(originalFileName))
    {
      hfont = BsZenLib::LoadCachedFont(originalFileName);
    }

    // This call also checks whether dependencies are loaded. Broken resources
    // can be fixed by importing them.
    if (!hfont.isLoaded())
    {
      hfont = BsZenLib::ImportAndCacheFont(originalFileName, gVirtualFileSystem().getFileIndex());
    }

    return hfont;
  }

  bs::HSpriteTexture OriginalGameResources::sprite(const bs::String& originalFileName)
  {
    bs::HTexture t = texture(originalFileName);

    if (!t)
    {
      REGOTH_LOG(Warning, Uncategorized, "[UIElement] Failed to load texture: {0}",
                 originalFileName);

      return {};
    }

    return bs::SpriteTexture::create(t);
  }

  OriginalGameResources& gOriginalGameResources()
  {
    static OriginalGameResources s_instance;

    return s_instance;
  }
}  // namespace REGoth
