#include "InputDezyne.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



InputController::InputController(const dzn::locator& dzn_locator)
: dzn_meta{"","InputController",0,0,{& trigger.meta,& io.meta},{},{[this]{api.check_bindings();},[this]{trigger.check_bindings();},[this]{io.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::IInput::eState::idle), value(false)

, api({{"api",this,&dzn_meta},{"",0,0}})

, trigger({{"",0,0},{"trigger",this,&dzn_meta}})
, io({{"",0,0},{"io",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  api.in.activate = [&](){return dzn::call_in(this,[=]{ return api_activate();}, this->api.meta, "activate");};
  api.in.deactivate = [&](){return dzn::call_in(this,[=]{ return api_deactivate();}, this->api.meta, "deactivate");};
  trigger.out.trigger = [&](){return dzn::call_out(this,[=]{ return trigger_trigger();}, this->trigger.meta, "trigger");};


  api.in.getValue = [&](){return dzn::call_in(this,[=]{ return api_getValue();}, this->api.meta, "getValue");};



}

void InputController::api_activate()
{
  if ((state == ::IInput::eState::idle && true)) state = ::IInput::eState::active;
  else if ((!((state == ::IInput::eState::idle && true)) && true)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void InputController::api_deactivate()
{
  if ((state == ::IInput::eState::idle && true)) ;
  else if ((state == ::IInput::eState::active && true)) state = ::IInput::eState::idle;
  else if ((!((state == ::IInput::eState::active && true)) && (!((state == ::IInput::eState::idle && true)) && true))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
bool InputController::api_getValue()
{
  if (true) { this->reply_bool = value; }
  else if ((!(true) && true)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return this->reply_bool;
}
void InputController::trigger_trigger()
{
  if ((state == ::IInput::eState::idle && true)) value = this->io.in.getValue();
  else if ((state == ::IInput::eState::active && true)) 
  {
    bool newVal = this->io.in.getValue();
    {
      if (newVal != value
      ) 
      {
        value = newVal;
        this->api.out.changed();
      }
    }
  }
  else if ((!((state == ::IInput::eState::active && true)) && (!((state == ::IInput::eState::idle && true)) && true))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void InputController::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void InputController::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

InputDezyne::InputDezyne(const dzn::locator& dzn_locator)
: dzn_meta{"","InputDezyne",0,0,{& input.meta},{& trigger.dzn_meta,& control.dzn_meta},{[this]{api.check_bindings();},[this]{input.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, trigger(dzn_locator)
, control(dzn_locator)

, api(control.api)
, input(control.io)
{


  trigger.dzn_meta.parent = &dzn_meta;
  trigger.dzn_meta.name = "trigger";
  control.dzn_meta.parent = &dzn_meta;
  control.dzn_meta.name = "control";


  connect(trigger.api, control.trigger);

  dzn::rank(api.meta.provides.meta, 0);

}

void InputDezyne::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void InputDezyne::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.8.2