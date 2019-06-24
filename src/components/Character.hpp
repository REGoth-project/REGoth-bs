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
    Character(const bs::HSceneObject& parent, const bs::String& instance, HGameWorld gameWorld);

    void onInitialized() override;

    /**
     * Registers this character as the hero. The hero will most likely be the player,
     * but doesn't have to be, ie. if the player controls another character.
     *
     * Also sets this character to be referenced by `HERO` inside the scripts.
     */
    void useAsHero();

    /**
     * Sets this character to be referenced by `SELF` inside the scripts.
     */
    void useAsSelf();

    /**
     * Sets this character to be referenced by `VICTIM` inside the scripts.
     */
    void useAsVictim();

    /**
     * Sets this character to be referenced `OTHER` inside the scripts.
     */
    void useAsOther();

    /**
     * To be called from the ScriptState-module if this character is currently
     * Unconscious.
     */
    void handleUnconsciousness();

    /**
     * TODO: Called from the ScriptState-Module. See in old REGoth what isReady() means.
     *       Supposidly it "Checks for death, etc". So maybe a name of `isAliveAndWell()`
     *       would me more descriptive.
     */
    bool isReady();

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

    /**
     * @return The waypoint this character is around, needed by the scripts.
     */
    const bs::String& currentWaypoint() const;

    /**
     * Sets the waypoint this character is around, needed by the scripts.
     * This could be the waypoint of the active routine, for example.
     */
    void setCurrentWaypoint(const bs::String& waypoint);

    /**
     * @return On monsters, this will return the AI-state the monster should
     *         automatically start after spawning.
     *         On Humans, this will return an empty string.
     */
    const bs::String& getStartAIState();

    /**
     * @return Name of the script function to call for the daily routine.
     */
    bs::String dailyRoutine();

    /**
     * Sets the name of the daily routine this character should use to the characters script object.
     * Note that you will need to call CharacterEventQueue::reinitRoutine() to actually apply
     * the new routine.
     *
     * @param  newDailyRoutine  Name of the daily routine as used inside Gothic scripts.
     *                          This is actually a part of a script function name. For
     *                          example, if a routine name of "Start" is supplied and this
     *                          character has an ID of 456, then the final routine function
     *                          would be constructed as `Rtn_Start_456`, which needs to
     *                          be an actual script function.
     */
    void setDailyRoutine(const bs::String& newDailyRoutine);

    /**
     * @return If this is a monster, it will not use routines to manage it's actions, but
     *         rather start a script-state which dictates what the monster should do.
     *         So basically this should return TRUE, if this is monster,
     *         FALSE, if this is a human.
     */
    bool isStateDriven();

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

    /**
     * All *Information*-Instances for this Character. See DaedalusVMForGameWorld::allInfosOfNpc().
     *
     * This is more like raw data. Use the `StoryInformation`-component to actually work with these.
     */
    const bs::Vector<Scripting::ScriptObjectHandle>& allInfosForThisCharacter() const;

    /**
     * Returns a list of all characters standing near this character, in the specified range.
     *
     * @note This list will also include this character!
     */
    bs::Vector<HCharacter> findCharactersInRange(float range) const;

    bs::INT32 GetStateTime();

  public:
    REGOTH_DECLARE_RTTI(Character);

  protected:
    Character() = default;  // For RTTI
  };
}  // namespace REGoth
