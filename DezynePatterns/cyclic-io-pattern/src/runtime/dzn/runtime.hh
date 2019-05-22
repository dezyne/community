#ifndef DZN_RUNTIME_HH
#define DZN_RUNTIME_HH

#include <dzn/meta.hh>
#include <dzn/locator.hh>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <tuple>

inline std::string to_string(bool b){return b ? "true" : "false";}
inline std::string to_string(int i){return std::to_string(i);}

namespace dzn
{
  extern std::ostream debug;

  void trace_in(std::ostream&, port::meta const&, const char*);
  void trace_out(std::ostream&, port::meta const&, const char*);

  inline void apply(const dzn::meta* m, const std::function<void(const dzn::meta*)>& f)
  {
    f(m);
    for (auto c : m->children)
    {
      apply(c, f);
    }
  }

  inline void check_bindings(const dzn::meta* c)
  {
    apply(c, [](const dzn::meta* m){
        std::for_each(m->ports_connected.begin(), m->ports_connected.end(), [](const std::function<void()>& p){p();});
      });
  }

  inline void dump_tree(std::ostream& os, const dzn::meta* c)
  {
    apply(c, [&](const dzn::meta* m){
        os << path(m) << ":" << m->type << std::endl;
      });
  }

  struct runtime
  {
    std::map<void*, std::tuple<bool, void*, std::queue<std::function<void()> >, bool> > queues;

    bool external(void*);
    bool& handling(void*);
    void*& deferred(void*);
    std::queue<std::function<void()> >& queue(void*);
    bool& performs_flush(void* scope);
    void flush(void*);
    void defer(void*, void*, const std::function<void()>&);
    template <typename T>
    struct scoped_value
    {
      T& current;
      T initial;
      scoped_value(T& current, T value)
      : current(current)
      , initial(current)
      { current = value; }
      ~scoped_value()
      {
        current = initial;
      }
    };
    template <typename L, typename = typename std::enable_if<std::is_void<typename std::result_of<L()>::type>::value>::type>
    void handle(void* scope, L&& l)
    {
      bool& handle = handling(scope);
      if(handle) throw std::logic_error("component already handling an event");
      {scoped_value<bool> sv(handle, true);
        l();}
      flush(scope);
    }
    template <typename L, typename = typename std::enable_if<!std::is_void<typename std::result_of<L()>::type>::value>::type>
    inline auto handle(void* scope, L&& l) -> decltype(l())
    {
      bool& handle = handling(scope);
      if(handle) throw std::logic_error("component already handling an event");
      decltype(l()) r;
      {scoped_value<bool> sv(handle, true);
        r = l();}
      flush(scope);
      return r;
    }
    runtime();
  private:
    runtime(const runtime&);
    runtime& operator = (const runtime&);
  };

  void collateral_block(const locator&);
  void port_block(const locator&, void*);
  void port_release(const locator&, void*, std::function<void()>&);

  template <typename C>
  struct call_helper
  {
    C* c;
    std::ostream& os;
    const dzn::port::meta& meta;
    const char* event;
    std::string reply;
    call_helper(C* c, const dzn::port::meta& meta, const char* event)
    : c(c)
    , os(c->dzn_locator.template get<typename std::ostream>())
    , meta(meta)
    , event(event)
    , reply("return")
    {
      trace_in(os, meta, event); os << " " << *c << std::endl;
      if(c->dzn_rt.handling(c))
      {
        collateral_block(c->dzn_locator);
      }
    }
    template <typename L, typename = typename std::enable_if<std::is_void<typename std::result_of<L()>::type>::value>::type>
    void operator()(L&& l)
    {
      c->dzn_rt.handle(c, l);
    }
    template <typename L, typename = typename std::enable_if<!std::is_void<typename std::result_of<L()>::type>::value>::type>
    auto operator()(L&& l) -> decltype(l())
    {
      auto r = c->dzn_rt.handle(c, l);
      reply = ::to_string(r);
      return r;
    }
    ~call_helper()
    {
      trace_out(os, meta, reply.c_str()); os << " " << *c << std::endl;
    }
  };

  template <typename C, typename L>
  auto call_in(C* c, L&& l, const dzn::port::meta& meta, const char* event) -> decltype(l())
  {
    call_helper<C> helper(c, meta, event);
    return helper(l);
  }

  template <typename C, typename L>
  void call_out(C* c, L&& l, const dzn::port::meta& meta, const char* event)
  {
    auto& os = c->dzn_locator.template get<typename std::ostream>();
    trace_out(os, meta, event); os << " " << *c << std::endl;
    c->dzn_rt.defer(meta.provides.address, c, l);
  }
}
#endif //DZN_RUNTIME_HH
//version: 2.8.2

