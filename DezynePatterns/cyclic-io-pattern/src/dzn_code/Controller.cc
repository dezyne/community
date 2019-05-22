#include "Controller.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



Controller::Controller(const dzn::locator& dzn_locator)
: dzn_meta{"","Controller",0,0,{& cpp.meta,& io.meta,& slow.meta},{},{[this]{api.check_bindings();},[this]{cpp.check_bindings();},[this]{io.check_bindings();},[this]{slow.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::Controller::eState::idle)

, api({{"api",this,&dzn_meta},{"",0,0}})

, cpp({{"",0,0},{"cpp",this,&dzn_meta}})
, io({{"",0,0},{"io",this,&dzn_meta}})
, slow({{"",0,0},{"slow",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  api.in.start = [&](){return dzn::call_in(this,[=]{ return api_start();}, this->api.meta, "start");};
  cpp.out.changed = [&](){return dzn::call_out(this,[=]{ return cpp_changed();}, this->cpp.meta, "changed");};
  slow.out.changed = [&](){return dzn::call_out(this,[=]{ return slow_changed();}, this->slow.meta, "changed");};





}

void Controller::api_start()
{
  if ((state == ::Controller::eState::idle && true)) 
  {
    state = ::Controller::eState::active;
    this->slow.in.activate();
    this->cpp.in.activate();
  }
  else if ((!(state == ::Controller::eState::idle) && true)) ;
  else if ((!((!(state == ::Controller::eState::idle) && true)) && (!((state == ::Controller::eState::idle && true)) && true))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Controller::cpp_changed()
{
  if ((state == ::Controller::eState::active && true)) eval();
  else if ((!((state == ::Controller::eState::active && true)) && true)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Controller::slow_changed()
{
  if ((state == ::Controller::eState::active && true)) eval();
  else if ((!((state == ::Controller::eState::active && true)) && true)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}

void Controller::eval () 
{
  bool cppVal = this->cpp.in.getValue();
  bool slowVal = this->slow.in.getValue();
  {
    if ((cppVal && slowVal)) 
    {
      this->io.in.high();
    }
    else 
    {
      this->io.in.low();
    }
  }
  return ;
}

void Controller::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Controller::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

ControlSystem::ControlSystem(const dzn::locator& dzn_locator)
: dzn_meta{"","ControlSystem",0,0,{& io.meta,& input.meta},{& control.dzn_meta,& dezyne.dzn_meta,& cpp.dzn_meta},{[this]{api.check_bindings();},[this]{io.check_bindings();},[this]{input.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, control(dzn_locator)
, dezyne(dzn_locator)
, cpp(dzn_locator)

, api(control.api)
, io(control.io), input(dezyne.input)
{


  control.dzn_meta.parent = &dzn_meta;
  control.dzn_meta.name = "control";
  dezyne.dzn_meta.parent = &dzn_meta;
  dezyne.dzn_meta.name = "dezyne";
  cpp.dzn_meta.parent = &dzn_meta;
  cpp.dzn_meta.name = "cpp";


  connect(cpp.api, control.cpp);
  connect(dezyne.api, control.slow);

  dzn::rank(api.meta.provides.meta, 0);

}

void ControlSystem::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void ControlSystem::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.8.2