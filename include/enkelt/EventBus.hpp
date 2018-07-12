#pragma once
#include <cassert>
#include <string>
#include <typeindex>
#include <variant>
#include <vector>

// Since std c++ doesn't have these for some stupid reason.
template <typename T>
inline std::type_index typeindex(const T&) {
   return std::type_index(typeid(T));
}

template <typename T>
inline std::type_index typeindex() {
   return std::type_index(typeid(T));
}

struct EventData {
   EventData() : data{nullptr} {}

   template <typename T>
   EventData(T dat) : data{dat} {}

   std::variant<void*, char, int, float> data;  // No matter how many defaults are added, void* will always be
                                                // index 0
   // Maybe use Urho3D style hashvariantmaps?
   static EventData Empty;
};

EventData EventData::Empty = {nullptr};

class EventBus {
  public:
   using EventID = uint16_t;
   // Since these are static, C++11 should mandate that they're thread safe, but will that also slow stuff down?


   /// \brief Register a new event name, like "Update" or "FixedProcess".
   /// \note For now, there is no way to de-register an event name. Will likely be added in the future.
   static EventID RegisterEvent(const std::string& name) {
      auto& instance = Instance();

      EventID newID = instance.eventTypeNames.size();

      // Add the event to the list
      instance.eventTypeNames.push_back(name);
      // Make sure we have a matching entry in events
      instance.events.push_back(std::vector<EventData*>());
      assert(instance.events.size() == (newID + 1));

      return newID;
   }

   /// \brief Add an [ev]ent to the [type]'s queue
   static void PushEvent(EventID type, EventData* ev) { Instance().events[type].push_back(ev); }

   /// \brief Get a constant yet iterable list of all events of a certain type.
   static const std::vector<EventData*>& GetEvents(EventID type) { return Instance().events[type]; }

   /// \brief Check if there are any events to get
   static const bool HasEvents(EventID type) { return Instance().events[type].size() != 0; }

   /// \brief Get rid of all events. Intended to be called after everyone has had a chance to GetEvents()
   static const void FlushEvents() {
      for (auto& eventType : Instance().events)
         eventType.resize(0);  // Set the size to 0 without removing old stuff. *Should* be faster than a clear
   }

  private:
   static EventBus& Instance() {
      static EventBus* bus = new EventBus();
      return *bus;
   }

   // TODO: Transition to an NVector
   std::vector<std::string>             eventTypeNames;
   std::vector<std::vector<EventData*>> events;
};
