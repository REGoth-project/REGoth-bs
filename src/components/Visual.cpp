#include "Visual.hpp"
#include "VisualMorphMesh.hpp"
#include "VisualStaticMesh.hpp"
#include <Scene/BsSceneObject.h>

namespace REGoth
{
  Visual::VisualKind Visual::guessVisualKind(const bs::String& visual)
  {
    const bool lowercase = true;

    if (bs::StringUtil::endsWith(visual, ".3ds", lowercase))
    {
      return VisualKind::StaticMesh;
    }
    else if (bs::StringUtil::endsWith(visual, ".mmb", lowercase))
    {
      return VisualKind::MorphMesh;
    }
    else
    {
      // TODO: Handle Particles, SkeletalMeshes (and possible more?)
      return VisualKind::Unknown;
    }
  }

  bool Visual::addToSceneObject(bs::HSceneObject so, const bs::String& visual)
  {
    VisualKind kind = guessVisualKind(visual);

    if (kind == VisualKind::StaticMesh)
    {
      HVisualStaticMesh mesh = so->addComponent<VisualStaticMesh>();
      mesh->setMesh(visual);

      return true;
    }
    if (kind == VisualKind::MorphMesh)
    {
      HVisualMorphMesh mesh = so->addComponent<VisualMorphMesh>();
      mesh->setMesh(visual);

      return true;
    }
    else
    {
      bs::gDebug().logWarning("[Visual] Unsupported visual type (" + bs::toString((int)kind) +
                              ") of " + visual);
      return false;
    }
  }

}  // namespace REGoth
