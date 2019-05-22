#ifndef DZN_CONTEXT_HH
#define DZN_CONTEXT_HH

#include <cassert>
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <stdexcept>
#include <thread>

namespace dzn
{
  extern std::ostream debug;

class context
{
  enum State {INITIAL, RELEASED, BLOCKED, FINAL};
  static std::string to_string(State state)
  {
    switch(state)
    {
    case INITIAL: return "INITIAL";
    case RELEASED: return "RELEASED";
    case BLOCKED: return "BLOCKED";
    case FINAL: return "FINAL";
    }
    throw std::logic_error("UNKNOWN STATE");
  }
  State state;
  std::function<void()> rel;
  std::function<void(std::function<void(context&)>&)> work;
  std::mutex mutex;
  std::condition_variable condition;
  std::thread thread;
public:
  struct forced_unwind: public std::runtime_error
  {
    forced_unwind(): std::runtime_error("forced_unwind") {}
  };
  context()
  : state(INITIAL)
  , work()
  , mutex()
  , condition()
  , thread([this] {
      try
      {
        debug << "[" << get_id () << "] create" << std::endl;
        std::unique_lock<std::mutex> lock(mutex);
        while(state != FINAL)
        {
          do_block(lock);
          if(state == FINAL) break;
          if(!this->work) break;
          lock.unlock();
          std::function<void(context&)> yield([this](context& c){ this->yield(c); });
          assert(yield);
          this->work(yield);
          lock.lock();
          if(state == FINAL) break;
          if(this->rel) this->rel();
        }
      }
      catch(const forced_unwind&) { debug << "ignoring forced_unwind" << std::endl; }
    })
  {
    std::unique_lock<std::mutex> lock(mutex);
    while(state != BLOCKED) condition.wait(lock);
  }
  context(bool)
  : state(INITIAL)
  , work()
  , mutex()
  , condition()
  , thread()
  {}
  context(context&&) = delete;
  context& operator=(context&&) = delete;
  context(const context&) = delete;
  context& operator=(const context&) = delete;
  template <typename Work>
  context(Work&& work)
  : context()
  {
    std::unique_lock<std::mutex> lock(mutex);
    this->work = std::move(work);
  }
  ~context()
  {
    std::unique_lock<std::mutex> lock(mutex);
    do_finish(lock);
  }
  static size_t get_id()
  {
    static std::mutex mutex;
    static std::map<std::thread::id, size_t> m;
    std::unique_lock<std::mutex> lock(mutex);
    std::thread::id i = std::this_thread::get_id();
    auto it = m.find(i);
    if (it == m.end()) it = m.emplace(i, m.size()).first;
    return it->second;
  }
  void finish()
  {
    std::unique_lock<std::mutex> lock(mutex);
    do_finish(lock);
  }
  void block()
  {
    std::unique_lock<std::mutex> lock(mutex);
    do_block(lock);
  }
  void release()
  {
    std::unique_lock<std::mutex> lock(mutex);
    do_release(lock);
  }
  void call(context& c)
  {
    debug << "[" << get_id () << "] call" << std::endl;
    std::unique_lock<std::mutex> lock(mutex);
    do_release(lock);

    std::unique_lock<std::mutex> lock2(c.mutex);
    c.state = BLOCKED;

    lock.unlock();

    do { c.condition.wait(lock2); } while(c.state == BLOCKED);
  }
  void yield(context& to)
  {
    if(&to == this) return;
    std::unique_lock<std::mutex> lock(mutex);
    to.release();
    do_block(lock);
  }
private:
  void do_block(std::unique_lock<std::mutex>& lock)
  {
    state = BLOCKED;
    condition.notify_one();
    debug << "[" << get_id () << "] do_block0" << std::endl;
    do { condition.wait(lock); } while(state == BLOCKED);
    debug << "[" << get_id () << "] do_block1" << std::endl;
    if(state == FINAL) throw forced_unwind();
  }
  void do_release(std::unique_lock<std::mutex>&)
  {
    if(state != BLOCKED)
      throw std::runtime_error("not allowed to release a call which is " +
                               to_string(state));
    state = RELEASED;
    debug << "[" << get_id () << "] do_release0" << std::endl;
    condition.notify_one();
    debug << "[" << get_id () << "] do_release1" << std::endl;
  }
  void do_finish(std::unique_lock<std::mutex>& lock)
  {
    debug << "[" << get_id () << "] finish0" << std::endl;
    state = FINAL;
    lock.unlock();
    condition.notify_all();
    if(thread.joinable()) thread.join();
  }
};
}

#endif //DZN_CONTEXT_HH
//version: 2.8.2

