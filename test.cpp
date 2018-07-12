#include <chrono>
#include <iostream>
#include <random>


#include "enkelt/Context.hpp"
#include "enkelt/EventBus.hpp"
#include "enkelt/TaskScheduler.hpp"

bool testEventBus() {
   auto quitEventID  = EventBus::RegisterEvent("Quit");
   auto printEventID = EventBus::RegisterEvent("PrintEvent");
   //   std::srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
   //   bool running = true;
   //   while (running) {
   //      int* num = new int;
   //      *num     = rand();
   //      if (*num % 10 == 0)
   //         EventBus::PushEvent(quitEventID, &EventData::Empty);  // No data that goes with quit
   //      else
   //         EventBus::PushEvent(printEventID, {num});

   //      if (EventBus::HasEvents(quitEventID)) {
   //         std::cout << "Requested quit!\n";
   //         running = false;
   //      }
   //   }

   //   for (const auto& arg : EventBus::GetEvents(printEventID)) {
   //      auto realPtr = static_cast<int*>(arg.data);
   //      std::cout << "Event requested to print: " << *realPtr << std::endl;
   //      delete realPtr;
   //   }

   return true;
}

int main() {
   // TODO: Real testing
   testEventBus();
   return 0;
}
