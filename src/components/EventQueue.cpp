#include "EventQueue.hpp"
#include <RTTI/RTTI_EventQueue.hpp>
#include <Scene/BsSceneObject.h>
#include <exception/Throw.hpp>

namespace REGoth
{
  using SharedEMessage = EventQueue::SharedEMessage;

  EventQueue::EventQueue(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
  }

  void EventQueue::handleMessage(SharedEMessage message, bs::HSceneObject sender)
  {
    message->isFirstRun = true;

    if (shouldExecuteMessageInstantly(message))
    {
      // Pass the message to the host
      sendMessageToHost(message, sender);

      // Flag as done
      message->deleted = true;
    }
    else
    {
      mEventQueue.push_back(message);
    }
  }

  void EventQueue::sendMessageToHost(SharedEMessage message, bs::HSceneObject sender)
  {
    onExecuteEventAction(message);

    message->isFirstRun = false;
  }

  void EventQueue::processMessageQueue()
  {
    // Not using iterators here, because a message might get pushed inside a callback, which
    // would make them invalid. This has to be done before deleting the message for this very
    // reason.
    for (size_t i = 0, end = mEventQueue.size(); i < end; i++)
    {
      auto& event = mEventQueue[i];
      if (event->deleted)
      {
        event->onMessageDone(event);
      }
    }

    // Remove deleted messages from last time
    for (auto it = mEventQueue.begin(); it != mEventQueue.end();)
    {
      if ((*it)->deleted)
      {
        it = mEventQueue.erase(it);
      }
      else
      {
        it++;
      }
    }

    if (mEventQueue.empty()) return;

    // Process messages as far as we can
    for (SharedEMessage ev : mEventQueue)
    {
      bs::HSceneObject sender = {};  // TODO: Don't we need that?
      sendMessageToHost(ev, sender);

      // FIXME: This event manager could have been deleted as a reaction to the message! Take care
      // of that!

      // Mark as done if this wasn't a job
      if (!ev->isJob) ev->deleted = true;

      if (!ev->isOverlay) break;
    }
  }

  SharedEMessage EventQueue::findLastConversationMessageWith(bs::HSceneObject other)
  {
    auto begin = mEventQueue.rbegin();
    auto end   = mEventQueue.rend();

    auto lastConvMessageIterator = std::find_if(begin, end, [&](SharedEMessage ev) {
      if (!ev->isOverlay && ev->messageType == AI::EventMessageType::Conversation)
      {
        auto conv = reinterpret_cast<AI::ConversationMessage*>(ev.get());
        return conv->target == other;
      }
      return false;
    });

    if (lastConvMessageIterator == end) return nullptr;

    return *lastConvMessageIterator;
  }

  bool EventQueue::hasConversationMessageWith(bs::HSceneObject other)
  {
    return findLastConversationMessageWith(other) != nullptr;
  }

  bool EventQueue::shouldExecuteMessageInstantly(SharedEMessage message) const
  {
    if (message->isJob) return false;

    if (message->isHighPriority) return true;
    if (mEventQueue.empty()) return true;

    return false;
  }

  void EventQueue::waitForMessage(SharedEMessage other)
  {
    // Push a wait-message first
    AI::ConversationMessage wait;
    wait.subType        = AI::ConversationMessage::ST_WaitTillEnd;
    wait.waitIdentifier = other;

    // Let the EM wait for this talking-action to complete
    bs::SPtr<AI::ConversationMessage> queuedWait = onMessage(wait);

    other->onMessageDone.connect([queuedWait](SharedEMessage msg) {
      // Once this event fires, we're done waiting
      queuedWait->canceled = true;
    });
  }

  void EventQueue::clear()
  {
    for (SharedEMessage ev : mEventQueue)
    {
      ev->deleted = true;
    }
  }

  bool EventQueue::isEmpty()
  {
    for (SharedEMessage ev : mEventQueue)
    {
      if (!ev->deleted) return false;
    }

    return true;
  }

  REGOTH_DEFINE_RTTI(EventQueue)
}  // namespace REGoth
