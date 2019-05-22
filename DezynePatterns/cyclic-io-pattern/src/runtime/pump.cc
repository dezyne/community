#include <dzn/locator.hh>
#include <dzn/pump.hh>

#include <algorithm>
#include <cassert>

namespace dzn
{
  void port_block(const locator& l, void* p)
  {
    l.get<dzn::pump>().block(p);
  }
  void port_release(const locator& l, void* p, std::function<void()>& out_binding)
  {
    if(out_binding) out_binding();
    out_binding = nullptr;
    l.get<dzn::pump>().release(p);
  }
  void collateral_block(const locator& l)
  {
    l.get<dzn::pump>().collateral_block_lambda();
  }

  static std::list<coroutine>::iterator find_self(std::list<coroutine>& coroutines)
  {
    assert(1 == std::count_if(coroutines.begin(), coroutines.end(), [](const coroutine& c){return c.port == nullptr && !c.finished;}));
    auto self = std::find_if(coroutines.begin(), coroutines.end(), [](dzn::coroutine& c){return c.port == nullptr && !c.finished;});
    return self;
  }

  static std::list<coroutine>::iterator find_blocked(std::list<coroutine>& coroutines, void* port)
  {
    auto self = std::find_if(coroutines.begin(), coroutines.end(), [port](dzn::coroutine& c){return c.port == port;});
    return self;
  }

  static void finish(std::list<coroutine>& coroutines)
  {
    auto self = find_self(coroutines);
    self->finished = true;
    debug << "[" << self->id << "] finish coroutine" << std::endl;
  }

  pump::pump()
  : collateral_block_lambda([this]{collateral_block();})
  , switch_context()
  , running(true)
  , task(std::async(std::launch::async, std::ref(*this)))
  {}
  pump::~pump()
  {
    stop();
  }
  void pump::stop()
  {
    std::unique_lock<std::mutex> lock(mutex);
    if(running)
    {
      running = false;
      condition.notify_one();
      lock.unlock();
      task.wait();
    }
  }
  void pump::wait()
  {
    std::unique_lock<std::mutex> lock(mutex);
    idle.wait(lock, [this]{return queue.empty();});
  }
  void pump::operator()()
  {
    try
    {
      thread_id = std::this_thread::get_id();

      worker = [&] {
        std::unique_lock<std::mutex> lock(mutex);
        if(queue.empty())
        {
          idle.notify_one();
        }
        if(timers.empty())
        {
          condition.wait(lock, [this]{return queue.size() || !running;});
        }
        else
        {
          condition.wait_until(lock, timers.begin()->first.t, [this]{return queue.size() || !running;});
        }

        if(queue.size())
        {
          std::function<void()> f(std::move(queue.front()));
          queue.pop();
          lock.unlock();
          f();
          lock.lock();
        }

        while(timers_expired())
        {
          auto f(timers.begin()->second);
          timers.erase(timers.begin());
          lock.unlock();
          f();
          lock.lock();
        }
      };

      coroutine zero;
      create_context();

      exit = [&]{debug << "enter exit" << std::endl; zero.release();};

      std::unique_lock<std::mutex> lock(mutex);
      while(running || queue.size() || collateral_blocked.size())
      {
        lock.unlock();

        assert(coroutines.size());

        coroutines.back().call(zero);

        lock.lock();

        remove_finished_coroutines();
      }
      debug << "finish pump" << std::endl;
      assert(queue.empty());
    }
    catch(const std::exception& e)
    {
      debug << "oops: " << e.what() << std::endl;
      std::terminate();
    }
  }
  bool pump::timers_expired() const
  {
    return timers.size() && timers.begin()->first.expired();
  }
  void pump::create_context()
  {
    coroutines.emplace_back([&]{
        try
        {
          auto self = find_self(coroutines);
          debug << "[" << self->id << "] create context" << std::endl;
          while(!self->released && (running ||
                                    queue.size() ||
                                    timers_expired()))
          {
            worker();
            if(!self->released) collateral_release(self);
          }
          if(self->released) finish(coroutines);

          if(switch_context) decltype(switch_context)(std::move(switch_context))();

          if(!self->released) collateral_release(self);

          exit();
        }
        catch(const forced_unwind&) { debug << "ignoring forced_unwind" << std::endl; }
        catch(const std::exception& e)
        {
          debug << "oops: " << e.what() << std::endl;
          std::terminate();
        }
      });
  }
  void pump::collateral_block()
  {
    auto self = find_self(coroutines);
    debug << "[" << self->id << "] collateral_block" << std::endl;

    collateral_blocked.splice(collateral_blocked.end(), coroutines, self);
    create_context();
    self->yield_to(coroutines.back());

    debug << "[" << self->id << "] collateral_unblock" << std::endl;
  }
  void pump::collateral_release(std::list<coroutine>::iterator self)
  {
    if(collateral_blocked.size()) finish(coroutines);
    while(collateral_blocked.size())
    {
      coroutines.splice(coroutines.end(), collateral_blocked, collateral_blocked.begin());
      self->yield_to(coroutines.back());
    }
  }
  void pump::block(void* p)
  {
    auto it = skip_block.find(p);
    if(it != skip_block.end())
    {
      skip_block.erase(it);
      return;
    }

    auto self = find_self(coroutines);

    self->port = p;

    debug << "[" << self->id << "] block" << std::endl;

    create_context();

    self->yield_to(coroutines.back());
    debug << "[" << self->id << "] entered context" << std::endl;
    if (debug.rdbuf())
    {
      debug << "routines: ";
      for (auto& c: coroutines) {
        debug << c.id << " ";
      }
      debug << std::endl;
    }
    remove_finished_coroutines();
  }
  void pump::release(void* p)
  {
    auto self = find_self(coroutines);

    auto blocked = find_blocked(coroutines, p);
    if(blocked == coroutines.end())
    {
    debug << "[" << self->id << "] skip block" << std::endl;
      skip_block.insert(p);
      return;
    }

    debug << "[" << blocked->id << "] unblock" << std::endl;
    debug << "[" << self->id << "] released" << std::endl;
    self->released = true;

    switch_context = [blocked,self] {
      blocked->port = nullptr;

      debug << "[" << self->id << "] switch from" << std::endl;
      debug << "[" << blocked->id << "] to" << std::endl;

      self->yield_to(*blocked);
    };
  }
  void pump::operator()(const std::function<void()>& e)
  {
    assert(e);
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(e);
    condition.notify_one();
  }
  void pump::operator()(std::function<void()>&& e)
  {
    assert(e);
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(std::move(e));
    condition.notify_one();
  }
  void pump::handle(size_t id, size_t ms, const std::function<void()>& e, size_t rank)
  {
    assert(e);
    assert(std::find_if(timers.begin(), timers.end(), [id](const std::pair<deadline, std::function<void()>>& p){ return p.first.id == id; }) == timers.end());
    timers.emplace(deadline(id, ms, rank), e);
  }
  void pump::remove(size_t id)
  {
    auto it = std::find_if(timers.begin(), timers.end(), [id](const std::pair<deadline, std::function<void()>>& p){ return p.first.id == id; });
    if(it != timers.end()) timers.erase(it);
  }
  void pump::remove_finished_coroutines()
  {
    coroutines.remove_if([](dzn::coroutine& c){
        if(c.finished) debug << "[" << c.id << "] removing" << std::endl;
        return c.finished;
      });
  }
}
//version: 2.8.2

