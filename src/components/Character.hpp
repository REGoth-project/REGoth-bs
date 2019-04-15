#pragma once
#include "ScriptBackedBy.hpp"
#include <BsPrerequisites.h>

namespace REGoth
{
  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  /**
   * Character logic. Implements most of the * externals.
   */
  class Character : public ScriptBackedBy
  {
  public:
    Character(const bs::HSceneObject& parent, const bs::String& instance);

    void onInitialized() override;

    /**
     * Registers this character as the hero. The hero will most likely be the player,
     * but doesn't have to be, ie. if the player controls another character.
     */
    void useAsHero();

    /**
     * @return Distance from this Character to the given other Character in meters.
     *
     * Throws if the given SO is not a character.
     */
    float getDistToNPC(bs::HSceneObject to);

    /**
     * @return Distance from this Character to the given waypoint in meters.
     *
     * Throws if the given waypoint does not exist.
     */
    float getDistToWaypoint(const bs::String& waypoint);

    /**
     * @return Distance from this Character to the given item in meters.
     *
     * Throws if the given SO is not an item.
     */
    float getDistToItem(bs::HSceneObject item);

    /**
     * @return Distance from this Character to the players character in meters.
     */
    float getDistToPlayer();

    bs::INT32 getTrueGuild();
    bs::INT32 setTrueGuild(bs::INT32 guild);

    void equipItem(const bs::String& instance);
    void createInventoryItem(const bs::String& instance, bs::INT32 num);

    void setToFightMode(const bs::String& weapon);
    void setToFistMode();

    /**
     * @return Whether this character is controlled by the player.
     */
    bool isPlayer();

    /**
     * @return Whether this character is currently dead.
     */
    bool isDead();

    /**
     * Check if `Character` is near a used spot (direct).
     *
     * The message handler is searching for the nearest spot
     * (in a 2 meters bounding box) where the name of the spot
     * includes `NamePart`, that is available for the `Character`,
     * and that is in the free line of sight for the \p Character.
     *
     * @note    If the `Character` is currently using the spot, the spot is
     *          made available again for usage if the `Character` is currently
     *          not inside a 1x2x1 meter bounding box of the spot.
     *
     * @param   namePart   Substring that has to be present in the name of the spot
     *                     (the comparison is case sensitive, should be upper case).
     *
     * @return  Returns TRUE if the `Character` is using the nearest spot.
     *
     * @sa      CharacterAI::alignToFP
     * @sa      CharacterAI::gotoFP
     * @sa      CharacterAI::GotoNextFP
     * @sa      World::isFPAvailable
     * @sa      World::isNextFPAvailable
     */
    bool isOnFreepoint(const bs::String& namePart);

    /**
     * Checks if a character object can be seen (direct).
     *
     * The \p Origin can see the \p Target if the distance
     * is not greater than the \b C_NPC.senses_range of the \p Origin,
     * there are no opaque static world polys between the objects,
     * and the angle to the \p Target is 90 degrees or less.
     *
     * @note    A \p Target that is hiding in a barrel cannot be seen
     *          (current interactive object is in state 1
     *          and the scheme name contains the substring "BARREL").
     *
     * @param   Origin
     *              Object reference to the origin character.
     * @param   Target
     *              Object reference to the target character.
     * @return  Returns TRUE if the \p Target can be seen by the \p Origin.
     * @sa      AI_CanSeeNpc
     * @sa      Npc_CanSeeItem
     * @sa      Npc_CanSeeNpcFreeLOS
     */
    bool canSeeNPC(bs::HSceneObject targetCharacterSO);

    /**
     * Checks if a character is in free line of sight (direct).
     *
     * The target is in free line of sight if the distance
     * is not greater than the `C_NPC.senses_range` of the character,
     * and there are no opaque static world polys between the objects.
     *
     * @note    A Target that is hiding in a barrel cannot be seen
     *          (current interactive object is in state 1
     *          and the scheme name contains the substring "BARREL").
     *
     * @param   targetCharacterSO  Object reference to the target character.
     *
     * @return  Returns TRUE if the Target is in free line of sight.
     *
     * @sa      CharacterAI::canSeeNpc
     * @sa      Character::canSeeItem
     * @sa      Character::canSeeNpc
     */
    bool canSeeNpcFreeLOS(bs::HSceneObject targetCharacterSO);

    /**
     * Checks if an item object can be seen (direct).
     *
     * This Character can see the target `itemSO` if the distance
     * is not greater than the `C_NPC.senses_range` of this character,
     * there are no opaque static world polys between the objects,
     * and the angle to the target `itemSO` is 90 degrees or less.
     *
     * @param   itemSO  Object reference to the item.
     *
     * @return  Returns TRUE if the target `itemSO` can be seen by the character.
     *
     * @sa      CharacterAI::canSeeNpc
     * @sa      canSeeNpc
     * @sa      canSeeNpcFreeLOS
     */
    bool canSeeItem(bs::HSceneObject itemSO);

    void clearAiQueue();

