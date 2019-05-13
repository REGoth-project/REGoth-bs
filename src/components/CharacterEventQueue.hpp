#pragma once
#include "EventQueue.hpp"
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  class CharacterAI;
  using HCharacterAI = bs::GameObjectHandle<CharacterAI>;

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
    CharacterEventQueue(const bs::HSceneObject& parent);
    virtual ~CharacterEventQueue();

  protected:
    /**
     * Called cyclically for the first message in the queue. Override this
     * in the more specialized classes to handle messages meant for characters
     * or interactive objects.
     *
     * Don't forget to flag the message as "done" when the action was executed
     * completely, e.g. when the animation is done.
     */
    virtual void onExecuteEventAction(SharedEMessage message) override;

    /**
     * Cyclic update
     */
    virtual void fixedUpdate() override;

  private:

    /**
     * Quick access to other components attached to the SO of this component
     */
    HCharacter mCharacter;
    HCharacterAI mCharacterAI;

  public:
    REGOTH_DECLARE_RTTI(CharacterEventQueue)

  protected:
    CharacterEventQueue() = default;  // For RTTI
  };
}  // namespace REGoth
