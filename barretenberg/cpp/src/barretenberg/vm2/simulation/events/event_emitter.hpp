#pragma once

#include <list>

namespace bb::avm::simulation {

template <typename Event> class EventEmitterInterface {
  public:
    // TODO: DO NOT DO THIS. We shouldn't prescribe the base container.
    using Container = std::list<Event>;

    // EventEmitterInterface() = 0;
    // virtual ~EventEmitterInterface() = default;
    virtual Event& emit(Event&& event) = 0;

    virtual const Container& get_events() const = 0;
    virtual Container dump_events() = 0;
};

template <typename Event> class EventEmitter : public EventEmitterInterface<Event> {
  public:
    using Container = EventEmitterInterface<Event>::Container;

    EventEmitter() = default;
    virtual ~EventEmitter() = default;
    Event& emit(Event&& event) override
    {
        events.push_back(std::move(event));
        return events.back();
    };

    const Container& get_events() const override { return events; }
    Container dump_events() override { return std::move(events); }

  private:
    // Using a list to have pointer stability.
    // This is currently not used, but might be needed for interactions.
    Container events;
};

// TODO: one for all?
template <typename Event> class NoopEventEmitter : public EventEmitterInterface<Event> {
  public:
    using Container = EventEmitterInterface<Event>::Container;

    NoopEventEmitter() = default;
    virtual ~NoopEventEmitter() = default;

    Event& emit(Event&& event) override { return event; };
    const Container& get_events() const override
    {
        static Container empty;
        return empty;
    }
    Container dump_events() override { return {}; }
};

} // namespace bb::avm::simulation