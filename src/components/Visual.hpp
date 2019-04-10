#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  namespace Visual
  {
    enum class VisualKind
    {
      StaticMesh,
      MorphMesh,
      SkeletalMesh,
      Decal,
      Particles,
      Unknown,
    };

    /**
     * Tries to guess which visual should be created for the given visual name.
     *
     * Gothic uses the filenames of what a visual should represent to create the visual
     * instance matching the files type. For example, a `.3DS` file is handled by the
     * `StaticMesh`-Visual (`zCProgMeshProto` in the original).
     *
     * @param  visual  File name of the visual, e.g. `STONE.3DS`.
     *
     * @return Which kind of visual the given filename should use.
     */
    VisualKind guessVisualKind(const bs::String& visual);

    /**
     * Creates a matching visual component and attaches it to the given scene object.
     *
     * For example `STONE.3DS` will create a static mesh visual component.
     *
     * @param  so      Scene-Object to create the component for.
     * @param  visual  Name of the visual to create (The filename, that is), like `STONE.3DS`.
     *
     * @return Whether a visual component has been created.
     */
    bool addToSceneObject(bs::HSceneObject so, const bs::String& visual);
  }
}
