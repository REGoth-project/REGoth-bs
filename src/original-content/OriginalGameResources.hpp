#pragma once
#include <BsPrerequisites.h>

namespace BsZenLib
{
  namespace Res
  {
    class ModelScriptFile;
    typedef bs::ResourceHandle<ModelScriptFile> HModelScriptFile;

    class MeshWithMaterials;
    typedef bs::ResourceHandle<MeshWithMaterials> HMeshWithMaterials;
  }  // namespace Res
}  // namespace BsZenLib

namespace REGoth
{
  /**
   * This provides a global object to load resources from the original game.
   *
   * This includes:
   *
   *  - Textures,
   *  - Meshes,
   *  - Fonts.
   *
   * As data source, the Virtual File System is used.
   *
   * To make loading more efficient, a check whether the resource to load has been
   * cached is done. If it was not, the resource is imported into the cache so it
   * can be loaded quicker next time.
   */
  class OriginalGameResources
  {
  public:
    /**
     * Goes through the loaded VDFS packages and imports the original games resources
     * which were not already cached.
     */
    static void populateCache();

    /**
     * Loads a texture from the original game files.
     *
     * If the resource has not been cached before, it will be cached inside.
     * Otherwise, the cached resource is loaded.
     *
     * @param  originalFileName  File name as in the original game, e.g. `STONE.TGA`.
     *
     * @return bsf resource handle. Empty if loading failed.
     */
    bs::HTexture texture(const bs::String& originalFileName);

    /**
     * Loads a ModelScriptFile (MDS) from the original game files.
     *
     * If the resource has not been cached before, it will be cached inside.
     * Otherwise, the cached resource is loaded.
     *
     * @param  originalFileName  File name as in the original game, e.g. `HUMANS.MDS`.
     *
     * @return bsf resource handle. Empty if loading failed.
     */
    BsZenLib::Res::HModelScriptFile modelScript(const bs::String& originalFileName);

    /**
     * Loads a Static Mesh (3DS) from the original game files.
     *
     * If the resource has not been cached before, it will be cached inside.
     * Otherwise, the cached resource is loaded.
     *
     * @param  originalFileName  File name as in the original game, e.g. `STONE.3DS`.
     *
     * @return bsf resource handle. Empty if loading failed.
     */
    BsZenLib::Res::HMeshWithMaterials staticMesh(const bs::String& originalFileName);

    /**
     * Loads a MorphMesh (MMS/MMB) from the original game files.
     *
     * If the resource has not been cached before, it will be cached inside.
     * Otherwise, the cached resource is loaded.
     *
     * @param  originalFileName  File name as in the original game, e.g. `STONE.3DS`.
     *
     * @return bsf resource handle. Empty if loading failed.
     */
    BsZenLib::Res::HMeshWithMaterials morphMesh(const bs::String& originalFileName);

    /**
     * Loads a Font (FNT) from the original game files.
     *
     * If the resource has not been cached before, it will be cached inside.
     * Otherwise, the cached resource is loaded.
     *
     * @param  originalFileName  File name as in the original game, e.g. `FONT_10_BOOK.FNT`.
     *
     * @return bsf resource handle. Empty if loading failed.
     */
    bs::HFont font(const bs::String& originalFileName);

    /**
     * Loads a texture into a sprite.
     *
     * @param  originalFileName  File name as in the original game, e.g. `STONE.TGA`.
     *
     * @return Sprite with the given texture. Empty handle if loading failed.
     */
    bs::HSpriteTexture sprite(const bs::String& originalFileName);

  private:
    /**
     * Caches so that we don't have to re-load resources.
     */
    bs::Map<bs::String, bs::HTexture> mTextures;
    bs::Map<bs::String, BsZenLib::Res::HModelScriptFile> mModelScripts;
    bs::Map<bs::String, BsZenLib::Res::HMeshWithMaterials> mStaticMeshes;
    bs::Map<bs::String, BsZenLib::Res::HMeshWithMaterials> mMorphMeshes;
    bs::Map<bs::String, bs::HFont> mFonts;
    bs::Map<bs::String, bs::HSpriteTexture> mSprites;
  };

  /**
   * Global access to the virtual file system.
   */
  OriginalGameResources& gOriginalGameResources();
}  // namespace REGoth
