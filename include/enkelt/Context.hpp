#pragma once
/*
 * A simple context/base object style library.
 * Heavily inspired by Urho3D's core, but modernized.
 *
 */


#include <string>
#include <unordered_map>
#include <vector>

#include "type_index.hpp"


class Obj;
class Context;


class Context {
  public:
   Context();


   /// \brief Register a type so it can be easily recreated
   /// \tparam T MUST take a Context* in a constructor (and preferably send it to the Object as a subclass)
   template <typename T>
   void registerType(const std::string& typeName) {
      typeNames_[typeName]       = typeindex<T>();
      factories_[typeindex<T>()] = new T(this);
   }

   /// \brief Register a context-global object instance. Only 1 of each type allowed.
   /// \tparam T must ALREADY be registered as a type.
   template <typename T>
   void registerSubsystem() {
      subsystems_[typeindex<T>()] = factories_[typeindex<T>()]->clone();
      subsystems_[typeindex<T>()]->init();
   }

   template <typename T>
   T* getSubsystem() {
      return subsystems_[typeindex<T>()];
   }

  private:
   std::unordered_map<std::string, std::type_index> typeNames_;

   std::unordered_map<std::type_index, Obj*> factories_;
   std::unordered_map<std::type_index, Obj*> subsystems_;
};

class Obj {
  public:
   Obj(Context* ctx) : ctx_{ctx} {}
   inline virtual ~Obj() {}
   template <typename T>
   T* getSubsystem() {
      return ctx_->getSubsystem<T>();
   }

   virtual Obj* clone() = 0;

  private:
   Context* ctx_;
};
// Inherit from this to exploit the CRTP and have auto context registration.
// If you want to use multiple contexts, you CANNOT inherit from this class, as it implements a static flag.
template <typename OBJTYPE>
class Object {
  public:
   Object(Context* ctx) {
      if (!isRegistered())
         ctx_->registerType<OBJTYPE>();
   }



  private:
   Context* ctx_;

   // Check if this class is registered by the CRTP
   static bool& isRegistered() {
      static bool val = false;
      return val;
   }
};
