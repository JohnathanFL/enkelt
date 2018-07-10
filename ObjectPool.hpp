#pragma once
#include <stdint.h>
#include <vector>

#include "Logger.hpp"


/// \class Pool
/// \tparam The type to be pooled
/// \brief A simple, generic, non-fixed-size object pool.
///
/// Try to put all frequenty used/iterated objects in a pool, since this ensures locality.
template <typename T>
class Pool {
   using ID = uint16_t;
   Pool() {}

   size_t          increasePerAlloc = 10;
   std::vector<ID> freeObjects;
   std::vector<T>  pool;

   static Pool<T>* Instance() {
      static Pool<T>* instance = new Pool<T>();
      return instance;
   }

  public:
   static void Return(ID id) {
#ifdef DEBUG  // Save the cost of checking ourselves for only when needed.
      if (Instance()->freeObjects.find(id) == Instance()->freeObjects.end()) {
         Logger::Debug("Tried to double return an object with ID ", id);
      }
#endif

      Instance()->pool[id].deinit();
      Instance()->freeObjects.push_back(id);
   }
   template <typename... Args>
   static ID New(const Args&... args) {
      auto thisInstance = Instance();
      ID   newId;
      if (thisInstance->freeObjects.size() > 0) {  // We have free objects to reuse
         newId = Instance()->freeObjects.back();
         thisInstance->freeObjects.pop_back();
      } else {  // We need to make more
         newId = thisInstance->freeObjects.size();
         thisInstance->pool.reserve(thisInstance->increasePerAlloc);
         thisInstance->pool[newId].id = newId;
      }
      thisInstance->pool[newId].init(args...);
      return newId;
   }
   static T& Get(ID id) {
#ifdef DEBUG  // Save the cost of checking ourselves for only when needed.
      // We'll allow this to continue even if this check fails, since if it's in bounds it won't be too much of a
      // problem
      if (Instance()->freeObjects.find(id) == Instance()->freeObjects.end()) {
         Logger::Debug("Tried to get an already returned object with ID ", id);
      }
#endif

      return Instance()->pool[id];
   }
};

struct Pooled {
   using ID = uint16_t;
   ID id;
   // Leave it up to the subclass to specialize for what they need.
   template <typename... Args>
   void init(const Args&... args);

   // Object has been removed from use. Clean anything up before going dormant
   void deinit();
};
