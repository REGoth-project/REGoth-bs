/**\file
 * See documentation at:
 * https://regoth-project.github.io/REGoth-bs/content/case-study-character-statemachine.html
 */

#pragma once
#include <BsPrerequisites.h>
#include <AI/WalkMode.hpp>
#include <AI/WeaponMode.hpp>

namespace REGoth
{
  namespace AnimationState
  {
    /**
     * Construct a state animation name from the given weapon-mode and state-name.
     *
     * @note There might be no actual file for the generated animation name.
     *
     * @param  weaponMode  Weapon-Mode, like *None* or *One Handed Weapon*.
     * @param  walkMode    Walk-Mode, like *Run* or *Sneak*.
     * @param  stateName   Name of the state, like *L*.
     *
     * @result State-Animation name, like *S_RUNL*.
     */
    bs::String constructStateAnimationName(AI::WeaponMode weaponMode, AI::WalkMode walkMode,
                                           const bs::String& name);

    /**
     * Construct a transition animation name from the given weapon-mode and state-name to go from and
     * to.
     *
     * @note There might be no actual file for the generated animation name.
     *
     * @param  weaponMode  Weapon-Mode, like *None* or *One Handed Weapon*.
     * @param  from        Name of the state currently in, like *RUN*.
     * @param  to          Name of the state going to, like *RUNL*.
     *
     * @result Transition animation, like *T_RUN_2_RUNL*.
     */
    bs::String constructTransitionAnimationName(AI::WeaponMode weaponMode, const bs::String& from,
                                                const bs::String& to);

    /**
     * Returns the name of the state given it's animation.
     *
     * All weapon mode tags will also be discarded by the function.
     *
     * @param  animation  Animation name like `S_RUNL`.
     *
     * @return Name of the state, so for `S_1HRUNL` this will return `RUNL`.
     */
    bs::String getStateName(const bs::String& animation);

    /**
     * Given an animation name like `S_1HRUNL`, this function will return which
     * weapon mode the animation is for.
     *
     * So for `S_1HRUNL` it will return `AI::WeaponMode::OneHanded`.
     *
     * @param  animation  State-Animation name, like `S_1HRUNL`.
     *
     * @return Weapon mode of the given animation.
     */
    AI::WeaponMode getWeaponMode(const bs::String& animation);

    /**
     * Checks whether the given animation needs a transition.
     *
     * Some animations like `T_JUMPB` don't need a transition as they are
     * their own transition (or something like that).
     * Therefore, they can be reached from the usual states like `S_RUN`.
     *
     * Special care has to be taken on walk modes which don't support eg. `T_JUMPB`
     * like diving.
     *
     * @param  animation  Animation name like `S_RUN` or `T_JUMPB`
     *
     * @return True if a transition animation needs to be found for going into that animation.
     */
    bool isTransitionNeeded(const bs::String& animation);

    /**
     * Given a raw animation name, this will strip the weapon tag from it.
     *
     * For example, given `1HRUNL`, this will return `RUNL`.
     *
     * @param  animation  raw animation name, without state tag (e.g. `1HRUNL`)
     *
     * @return Given animation name without any weapon information.
     */
    bs::String stripWeaponModeFromAnimationName(const bs::String& animation);

  }  // namespace Animation
}  // namespace REGoth
