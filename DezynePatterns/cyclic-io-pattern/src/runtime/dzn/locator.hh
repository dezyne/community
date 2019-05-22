#ifndef DZN_LOCATOR_HH
#define DZN_LOCATOR_HH

#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace dzn {
  struct illegal_handler {std::function<void()> illegal = [] {assert(!"illegal");};};

  struct locator
  {
  private:
    typedef std::string Key;
    struct type_info
    {
      const std::type_info* type;
      type_info(const std::type_info& t)
      : type(&t)
      {}
      bool operator < (const type_info& that) const
      {
        return type->before(*that.type);
      }
    };
    std::map<std::pair<Key,type_info>, const void*> services;
  public:
    locator()
    {
      static illegal_handler ih;
      set(std::clog).set(ih);
    }
    locator clone() const
    {
      return locator(*this);
    }
    template <typename T>
    locator& set(T& t, const Key& key = Key())
    {
      services[std::make_pair(key,type_info(typeid(T)))] = &t;
      return *this;
    }
    template <typename T>
    T* try_get(const Key& key = Key()) const
    {
      auto it = services.find(std::make_pair(key,type_info(typeid(T))));
      if(it != services.end() && it->second)
        return reinterpret_cast<T*>(const_cast<void*>(it->second));
      return 0;
    }
    template <typename T>
    T& get(const Key& key = Key()) const
    {
      if(T* t = try_get<T>(key))
        return *t;
      throw std::runtime_error("<" + std::string(typeid(T).name()) + ",\"" + key + "\"> not available");
    }
  };
}
#endif //DZN_LOCATOR_HH
//version: 2.8.2

