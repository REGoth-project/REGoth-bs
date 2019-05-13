#pragma once
#include "EventQueue.hpp"
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
    CharacterEventQueue(const bs::HSceneObject& parent, HGameWorld world);
    virtual ~CharacterEventQueue();

    /**
     * Quick access to push a message to go to a position into the queue.
     */
    SharedEMessage pushGotoPosition(const bs::Vector3& position);

    /**
     * Quick access to push a message to go to an object into the queue.
     */
    SharedEMessage pushGotoObject(bs::HSceneObject object);

  protected:
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
    HGameWorld mWorld;

  public:
    REGOTH_DECLARE_RTTI(CharacterEventQueue)

  protected:
    CharacterEventQueue() = default;  // For RTTI
  };
}  // namespace REGoth
