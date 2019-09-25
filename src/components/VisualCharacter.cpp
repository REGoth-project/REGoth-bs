#include "VisualCharacter.hpp"
#include <RTTI/RTTI_VisualCharacter.hpp>

#include <BsZenLib/ImportPath.hpp>
#include <BsZenLib/ImportSkeletalMesh.hpp>

#include <Components/BsCAnimation.h>
#include <Components/BsCRenderable.h>

#include <Animation/BsAnimationClip.h>
#include <Debug/BsDebug.h>
#include <Image/BsTexture.h>
#include <Material/BsMaterial.h>
#include <Mesh/BsMesh.h>
#include <Scene/BsSceneObject.h>

#include <animation/Animation.hpp>
#include <animation/StateNaming.hpp>
#include <components/NodeVisuals.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <original-content/OriginalGameResources.hpp>
#include <original-content/VirtualFileSystem.hpp>

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

  void VisualCharacter::onInitialized()
  {
    VisualSkeletalAnimation::onInitialized();

    setBodyTexture(mBodyState.bodyTexture);
    setHeadTexture(mBodyState.headTexture);
  }

  void VisualCharacter::setBodyMesh(const bs::String& bodyMesh, bs::UINT32 bodyTextureIdx,
                                    bs::UINT32 bodySkinColorIdx)
  {
    if (!modelScript())
    {
      REGOTH_THROW(InvalidStateException, "No model script set! Has setVisual() been called?");
    }

    // Those sometimes come with file extension.
    bs::String bodyMeshNoExt                = bodyMesh.substr(0, bodyMesh.find_last_of('.'));
    BsZenLib::Res::HMeshWithMaterials hmesh = modelScript()->getMeshByName(bodyMeshNoExt);

    if (hmesh)
    {
      setMesh(hmesh);
    }
    else if (modelScript()->getMeshes().empty())
    {
      REGOTH_THROW(InvalidParametersException, "Did not find body mesh " + bodyMeshNoExt +
                                                   " in model script '" + modelScript()->getName() +
                                                   "' and it seems to be empty otherwise!");
    }
    else
    {
      REGOTH_LOG(Warning, Uncategorized,
                 "[VisualCharacter] Did not find body mesh {0}  in model script "
                 "'{1}'', defaulting to first one: {2}",
                 bodyMeshNoExt, modelScript()->getName(), modelScript()->getMeshes()[0]->getName());

      setMesh(modelScript()->getMeshes()[0]);
    }

    mBodyState.bodyTextureIdx   = bodyTextureIdx;
    mBodyState.bodySkinColorIdx = bodySkinColorIdx;

    updateBodyMesh();
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

    // TODO: Save head texture once it is set

    updateHeadMesh();
  }

  void VisualCharacter::updateBodyMesh()
  {
    bs::String bodyTexName = getCurrentBodyTextureName();

    if (bodyTexName.empty()) return;

    bs::String newTextureName = bodyTexName;

    bool isBodyTexture = bodyTexName.find("_BODY") != bs::String::npos;

    // Only modify if that is the core body texture (not armor)
    if (isBodyTexture)
    {
      if (mBodyState.bodyTextureIdx != 0)
        newTextureName = bs::StringUtil::replaceAll(newTextureName, "_V0",
                                                    "_V" + bs::toString(mBodyState.bodyTextureIdx));

      if (mBodyState.bodySkinColorIdx != 0)
        newTextureName = bs::StringUtil::replaceAll(
            newTextureName, "_C0", "_C" + bs::toString(mBodyState.bodySkinColorIdx));
    }

    mBodyState.bodyTexture = gOriginalGameResources().texture(newTextureName);

    setBodyTexture(mBodyState.bodyTexture);
  }

  bs::String VisualCharacter::getCurrentBodyTextureName() const
  {
    if (mesh()->getMaterials().empty()) return "";

    auto bodyMaterial = mesh()->getMaterials()[0];
    auto albedo       = bodyMaterial->getTexture("gAlbedoTex");

    if (!albedo) return "";

    return albedo->getName();
  }

  void VisualCharacter::setBodyTexture(bs::HTexture texture)
  {
    if (material(0)) material(0)->setTexture("gAlbedoTex", texture);
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
    mBodyState.headTexture = {};

    setHeadTexture(mBodyState.headTexture);
  }

  void VisualCharacter::setHeadTexture(bs::HTexture texture)
  {
    // TODO: Implement setHeadTexture
  }

  bs::Vector<bs::String> VisualCharacter::listPossibleDefaultAnimations() const
  {
    return {"S_RUN", "S_FISTRUN"};
  }

  REGOTH_DEFINE_RTTI(VisualCharacter)
}  // namespace REGoth
