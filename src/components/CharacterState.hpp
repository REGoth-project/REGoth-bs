/** \file
 */

#pragma once

#include <BsPrerequisites.h>
#include <BsZenLib/ZenResources.hpp>
#include <Scene/BsComponent.h>

#pragma once

namespace REGoth
{
  /**
   * Component for the state a character is in (Player, NPC, Monster).
   *
   * This component will take requests by user input or other external factors
   * and decide in which state the character should go. A state could be
   * *idle*, *running forward*, *weapon drawn*, *going from stating to running forward*
   * among others.
   *
   * Of course not all states can be reached all the time, so expect some actions
   * to be blocked or delayed. For example, if the character is in the *idle*-state
   * and the user presses the button to go forward, the state will need to go through
   * a *from-idle-to-forward*-state before it reaches the *running forward state*
   * since the correct animation has to be played.
   *
   * Depending on the state, the component can also suggest animations to be played
   * by the visual component, but it will never override the played animation
   * directly.
   *
   * Note the the animations are named after the names expected by the original
   * game, such as `S_RUN` for idle and `S_RUNL` for running forward.
   */
  class CharacterState : public bs::Component
  {
  public:
    CharacterState(const bs::HSceneObject& parent);

    /** Triggered once per frame. Allows the component to handle input and move. */
    void fixedUpdate() override;

    /** Information about the currently playing animations */
    struct PlayingAnimation
    {
      /** The currently playing animation clip */
      bs::HAnimationClip clip;

      /** How long the current animation has been played (in seconds) */
      float playingForSeconds;
    };

    /**
     * Set the currently playing animation.
     *
     * Since this component cannot play or query the currently playing animation
     * it has to rely on feedback of other components.
     *
     * @param  anim  The currently playing animation
     */
    void setPlayingAnimation(const PlayingAnimation& anim);

  protected:
    void stateIdle();

    PlayingAnimation mPlayingAnimation;
  };

}  // namespace REGoth
