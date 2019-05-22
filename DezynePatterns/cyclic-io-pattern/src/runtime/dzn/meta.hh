#ifndef DZN_META_HH
#define DZN_META_HH

#include <algorithm>
#include <functional>
#include <string>
#include <stdexcept>
#include <vector>

inline std::string to_string()
{
  return "return";
}

namespace dzn
{
#define DZN_VERSION_STRING "2.8.2"
int const version_major = 2;
int const version_minor = 8;
int const version_patch = 2;

  struct meta;

  namespace port
  {
    struct meta
    {
      struct
      {
        std::string port;
        void* address;
        const dzn::meta* meta;
      } provides;

      struct
      {
        std::string port;
        void* address;
        const dzn::meta* meta;
      } requires;
    };
  }

  struct meta
  {
    std::string name;
    std::string type;
    const meta* parent;
    mutable size_t rank;
    std::vector<const port::meta*> requires;
    std::vector<const meta*> children;
    std::vector<std::function<void()>> ports_connected;
  };

  inline void rank(const dzn::meta* m, size_t r)
  {
    if(m) {
      m->rank = std::max(m->rank, r);
      for(auto i : m->requires) rank(i->provides.meta, m->rank + 1);
    }
  }

  inline std::string path(const meta* m, std::string p = std::string())
  {
    p = p.empty() ? p : "." + p;
    if(!m) return "<external>" + p;
    if(!m->parent) return m->name + p;
    return path(m->parent, m->name + p);
  }

  struct binding_error: public std::runtime_error
  {
    binding_error(const port::meta& m, const std::string& msg)
    : std::runtime_error("not connected: " + path(m.provides.address ? m.provides.meta : m.requires.meta,
                                                  m.provides.address ? m.provides.port : m.requires.port) + "." + msg)
    {}
  };

  template <typename Signature>
  struct async
  {
    struct {
      std::function<Signature> req;
      std::function<void()> clr;
    } in;
    struct {
      std::function<Signature> ack;
    } out;

    dzn::port::meta meta;

    inline async(const dzn::port::meta& m) : meta(m) {}

    void check_bindings() const
    {
      if (! in.req) throw dzn::binding_error(meta, "in.req");
      if (! in.clr) throw dzn::binding_error(meta, "in.clr");
      if (! out.ack) throw dzn::binding_error(meta, "out.ack");
    }
  };
}
#endif //DZN_META_HH
//version: 2.8.2

