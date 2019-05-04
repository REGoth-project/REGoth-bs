#include "VisualCharacter.hpp"
#include "original-content/VirtualFileSystem.hpp"
#include <Animation/BsAnimationClip.h>
#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ImportSkeletalMesh.hpp>
#include <Components/BsCAnimation.h>
#include <Components/BsCRenderable.h>
#include <Debug/BsDebug.h>
#include <Mesh/BsMesh.h>
#include <RTTI/RTTI_VisualCharacter.hpp>
#include <Scene/BsSceneObject.h>
#include <animation/Animation.hpp>
#include <animation/StateNaming.hpp>
#include <components/NodeVisuals.hpp>
#include <exception/Throw.hpp>

const bs::String MODEL_NODE_NAME_R_HAND    = "BIP01 R HAND";
const bs::String MODEL_NODE_NAME_L_HAND    = "BIP01 L HAND";
const bs::String MODEL_NODE_NAME_R_FOOT    = "BIP01 R FOOT";
const bs::String MODEL_NODE_NAME_L_FOOT    = "BIP01 L FOOT";
const bs::String MODEL_NODE_NAME_HEAD      = "BIP01 HEAD";
const bs::String MODEL_NODE_NAME_FOOTSTEPS = " FOOTSTEPS";

namespace REGoth
{
  VisualCharacter::VisualCharacter(const bs::HSceneObject& parent)
      : VisualSkeletalAnimation(parent)
  {
    setName("VisualCharacter");
  }
  void VisualCharacter::setBodyMesh(const bs::String& bodyMesh)
  {
    if (!modelScript())
    {
      REGOTH_THROW(InvalidStateException, "No model script set! Has setVisual() been called?");
    }

    BsZenLib::Res::HMeshWithMaterials hmesh = modelScript()->getMeshByName(bodyMesh);

    if (hmesh)
    {
      setMesh(hmesh);
    }
    else if (modelScript()->getMeshes().empty())
    {
      REGOTH_THROW(InvalidParametersException, "Did not find body mesh " + bodyMesh +
                                                   " in model script '" + modelScript()->getName() +
                                                   "' and it seems to be empty otherwise!");
    }
    else
    {
      bs::gDebug().logWarning(bs::StringUtil::format(
          "[VisualCharacter] Did not find body mesh {0}  in model script "
          "'{1}'', defaulting to first one: {2}",
          bodyMesh, modelScript()->getName(), modelScript()->getMeshes()[0]->getName()));

      setMesh(modelScript()->getMeshes()[0]);
    }
  }

  void VisualCharacter::setHeadMesh(const bs::String& headmesh, bs::UINT32 headTextureIdx,
                                    bs::UINT32 teethTextureIdx)
  {
    mBodyState.headVisual      = headmesh;
    mBodyState.headTextureIdx  = headTextureIdx;
    mBodyState.teethTextureIdx = teethTextureIdx;

    // Choose MMB as default filetype of no extension was given
    if (!mBodyState.headVisual.empty() &&
        mBodyState.headVisual.find_first_of('.') == bs::String::npos)
    {
      mBodyState.headVisual += ".MMB";
    }

    updateHeadMesh();
  }

  void VisualCharacter::updateBodyMesh()
  {
  }

  void VisualCharacter::updateHeadMesh()
  {
    using namespace bs;

    if (!mBodyState.headVisual.empty())
    {
      nodeVisuals()->attachVisualToNode(MODEL_NODE_NAME_HEAD, mBodyState.headVisual);
    }
    else
    {
      nodeVisuals()->clearNodeAttachment(MODEL_NODE_NAME_HEAD);
    }

    // TODO: Fix texture and color
  }

  bs::Vector<bs::String> VisualCharacter::listPossibleDefaultAnimations() const
  {
    return {"S_RUN", "S_FISTRUN"};
  }

  REGOTH_DEFINE_RTTI(VisualCharacter)
}  // namespace REGoth
