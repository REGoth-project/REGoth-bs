#pragma once
#include "ScriptBackedBy.hpp"
#include <BsPrerequisites.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class VisualCharacter;
  using HVisualCharacter = bs::GameObjectHandle<VisualCharacter>;

  /**
   * Character AI. Implements most of the `AI_*` externals.
   * Needs to be attached to a scene-object which also has s `Character`-Component.
   */
  class CharacterAI : public bs::Component
  {
  public:
    CharacterAI(const bs::HSceneObject& parent);

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
     * Applies the currently set turning parameters to the character.
     *
     * Expected to be called inside fixedUpdate().
     */
    void handleTurning();

    // Visual attached to this character
    HVisualCharacter mVisual;
    bs::HCharacterController mCharacterController;

    enum class TurnDirection
    {
      None,
      Left,
      Right,
    };

    // Direction the character is currently turning in
    TurnDirection mTurnDirection = TurnDirection::None;

  public:
    REGOTH_DECLARE_RTTI(CharacterAI);
  public:  // FIXME: Should be protected, it is only used by RTTI but friend doesn't seem to work?!
    CharacterAI() = default;  // Serialization only
  };

  using HCharacterAI = bs::GameObjectHandle<CharacterAI>;

}  // namespace REGoth
