#pragma once
#include "ScriptBackedBy.hpp"
#include <AI/ScriptState.hpp>
#include <BsPrerequisites.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class VisualCharacter;
  using HVisualCharacter = bs::GameObjectHandle<VisualCharacter>;

  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  /**
   * Character AI. Implements most of the `AI_*` externals.
   * Needs to be attached to a scene-object which also has s `Character`-Component.
   */
  class CharacterAI : public bs::Component
  {
  public:
    CharacterAI(const bs::HSceneObject& parent, HGameWorld world);

    /**
     * Puts the characters physics to sleep which saves processing time.
     *
     * During physics sleep, no movement is being calculated and applied to the
     * Character-Controller. To enable physics again, see activatePhysics().
     */
    void deactivatePhysics();

    /**
     * Re-enables physics on this character.
     *
     * See deactivatePhysics() for more information.
     */
    void activatePhysics();

    /**
     * Whether Physics is active for this character.
     */
    bool isPhysicsActive() const;

    /**
     * Virtual input to the character. Calling these functions is equivalent to
     * holding down a button on the keyboard. If `goForward` is called, the Character
     * will try to move forward for as long as no other movement-function is called.
     * Transition-animations will also be played.
     *
     * To simulate letting go off all keys, `stopMoving()` can be called.
     *
     * @return True,  if a transition animation existed and the action was possible.
     *         False, if no transition animation existed and the requested action is not
     *                valid in the current state. This might happen while a transition
     *                animation is already playing.
     */
    bool goForward();
    bool goBackward();
    bool strafeLeft();
    bool strafeRight();
    bool stopMoving();
    bool turnLeft();
    bool turnRight();
    bool stopTurning();

    /**
     * Determines whether the character is allowed to switch to an other animation state,
     * such as from standing to running forward.
     *
     * A state switch is not allowed if a transition animation is currently playing. For
     * example, you have to see the whole *Jump Back* animation before you gain control
     * over your character again. In such a case, a state switch is not allowed.
     *
     * @return Whether the animation state is allowed to change now, e.g. from `S_RUN` to `S_RUNL`.
     */
    bool isStateSwitchAllowed();

    /**
     * Determines whether the character is allowed to turn (rotate).
     *
     * Turning is allowed in most movement states, but could be disabled for some special animations.
     *
     * @return Whether the character is allowed to turn right now.
     */
    bool isTurningAllowed();

    /**
     * Lets the character face into the direction of the given position instantly.
     *
     * Does not play any animations, just rotates the object accordingly.
     */
    void instantTurnToPosition(const bs::Vector3& position);

    /**
     * Will let the character move to the given postion.
     *
     * This will not do any obstruction checks and just walk the character there
     * in a straight line.
     *
     * @param  position  Position to go to.
     *
     * @return True, if the character reached the destination,
     *         False, while the character is still going.
     */
    bool gotoPositionStraight(const bs::Vector3& position);

    /**
     * Checks whether the character is at the given postiion.
     *
     * The check includes some leeway in all directions.
     *
     * @return True, if the character is currently at the given position
     *         False, if the character is NOT at the given position
     */
    bool isAtPosition(const bs::Vector3& position);

    /**
     * Debug routine to make animations play faster, which also makes movement
     * faster.
     */
    void fastMove(float factor);

    // Component -----------------------------------------------------------------------------------

    void fixedUpdate() override;

    // AI - Externals ------------------------------------------------------------------------------

    void teleport(const bs::String& waypoint);
    void turnToNPC(bs::HSceneObject targetSO);
    void standUp();
    void standUpQuick();
    void gotoWaypoint(const bs::String& waypoint);
    void gotoFreePoint(const bs::String& freepoint);
    void gotoNextFreePoint(const bs::String& freepoint);
    void gotoNpc(bs::HSceneObject targetCharacterSO);
    void startState(const bs::String& stateFunction, bs::INT32 stateBehavior,
                    const bs::String& waypoint);
    void wait(float seconds);
    void playAni(const bs::String& animation);
    void setWalkMode(bs::UINT32 walkMode);
    void stopProcessingInfos();
    void output(bs::HSceneObject characterSO, const bs::String& svmName);
    void processInfos();

  private:
    /**
     * Checks whether the character is so far away from the player that it should
     * deactivate physics to save some performance. Note that there exists another
     * method for checking whether to *activate* physics again. If the character
     * stands between the distances required for both these methods, both will
     * return false.
     *
     * See https://regoth-project.github.io/REGoth-bs/content/characters.html
     */
    bool shouldDisablePhysics() const;

    /**
     * Checks whether the character is so close to the player that it should
     * wake up and activate physics. Note that there exists another
     * method for checking whether to *deactivate* physics. If the character
     * stands between the distances required for both these methods, both will
     * return false.
     *
     * See https://regoth-project.github.io/REGoth-bs/content/characters.html
     */
    bool shouldEnablePhysics() const;

    /**
     * Activates, deactivates or keeps the current state of whether the physics
     * of this character are enabled. To be called every couple frames.
     */
    void handlePhysicsActivation();

    /**
     * Applies the currently set turning parameters to the character.
     *
     * Expected to be called inside fixedUpdate().
     */
    void handleTurning();

    // Visual attached to this character
    HVisualCharacter mVisual;
    HGameWorld mWorld;
    bs::HCharacterController mCharacterController;

    // AI-Script state handler
    bs::SPtr<AI::ScriptState> mScriptState;

    enum class TurnDirection
    {
      None,
      Left,
      Right,
    };

    // Direction the character is currently turning in
    TurnDirection mTurnDirection = TurnDirection::None;

    // Whether Physics is being processed for this character
    bool mIsPhysicsActive = true;

  public:
    REGOTH_DECLARE_RTTI(CharacterAI);

  protected:
    CharacterAI() = default;  // For RTTI
  };

  using HCharacterAI = bs::GameObjectHandle<CharacterAI>;

}  // namespace REGoth
