#pragma once
#include "EventQueue.hpp"
#include <AI/Pathfinder.hpp>
#include <AI/ScriptState.hpp>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  class CharacterAI;
  using HCharacterAI = bs::GameObjectHandle<CharacterAI>;

  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  class CharacterEventQueue;
  using HCharacterEventQueue = bs::GameObjectHandle<CharacterEventQueue>;

  class VisualCharacter;
  using HVisualCharacter = bs::GameObjectHandle<VisualCharacter>;

  namespace AI
  {
    class Pathfinder;
  }  // namespace AI

  /**
   * Implmementaion of the EventQueue for Character-related messages.
   *
   * This component is attached to Character-objects and handles the
   * messages targeted at characters. See the EventQueue component for
   * more information.
   */
  class CharacterEventQueue : public EventQueue
  {
  public:
    CharacterEventQueue(const bs::HSceneObject& parent, HCharacter character,
                        HCharacterAI characterAI, HVisualCharacter visualCharacter,
                        HGameWorld world);
    virtual ~CharacterEventQueue();

    /**
     * Push a message to go to a position into the queue.
     */
    SharedEMessage pushGotoPosition(const bs::Vector3& position);

    /**
     * Push a message to go to an object into the queue.
     */
    SharedEMessage pushGotoObject(bs::HSceneObject object);

    /**
     * Push a message to set the walk-mode into the queue.
     */
    SharedEMessage pushSetWalkMode(AI::WalkMode walkMode);

    /**
     * Push a message which lets the queue execution wait for the given time.
     */
    SharedEMessage pushWait(float seconds);

    /**
     * Push a message which lets the character talk to the given other character.
     *
     * This is used to initiate a dialogue (with UI) between the player and an NPC. To do this,
     * you would need to call pushTalkToCharacter() as seen from the Character the
     * player wants to talk to.
     *
     * So, suppose you want the player to talk to Diego, you need to call pushTalkToCharacter()
     * on Diego, with `other` set to the players handle.
     */
    SharedEMessage pushTalkToCharacter(HCharacter other);

    /**
     * Push a message which starts a new script state after ending the current one gracefully.
     */
    SharedEMessage pushStartScriptState(const bs::String& state, const bs::String& waypoint,
                                        HCharacter other, HCharacter victim);
    /**
     * Push a message which interrupts the currently active script state starts a new one.
     */
    SharedEMessage pushInterruptAndStartScriptState(const bs::String& state,
                                                    const bs::String& waypoint, HCharacter other,
                                                    HCharacter victim);

    /**
     * Push a message which will make the character play an animation.
     */
    SharedEMessage pushPlayAnimation(const bs::String animation);

    /**
     * Push a message which will make the character go into fist-mode immediately.
     */
    SharedEMessage pushGoToFistModeImmediate();

    /**
     * Insert a new routine task. See AI::ScriptState::insertRoutineTask().
     */
    void insertRoutineTask(const AI::ScriptState::RoutineTask& task);

    /**
     * Queries the routine supposed to be active from the characters script
     * object and starts it.
     */
    void reinitRoutine();

  protected:
    void onInitialized() override;

    /**
     * Uses the Pathfinder to start a new route to the given location.
     */
    void startRouteToPosition(const bs::Vector3& target);

    /**
     * Uses the Pathfinder to start a new route to the given object.
     */
    void startRouteToObject(bs::HSceneObject target);

    /**
     * Lets the Character move along the currently active route in the Pathfinder.
     */
    void travelActiveRoute();

    /**
     * @return The scene objects position
     */
    const bs::Vector3& positionNow() const;

    /**
     * Called cyclically for the first message in the queue. Override this
     * in the more specialized classes to handle messages meant for characters
     * or interactive objects.
     *
     * Don't forget to flag the message as "done" when the action was executed
     * completely, e.g. when the animation is done.
     */
    virtual void onExecuteEventAction(SharedEMessage message, bs::HSceneObject sender) override;

    /**
     * Cyclic update
     */
    virtual void fixedUpdate() override;

  private:
    bool EV_Event(AI::EventMessage& message, bs::HSceneObject sender);
    bool EV_Npc(AI::NpcMessage& message, bs::HSceneObject sender);
    bool EV_Damage(AI::DamageMessage& message, bs::HSceneObject sender);
    bool EV_Weapon(AI::WeaponMessage& message, bs::HSceneObject sender);
    bool EV_Movement(AI::MovementMessage& message, bs::HSceneObject sender);
    bool EV_Attack(AI::AttackMessage& message, bs::HSceneObject sender);
    bool EV_UseItem(AI::UseItemMessage& message, bs::HSceneObject sender);
    bool EV_State(AI::StateMessage& message, bs::HSceneObject sender);
    bool EV_Manipulate(AI::ManipulateMessage& message, bs::HSceneObject sender);
    bool EV_Conversation(AI::ConversationMessage& message, bs::HSceneObject sender);
    bool EV_Magic(AI::MagicMessage& message, bs::HSceneObject sender);

    /**
     * Quick access to other components attached to the SO of this component
     */
    HCharacter mCharacter;
    HCharacterAI mCharacterAI;
    HVisualCharacter mVisualCharacter;
    HGameWorld mWorld;
    bs::SPtr<AI::Pathfinder> mPathfinder;
    bs::SPtr<AI::ScriptState> mScriptState;

  public:
    REGOTH_DECLARE_RTTI(CharacterEventQueue)

  protected:
    CharacterEventQueue() = default;  // For RTTI
  };
}  // namespace REGoth
