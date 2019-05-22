#ifndef DZN_PUMP_HH
#define DZN_PUMP_HH

#include <dzn/coroutine.hh>

#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <map>
#include <mutex>
#include <queue>
#include <set>

namespace dzn
{
  extern std::ostream debug;

  struct pump
  {
    std::function <void()> collateral_block_lambda;
    std::function<void()> worker;
    std::list<coroutine> coroutines;
    std::list<coroutine> collateral_blocked;
    std::set<void*> skip_block;
    std::queue<std::function<void()>> queue;

    struct deadline
    {
      size_t id;
      std::chrono::steady_clock::time_point t;
      size_t rank;
      deadline(size_t id, size_t ms, size_t rank)
      : id(id)
      , t(std::chrono::steady_clock::now() + std::chrono::milliseconds(ms))
      , rank(rank)
      {}
      bool expired() const {return t <= std::chrono::steady_clock::now();}
      bool operator < (const deadline& d) const { return rank_less(d); }
    private:
      bool rank_less(const deadline& d) const {return rank < d.rank || rank == d.rank && time_less(d);}
      bool time_less(const deadline& d) const {return t < d.t || (t == d.t && id < d.id);}
    };

    std::map<deadline, std::function<void()>> timers;
    std::function<void()> switch_context;
    std::function<void()> exit;
    std::thread::id thread_id;
    bool running;
    std::condition_variable condition;
    std::condition_variable idle;
    std::mutex mutex;
    std::future<void> task;
    pump();
    ~pump();
    void stop();
    void wait();
    void operator()();

    void collateral_block();
    void collateral_release(std::list<coroutine>::iterator);

    void block(void*);
    void create_context();
    void release(void*);
    void operator()(const std::function<void()>&);
    void operator()(std::function<void()>&&);
    void handle(size_t id, size_t ms, const std::function<void()>&, size_t rank = std::numeric_limits<size_t>::max());
    void remove(size_t id);
  private:
    bool timers_expired() const;
    void remove_finished_coroutines();
  };

  template <typename L, typename ... Args, typename = typename std::enable_if<std::is_void<typename std::result_of<L()>::type>::value>::type>
  void shell(dzn::pump& pump, L&& l, Args&& ...args)
  {
    std::promise<void> p;
    pump([&]{l(std::forward<Args>(args)...); p.set_value();});
    return p.get_future().get();
  }
  template <typename L, typename ... Args, typename = typename std::enable_if<!std::is_void<typename std::result_of<L()>::type>::value>::type>
  auto shell(dzn::pump& pump, L&& l, Args&& ...args) -> decltype(l(std::forward<Args>(args)...))
  {
    std::promise<decltype(l(std::forward<Args>(args)...))> p;
    pump([&]{p.set_value(l(std::forward<Args>(args)...));});
    return p.get_future().get();
  }
}

#endif //DZN_PUMP_HH
//version: 2.8.2