    /**
     * Exchange daily routine (direct).
     *
     * If a parser symbol with the name "RTN_" + `RoutineName` + "_" + `C_NPC.id` is
     * found:
     *
     *  1. all daily routine entries for the Character are removed
     *  2. the active entry in the routine manager is updated
     *  3. \b C_NPC.daily_routine is set for the Character
     *  4. the global instance variable \b SELF is set to Character
     *  5. \b C_NPC.daily_routine of the Character is called
     *  6. the daily routine waybox list for the Character is created
     *  7. the current daily routine entry is searched and enabled
     *  8. the active entry in the routine manager is updated again.
     *
     * @warning The game only checks if a parser symbol with the daily routine
     *          function name exists, but does not verify the symbol type.
     *
     * The daily routine function is expected to use \ref TA and \ref TA_Min to
     * add daily routine entries for the Character.  If the daily routine
     * function did not add any entries and the current state is caused by a
     * daily routine (or the current and next state are both invalid), the
     * message queue for the Character is cleared, the Character is interrupted,
     * and the current/last AI state is started (`C_NPC.start_aistate`).  If a
     * daily routine entry is found for the current world time (step 7) and the
     * current state is caused by a daily routine (or the current and next state
     * are both invalid) and the message queue is empty (or only contains
     * overlay messages):
     *
     *  1. the message queue for the Character is cleared
     *  2. the Character is interrupted
     *  3. the found entry is enabled (includes setting `C_NPC.wp`)
     *  4. and if the message queue is still empty (or only contains overlay messages)
     *     the AI state for the entry is started.
     *
     * The last condition is important, because interrupting the Character (hit
     * animation, spell, object interaction, interact item, higher animation
     * layers, talk, voices, trade, weapon mode) might leave non-overlay
     * messages in the queue.
     *
     * @note   The global instance variables \b SELF, \b OTHER, and \b VICTIM
     *         are saved and restored during the function call.
     *
     * @param  routineName  Base name of the new daily routine to enable
     *                      (always converted to upper case).
     *
     * @sa     AI_ContinueRoutine
     * @sa     AI_StartState
     * @sa     Npc_IsInRoutine
     * @sa     Rtn_Exchange
     * @sa     TA
     * @sa     TA_BeginOverlay
     * @sa     TA_EndOverlay
     * @sa     TA_Min
     */
    void exchangeRoutine(const bs::String& routineName);

    /**
     * Returns the name of the nearest waypoint (direct).
     *
     * @return  Returns the name of the nearest waypoint or an empty string
     *          if no waypoint has been found.

     * @sa      AI_AlignToWP
     * @sa      AI_GotoWP
     * @sa      Npc_GetDistToWP
     * @sa      Npc_GetNextWP
     */
    bs::String getNearestWaypoint();

    /**
     * Returns the name of the second nearest waypoint (direct).
     *
     * @param   Origin  Reference to the character object.
     *
     * @return  Returns the name of the second nearest waypoint
     *          or an empty string if no waypoint has been found.
     *
     * @sa      AI_AlignToWP
     * @sa      AI_GotoWP
     * @sa      Npc_GetDistToWP
     * @sa      Npc_GetNearestWP
     */
    bs::String getNextWaypoint();

    void setRefuseTalk(bs::INT32 durationSeconds);
    void refuseTalk();

    /**
     * Check if an AI state is active (direct).
     *
     * The function first checks if the current state is valid
     * and compares the active state function with \p AIState.
     * If the current state is invalid, the next state is tested.
     * \p AIState is compared with the base function of the state.
     * Therefore the function also returns TRUE if \p AIState is
     * \b ZS_Dead and the \p NonPlayer is currently in
     * \b ZS_Dead_Loop or \b ZS_Dead_End.
     *
     * @note    Some AI states are started by the engine and
     *          can be tested with the thair internal numbers:
     *          \arg \c -2 \b ZS_Answer
     *          \arg \c -3 \b ZS_Dead
     *          \arg \c -4 \b ZS_Unconscious
     *          \arg \c -5 \b ZS_FadeAway
     *          \arg \c -6 \b ZS_Follow
     *
     *
     * @param   AIState Base function of the AI state.
     *
     * @return  Returns TRUE if the current or next state is \p AIState.
     *
     * @sa      AI_ContinueRoutine
     * @sa      AI_StartState
     * @sa      Npc_ExchangeRoutine
     * @sa      Npc_IsInRoutine
     * @sa      Npc_WasInState
     * @sa      Rtn_Exchange
     */
    bool isInState(const bs::INT32 state);

    bool hasEquippedMelleWeapon();

    bool knowsInfo(const bs::String& instance);

    void clearInventory();
    bool hasItem(const bs::String& instance);
    void giveItem(const bs::String& instance, bs::HSceneObject characterToSO);
    void removeInventoryItems(const bs::String& instance, bs::INT32 amount);
    bool checkInfo(bool important);

    bs::INT32 GetStateTime();

    /************************************************************************/
    /* 								RTTI */
    /************************************************************************/
  public:
    friend class RTTI_Character;
    static bs::RTTITypeBase* getRTTIStatic();
    bs::RTTITypeBase* getRTTI() const override;

    // protected:
  public:  // FIXME: Should be protected, it is only used by RRIT but `friend` doesn't seem to work?!
    Character() = default;  // Serialization only
  };
}  // namespace REGoth
