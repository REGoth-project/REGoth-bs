#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  /**
   * Finds REGoths own `content`-folder and offers functions to load the
   * resources stored inside it. This includes loading shaders and other resources.
   *
   * Since this class also imports and loads resources, it comes with its own
   * resource manifest. This manifest is also managed here.
   *
   * All resources loaded here, are automatically added to the ResourceManifest, and
   * the manifest itself is saved after each added resource.
   */
  class EngineContent
  {
  public:
    /**
     * Constructor.
     *
     * @param  executablePath  Path to the currently running REGoth-executable.
     *                         Can be retrieved from argv[0].
     */
    EngineContent(const bs::Path& executablePath);

    /**
     * @return True, if REGoth's `content`-directory has been found.
     */
    bool hasFoundContentDirectory() const;

    /**
     * @return Path to REGoth's `content`-directory, e.g. `/home/nameless/REGoth/content/`.
     *
     * Throws, if the `content`-directory was not found.
     */
    const bs::Path& contentPath() const;

    /**
     * Will try to load a possibly existing resource manifest. If none is found
     * a blank one will be created.
     */
    void loadResourceManifest();

    struct Shaders
    {
      bs::HShader opaque;
    };

    /**
     * Loads the shader stored inside the `content`-directory. Will also handle
     * importing and caching.
     *
     * Throws if the `content`-directory was not found or one shader does
     * not exist.
     *
     * Throws if the ResourceManifest has not been loaded before,
     * call loadResourceManifest().
     */
    Shaders loadShaders();

  private:

    /**
     * Loads or imports the shader with the given name.
     *
     * If there exists a cached version of the shader, the cached shader
     * will be loaded. If there is no cached shader, the shader
     * will be imported.
     *
     * This method looks inside the contents `shaders`-directory for
     * a file with the given name.
     *
     * @param  name  Name of the shader file, e.g. `World.bsl`.
     *
     * @return Handle to the loaded or imported shader. Invalid handle
     *         if something failed.
     */
    bs::HShader loadOrImportShader(const bs::String& name);

    void throwOnMissingResourceManifest();
    void addResourceToManifestAndSave(bs::HResource resource, const bs::Path& path);
    void saveResourceManifest();

    /**
     * Path to REGoth's `content`-directory, e.g. `/home/nameless/REGoth/content/`.
     */
    bs::Path mContentPath;

    /**
     * Resource-Manifest for REGoth's content.
     */
    bs::SPtr<bs::ResourceManifest> mResourceManifest;
  };
}
