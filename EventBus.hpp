#pragma once
#include <cassert>
#include <string>
#include <vector>

struct Event {
   void* data;  // Todo: Maybe add more? A void* should technically cover all bases, but it feels a little cheap.
   // Maybe use Urho3D style hashvariantmaps?
};

class EventBus {
  public:
   using EventID = uint16_t;

   // Since these are static, C++11 should mandate that they're thread safe, but will that also slow stuff down?
   static EventID RegisterEvent(const std::string& name) {
      auto& instance = Instance();

      EventID newID = instance.eventTypeNames.size();

      // Add the event to the list
      instance.eventTypeNames.push_back(name);
      // Make sure we have a matching entry in events
      instance.events.push_back(std::vector<Event>());
      assert(instance.events.size() == (newID + 1));

      return newID;
   }

   static void PushEvent(EventID type, Event ev) { Instance().events[type].push_back(ev); }

   // Returning as const vec ref so we can directly iterate over it.
   static const std::vector<Event>& GetEvents(EventID type) { return Instance().events[type]; }

  private:
   static EventBus& Instance() {
      static EventBus* bus = new EventBus();
      return *bus;
   }

   // TODO: Transition to an NVector
   std::vector<std::string>        eventTypeNames;
   std::vector<std::vector<Event>> events;
};
