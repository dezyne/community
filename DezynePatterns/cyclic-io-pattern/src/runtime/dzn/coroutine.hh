#ifndef DZN_COROUTINE_HH
#define DZN_COROUTINE_HH

#if HAVE_BOOST_COROUTINE
#include <boost/coroutine/all.hpp>
#else
#include <dzn/context.hh>
#endif

namespace dzn
{
#if HAVE_BOOST_COROUTINE
  typedef boost::coroutines::symmetric_coroutine<void>::call_type context;
  typedef boost::coroutines::symmetric_coroutine<void>::yield_type yield;
  typedef boost::coroutines::detail::forced_unwind forced_unwind;
#else
  typedef context::forced_unwind forced_unwind;
  typedef std::function<void(dzn::context&)> yield;
#endif

  struct coroutine
  {
    int id;
    dzn::context context;
    dzn::yield yield;
    void* port;
    bool finished;
    bool released;
    bool skip_block;
    template <typename Worker>
    coroutine(Worker&& worker)
    : id(-2)
    , context([this, worker](dzn::yield& yield){
#if !HAVE_BOOST_COROUTINE
        this->id = context::get_id();
#endif
        this->yield = std::move(yield);
        worker();
      })
    , port(nullptr)
    , finished(false)
    , released(false)
    , skip_block(false)
    {}
    void yield_to(dzn::coroutine& c)
    {
      this->yield(c.context);
    }
#if HAVE_BOOST_COROUTINE
    coroutine() : id(-1), context(), port(), finished(), released(), skip_block() {}
    void call(dzn::coroutine&)
    {
      this->context();
    }
    void release(){}
#else // !HAVE_BOOST_COROUTINE
    coroutine() : id(-1), context(false), port(), finished(), released(), skip_block() {}
    void call(dzn::coroutine& c)
    {
      this->context.call(c.context);
    }
    void release()
    {
      this->context.release();
    }
#endif // HAVE_BOOST_COROUTINE
  };
}
#endif //DZN_COROUTINE_HH
//version: 2.8.2

