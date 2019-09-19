#include "button.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



ButtonImpl::ButtonImpl(const dzn::locator& dzn_locator)
: dzn_meta{"","ButtonImpl",0,0,{& glue.meta,& timer.meta},{},{[this]{api.check_bindings();},[this]{glue.check_bindings();},[this]{timer.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::ButtonImpl::State::Idle), pollDuration(100)

, api({{"api",this,&dzn_meta},{"",0,0}})

, glue({{"",0,0},{"glue",this,&dzn_meta}})
, timer({{"",0,0},{"timer",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  api.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_start();}, this->api.meta, "start");};
  api.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_stop();}, this->api.meta, "stop");};
  timer.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->timer) = false; return timer_timeout();}, this->timer.meta, "timeout");};





}

void ButtonImpl::api_start()
{
  if (state == ::ButtonImpl::State::Idle) 
  {
    this->timer.in.create(pollDuration);
    state = ::ButtonImpl::State::Starting;
  }
  else if (!(state == ::ButtonImpl::State::Idle)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ButtonImpl::api_stop()
{

  {
    this->timer.in.cancel();
    state = ::ButtonImpl::State::Idle;
  }

  return;

}
void ButtonImpl::timer_timeout()
{
  if (state == ::ButtonImpl::State::Starting) 
  {
    this->timer.in.create(pollDuration);
    ::IButtonGlue::State::type button = this->glue.in.get();
    {
      if (button == ::IButtonGlue::State::On) 
      {
        this->api.out.turnedOn();
        state = ::ButtonImpl::State::On;
      }
    }
    {
      if (button == ::IButtonGlue::State::Off) 
      {
        this->api.out.turnedOff();
        state = ::ButtonImpl::State::Off;
      }
    }
  }
  else if (state == ::ButtonImpl::State::On) 
  {
    this->timer.in.create(pollDuration);
    ::IButtonGlue::State::type button = this->glue.in.get();
    {
      if (button == ::IButtonGlue::State::Off) 
      {
        this->api.out.turnedOff();
        state = ::ButtonImpl::State::Off;
      }
    }
  }
  else if (state == ::ButtonImpl::State::Off) 
  {
    this->timer.in.create(pollDuration);
    ::IButtonGlue::State::type button = this->glue.in.get();
    {
      if (button == ::IButtonGlue::State::On) 
      {
        this->api.out.turnedOn();
        state = ::ButtonImpl::State::On;
      }
    }
  }
  else if ((!(state == ::ButtonImpl::State::Off) && (!(state == ::ButtonImpl::State::On) && !(state == ::ButtonImpl::State::Starting)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void ButtonImpl::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void ButtonImpl::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

Button::Button(const dzn::locator& dzn_locator)
: dzn_meta{"","Button",0,0,{& glue.meta},{& timer.dzn_meta,& impl.dzn_meta},{[this]{api.check_bindings();},[this]{glue.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, timer(dzn_locator)
, impl(dzn_locator)

, api(impl.api)
, glue(impl.glue)
{


  timer.dzn_meta.parent = &dzn_meta;
  timer.dzn_meta.name = "timer";
  impl.dzn_meta.parent = &dzn_meta;
  impl.dzn_meta.name = "impl";


  connect(timer.api, impl.timer);

  dzn::rank(api.meta.provides.meta, 0);

}

void Button::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Button::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1