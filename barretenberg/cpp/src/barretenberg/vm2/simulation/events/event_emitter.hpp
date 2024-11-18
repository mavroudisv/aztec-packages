#pragma once

#include <list>

namespace bb::avm::simulation {

template <typename Event> class EventEmitterInterface {
  public:
    // EventEmitterInterface() = 0;
    // virtual ~EventEmitterInterface() = default;
    virtual Event& emit(Event&& event) = 0;
};

template <typename Event> class EventEmitter : public EventEmitterInterface<Event> {
  public:
    EventEmitter() = default;
    virtual ~EventEmitter() = default;
    Event& emit(Event&& event) override
    {
        events.push_back(std::move(event));
        return events.back();
    };

  private:
    // Using a list to have pointer stability.
    // This is currently not used, but might be needed for interactions.
    std::list<Event> events;
};

// TODO: one for all?
template <typename Event> class DiscardingEventEmitter : public EventEmitterInterface<Event> {
  public:
    DiscardingEventEmitter() = default;
    virtual ~DiscardingEventEmitter() = default;
    Event& emit(Event&& event) override { return event; };
};

} // namespace bb::avm::simulation