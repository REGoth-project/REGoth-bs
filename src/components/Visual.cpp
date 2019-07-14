#include "Visual.hpp"
#include "VisualInteractiveObject.hpp"
#include "VisualMorphMesh.hpp"
#include "VisualStaticMesh.hpp"
#include <Scene/BsSceneObject.h>
#include <log/logging.hpp>

namespace REGoth
{
  Visual::VisualKind Visual::guessVisualKind(const bs::String& visual)
  {
    enum
    {
      CompareCaseSensitive = false,
      ConvertToLowercase   = true,
    };

    if (bs::StringUtil::endsWith(visual, ".3ds", ConvertToLowercase))
    {
      return VisualKind::StaticMesh;
    }
    else if (bs::StringUtil::endsWith(visual, ".mmb", ConvertToLowercase))
    {
      return VisualKind::MorphMesh;
    }
    else if (bs::StringUtil::endsWith(visual, ".mms", ConvertToLowercase))
    {
      return VisualKind::MorphMesh;
    }
    else if (bs::StringUtil::endsWith(visual, ".asc", ConvertToLowercase))
    {
      return VisualKind::InteractiveObject;
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
    else if (kind == VisualKind::MorphMesh)
    {
      HVisualMorphMesh mesh = so->addComponent<VisualMorphMesh>();
      mesh->setMesh(visual);

      return true;
    }
    else if (kind == VisualKind::InteractiveObject)
    {
      HVisualInteractiveObject mesh = so->addComponent<VisualInteractiveObject>();

      mesh->setVisual(visual);

      return true;
    }
    else
    {
      REGOTH_LOG(Warning, Uncategorized, "[Visual] Unsupported visual type ({0}) of {1}", (int)kind,
                 visual);
      return false;
    }
  }

}  // namespace REGoth
