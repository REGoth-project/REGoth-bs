/** \file
 */

#pragma once

#include <BsPrerequisites.h>
#include <BsZenLib/ZenResources.hpp>
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class RTTI_VisualSkeletalAnimation;
  class NodeVisuals;
  using HNodeVisuals = bs::GameObjectHandle<NodeVisuals>;

  /**
   * Component for rendering an object animated using skeletal animation (Player, NPC, Monster).
   *
   * This component will take care of loading the correct resources and has
   * an interface to play animations.
   *
   * # Object tree
   *
   * Because of the way bsf works, one cannot modify any transforms like scaling
   * or rotation if root-motion is used on a scene object. Therefore the actual
   * characters renderable and animation components are put into a sub-object.
   *
   * This sub-object will also handle all visual node attachments.
   *
   * # Model script files
   *
   * In Gothic, similar creatures and characters are described inside a so called
   * *model script file*. Inside this file, every mesh matching the animation
   * node tree is listed, along with all possible animations and their properties.
   *
   * Therefore, this component needs to know the model script and the mesh
   * that it should display from that model script.
   */
  class VisualSkeletalAnimation : public bs::Component
  {
  public:
    VisualSkeletalAnimation(const bs::HSceneObject& parent);

    /**
     * Set which model script this component should read the list of possible
     * meshes and animations from. This **must** be set for the component to work.
     */
    void setModelScript(BsZenLib::Res::HModelScriptFile modelScript);

    /**
     * Set the mesh this component should display. This mesh **must** come from
     * within the given model script file, otherwise the component won't display
     * the mesh.
     */
    void setMesh(BsZenLib::Res::HMeshWithMaterials mesh);

    /**
     * Assigns the first mesh found in the model script via setMesh().
     *
     * Can be used instead of calling setMesh(), for example if you know
     * the model-script only contains a single mesh (like for interactive objects).
     * Note that setVisual() automatically uses this.
     *
     * Throws if no model script has been assigned before or it it is empty.
     */
    void useFirstMeshOfModelScript();

    /**
     * Sets up the visual according to the given visual name.
     *
     * Will automatically resolve and set the model script and assign its first mesh.
     *
     * Throws if that visual does not exist.
     *
     * @param  visual  Name of the visual (model-script) to use, e.g. `HUMANS.MDS`.
     */
    void setVisual(const bs::String& visual);

    /**
     * Calculates how far the characters animation has moved since the last
     * call ot this function via animation.
     *
     * @return Delta between the animation postion when this function was called
     *         the last time and now.
     */
    bs::Vector3 resolveFrameRootMotion();

    /**
     * @return The bounds of the underlaying renderable
     */
    bs::Bounds getBounds() const;

    /**
     * Find the animation clip matching the currently set model script and overlay.
     * Returns an invalid handle when the was not found.
     *
     * @param  name  The UPPERCASE animation name (`S_RUNL`)
     *
     * @return Animation clip for the given animation name. Invalid if not found.
     */
    bs::HAnimationClip findAnimationClip(const bs::String& name) const;

    /**
     * Plays the given animation clip.
     */
    void playAnimation(bs::HAnimationClip clip);

    /**
     * Searches for the default idle animation for this character and plays it.
     *
     * The idle animation for humans is `S_RUN`. For monsters, that one doesn't exist
     * since they are always in the *Fist*-Weaponmode. Therefore `S_FISTRUN` is being
     * played instead.
     *
     * This method should be only for for testing, since playing the correct animations
     * should be handled by other parts of the software.
     */
    void playDefaultIdleAnimation();

    /**
     * Tries to transition to the given animation name.
     *
     * @param  Animation to transition to, eg `S_RUNL` or `T_JUMPB`.
     *
     * @return true, if the transition was possible.
     */
    bool tryPlayTransitionAnimationTo(const bs::String& state);

    /**
     * @return Whether the given animation is currently playing
     */
    bool isAnimationPlaying(bs::HAnimationClip clip) const;

    /**
     * @return Name of the currently playing animation. Empty string if none.
     */
    bs::String getPlayingAnimationName() const;

    /**
     * @return The current state animations state, eg. `RUNL` for `S_RUNL`.
     */
    bs::String getStateFromPlayingAnimation() const;

    /**
     * Whether the currently playing animation can be interrupted by the user.
     *
     * This will return false on animations like jumping backwards or
     * falling onto the floor when dying.
     *
     * If no animation is played, true is returned.
     *
     * @return Whether the user can interrupt the currently playing animation.
     */
    bool isPlayingAnimationInterruptable() const;

    /**
     * This is only for debugging, don't expect this to be saved with a savegame!
     * Sets the animation speed factor, e.g. for fast running.
     */
    void setDebugAnimationSpeedFactor(float factor);

  protected:

    void onInitialized() override;

    /**
     * @return A list of animations to try playing after initialization or
     *         if no animation is playing.
     */
    virtual bs::Vector<bs::String> listPossibleDefaultAnimations() const;

    /**
     * Called when a animation event was triggered
     */
    virtual void onAnimationEvent(const bs::HAnimationClip& clip, bs::String string);

    /**
     * Access to the current model-script for sub-classes.
     */
    BsZenLib::Res::HModelScriptFile modelScript() const
    {
      return mModelScript;
    }

    /**
     * Access to the currently drawn mesh for sub-classes.
     */
    BsZenLib::Res::HMeshWithMaterials mesh() const
    {
      return mMesh;
    }

    /**
     * Access to attaching something to specific nodes for sub-classes.
     */
    HNodeVisuals nodeVisuals() const
    {
      return mSubNodeVisuals;
    }

    /**
     * Throws if there is no model-script or mesh set or if some initalization step
     * was missing.
     */
    void throwIfNotReadyForRendering() const;

  private:

    /**
     * Fills mAnimationClips.
     */
    void createAnimationMap();

    /**
     * @return Whether the given mesh is registered inside the currently set model script
     */
    bool isMeshRegisteredInModelScript(BsZenLib::Res::HMeshWithMaterials mesh);

    /**
     * Finds the correct animation to play now which will get the visual from the
     * state it is currently in into the given target state.
     *
     * For example, given the visual is currently in state `S_RUN` and is requested
     * to got to state `S_RUNL`, then this method will return `T_RUN_2_RUNL`.
     *
     * If there is no valid transition animation for going into the requested target
     * state, an empty string is returned. This means it's impossible for the visual
     * right now to go to that state, e.g. because it's falling and needs to land first.
     *
     * Some animations do not need any transitions, or maybe no animation is currently
     * being played, for which the target states animation name is returned.
     *
     * @return Name of the animation to play to reach the given state. Empty string
     *         if the transition is not possible.
     */
    bs::String findAnimationToTransitionToState(const bs::String& state);

    /**
     * Deletes all sub objects created by this component (ie. for rendering)
     */
    void deleteObjectSubtree();

    /**
     * (Re)creates all sub objects needed ie. for rendering.
     */
    void buildObjectSubtree();

    /**
     * Creates the sub object and all components needed on that object.
     */
    void createAndRegisterSubComponents();

    /**
     * Creates a sub-object and adds it to the list of all sub-objects.
     */
    bs::HSceneObject createAndRegisterSubObject(const bs::String& name);

    /**
     * Sets up the empty previously created renderable component.
     */
    void setupRenderableComponent();

    /**
     * Sets up the empty previously create animation component.
     */
    void setupAnimationComponent();

    /**
     * Adds the attachments defined inside the model script to their nodes.
     */
    void addDefaultAttachments();

    /**
     * Whether the given clip should be played as looping. If not, it will likely
     * switch to a different animation when it's done.
     */
    bool isClipLooping(bs::HAnimationClip clip);

    /**
     * @return Layer the clip should be played on.
     */
    bs::INT32 getClipLayer(bs::HAnimationClip clip);

    // Configuration ----------------------------------------------------------

    BsZenLib::Res::HModelScriptFile mModelScript; /**< Model-script of the displayed model */
    BsZenLib::Res::HMeshWithMaterials mMesh; /**< Currently displayed mesh, from the model script */

    // Object Sub Tree --------------------------------------------------------
    bs::Vector<bs::HSceneObject> mSubObjects; /**< All created sub-objects by this component */

    bs::HRenderable mSubRenderable; /**< The Renderable created inside a sub object */
    bs::HAnimation mSubAnimation;   /**< The Animation-Component created inside a sub object */
    HNodeVisuals mSubNodeVisuals;   /**< The NodeVisuals-Component created inside a sub object */

    // Animation --------------------------------------------------------------
    bs::Map<bs::String, bs::HAnimationClip> mAnimationClips; /**< Animation names -> clip */
    bs::HAnimationClip mRootMotionLastClip; /**< Last clip we got the root motion from */
    float mRootMotionLastTime = 0.0f; /**< Last time the animation was queried for root motion */

  public:
    REGOTH_DECLARE_RTTI(VisualSkeletalAnimation)

  protected:
    VisualSkeletalAnimation() = default; // For RTTI
  };

  using HVisualSkeletalAnimation = bs::GameObjectHandle<VisualSkeletalAnimation>;
}  // namespace REGoth
