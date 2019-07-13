#include "OriginalGameResources.hpp"
#include <Image/BsSpriteTexture.h>
#include <BsZenLib/ImportFont.hpp>
#include <BsZenLib/ImportMorphMesh.hpp>
#include <BsZenLib/ImportSkeletalMesh.hpp>
#include <BsZenLib/ImportStaticMesh.hpp>
#include <BsZenLib/ImportTexture.hpp>
#include <original-content/VirtualFileSystem.hpp>

namespace REGoth
{
  bs::HTexture OriginalGameResources::texture(const bs::String& originalFileName)
  {
    if (BsZenLib::HasCachedTexture(originalFileName))
    {
      return BsZenLib::LoadCachedTexture(originalFileName);
    }
    else
    {
      return BsZenLib::ImportAndCacheTexture(originalFileName, gVirtualFileSystem().getFileIndex());
    }
  }

  BsZenLib::Res::HModelScriptFile OriginalGameResources::modelScript(
      const bs::String& originalFileName)
  {
    if (BsZenLib::HasCachedMDS(originalFileName))
    {
      return BsZenLib::LoadCachedMDS(originalFileName);
    }
    else
    {
      return BsZenLib::ImportAndCacheMDS(originalFileName, gVirtualFileSystem().getFileIndex());
    }
  }

  BsZenLib::Res::HMeshWithMaterials OriginalGameResources::staticMesh(
      const bs::String& originalFileName)
  {
    if (BsZenLib::HasCachedStaticMesh(originalFileName))
    {
      return BsZenLib::LoadCachedStaticMesh(originalFileName);
    }
    else
    {
      return BsZenLib::ImportAndCacheStaticMesh(originalFileName,
                                                gVirtualFileSystem().getFileIndex());
    }
  }

  BsZenLib::Res::HMeshWithMaterials OriginalGameResources::morphMesh(
      const bs::String& originalFileName)
  {
    if (BsZenLib::HasCachedMorphMesh(originalFileName))
    {
      return BsZenLib::LoadCachedMorphMesh(originalFileName);
    }
    else
    {
      return BsZenLib::ImportAndCacheMorphMesh(originalFileName,
                                               gVirtualFileSystem().getFileIndex());
    }
  }

  bs::HFont OriginalGameResources::font(const bs::String& originalFileName)
  {
    if (BsZenLib::HasCachedFont(originalFileName))
    {
      return BsZenLib::LoadCachedFont(originalFileName);
    }
    else
    {
      return BsZenLib::ImportAndCacheFont(originalFileName, gVirtualFileSystem().getFileIndex());
    }
  }

  bs::HSpriteTexture OriginalGameResources::sprite(const bs::String& originalFileName)
  {
    bs::HTexture t = texture(originalFileName);

    if (!t)
    {
      BS_LOG(Warning, Uncategorized, "[UIElement] Failed to load texture: " + originalFileName);

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
