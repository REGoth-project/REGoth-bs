#pragma once

#include <AI/EventMessage.hpp>
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class EventHandler;
  using HEventHandler = bs::GameObjectHandle<EventHandler>;

  /**
   * Event Queue Component.
   *
   * The Event Queue implements a queue, in which events can be stored. The term
   * "Events" might be a bit misleading, but that is the name used in the
   * original engine. Think of them more like *Actions*, which can take some
   * time to execute.
   *
   * Note that the terms "event" and "message" mean the same thing in this
   * context.
   *
   *
   * Event Overview
   * ==============
   *
   * An Event can be something like "Go to Waypoint" or "Talk to Diego". By
   * queuing events you can create complex sequences like:
   *
   *  - Go to Waypoint X
   *  - Turn to Object Y
   *  - Take a piss
   *  - Play a farting sound
   *
   * Each of those events will be handled by the underlaying object, which might
   * be a character, but could also be something else. Interactive Objects use
   * the EventQueue as well.
   *
   * A event at the front of the queue is passed to the targeted object every
   * update cycle. The object will look at the event and perform what the event
   * says it should do. Since some actions can take longer, like walking
   * somewhere or playing an animation, the object can decide that the event
   * should stay active. The event is only popped from the queue, if the object
   * says it's done with it.
   *
   *
   * Overlay Events
   * ==============
   *
   * Usually, only the event at the front of the queue is active and passed to
   * the target object. However, events marked as "Overlay" will act a little
   * bit different.
   *
   * If such an overlay-event is at the front of the queue, it *will* be passed
   * to the target object every update cycle. Then, the next event in the queue
   * is look at. If that is also an overlay-event, it is passed to the object as
   * well. This continues until the first event is encountered which is not an
   * overlay. That will be the last event passed to the object.
   *
   *
   * User Implementation
   * ===================
   *
   * The EventQueue-component itself is kept fairly generic. To actually handle
   * incomming events, you need to create a component inheriting from this,
   * where you override the onExecuteEventAction() method.
   */
  class EventQueue : public bs::Component
  {
  public:
    using SharedEMessage = bs::SPtr<AI::EventMessage>;

    EventQueue(const bs::HSceneObject& parent);

    /**
     * To be called when a scene object wants to send a message to some
     * other scene object.
     *
     * @param  msg     Message to send. Will be copied internally.
     * @param  sender  Object that sent the message. Can be empty.
     *
     * @return The registered message (Copy of msg). This can be used to
     *         register callbacks, for example.
     */
    template <typename T>
    bs::SPtr<T> onMessageFromObject(const T& msg, bs::HSceneObject sender)
    {
      // Copy over the data from the given message
      auto copyDerived = bs::bs_shared_ptr_new<T>(msg);

      // Handle the message and potentially add it to the queue
      handleMessage(copyDerived, sender);
      return copyDerived;
    }

    /**
     * To be called when a message without an explicit sending object is to send.
     *
     * @param  msg     Message to send. Will be copied internally.
     *
     * @return The registered message (Copy of msg). This can be used to
     *         register callbacks, for example.
     */
    template <typename T>
    bs::SPtr<T> onMessage(const T& msg)
    {
      return onMessageFromObject(msg, {});
    }

    /**
     * @return Whether the message queue is currently empty.
     */
    bool isEmpty();

    /**
     * Searches the messages for the last one that is a conversation message and has
     * the character `other` as conversation target.
     *
     * @param  other  Character to search for.
     *
     * @return Pointer to the last non-overlay conv-message, if found. If not, nullptr.
     */
    SharedEMessage findLastConversationMessageWith(bs::HSceneObject other);

    /**
     * Searches the messages if any conversation message exists, that has the Character
     * `other` as target.
     *
     * @param  other  Character to search for
     *
     * @return True, if such a message was found.
     */
    bool hasConversationMessageWith(bs::HSceneObject other);

    /**
     * Blocks the event-queue until the given event was processed. This is usually used
     * to have one Character wait for an other Character to finish some action.
     *
     * @param other Message to wait for
     */
    void waitForMessage(SharedEMessage other);

    /**
     * Removes all pending messages
     */
    void clear();

  protected:
    /**
     * Called cyclically for the first message in the queue. Override this
     * in the more specialized classes to handle messages meant for characters
     * or interactive objects.
     *
     * Don't forget to flag the message as "done" when the action was executed
     * completely, e.g. when the animation is done.
     */
    virtual void onExecuteEventAction(SharedEMessage message, bs::HSceneObject sender) = 0;

    /**
     * Cyclic update
     */
    virtual void update() override;

  private:
    /**
     * Processes a single step of the message queue.
     *
     * Will inform the messages target object about the message and let it
     * decide whether the message is still going or whether the action is
     * completed.
     *
     * @note This might lead to the destruction of the host-vob and this
     *       object in response to a message!
     *
     */
    void processMessageQueue();

    /**
     * Handles an incomming message. This will decide whether or not to immediately execute the
     * message or whether it should wait in the queue. Immediate messages will be flagged as
     * to-delete after execution
     *
     * @param message Message to handle
     */
    void handleMessage(SharedEMessage message, bs::HSceneObject sender);

    /**
     * Sends the given message to the host-vob
     */
    void sendMessageToHost(SharedEMessage message, bs::HSceneObject sender);

    /**
     * To be called right after a message arrived. This function will tell you
     * whether the event should be handled now, instantly. Those one-shot
     * messages are also to be deleted after they have been handled.
     */
    bool shouldExecuteMessageInstantly(SharedEMessage message) const;

    /**
     * Events registered and managed here.
     */
    bs::Vector<SharedEMessage> mEventQueue;

  public:
    REGOTH_DECLARE_RTTI(EventQueue)

  protected:
    EventQueue() = default;  // For RTTI
  };
}  // namespace REGoth
