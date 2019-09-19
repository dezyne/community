#include "buttonled.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



ButtonLed::ButtonLed(const dzn::locator& dzn_locator)
: dzn_meta{"","ButtonLed",0,0,{& button.meta,& led.meta},{},{[this]{api.check_bindings();},[this]{button.check_bindings();},[this]{led.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::ButtonLed::State::Idle)

, api({{"api",this,&dzn_meta},{"",0,0}})

, button({{"",0,0},{"button",this,&dzn_meta}})
, led({{"",0,0},{"led",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  api.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_start();}, this->api.meta, "start");};
  api.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_stop();}, this->api.meta, "stop");};
  button.out.turnedOn = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->button) = false; return button_turnedOn();}, this->button.meta, "turnedOn");};
  button.out.turnedOff = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->button) = false; return button_turnedOff();}, this->button.meta, "turnedOff");};





}

void ButtonLed::api_start()
{
  if (state == ::ButtonLed::State::Idle) 
  {
    this->button.in.start();
    this->led.in.turnOff();
    state = ::ButtonLed::State::Starting;
  }
  else if (!(state == ::ButtonLed::State::Idle)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ButtonLed::api_stop()
{
  if (state == ::ButtonLed::State::Idle) ;
  else if (state == ::ButtonLed::State::Starting) 
  {
    this->button.in.stop();
    state = ::ButtonLed::State::Idle;
  }
  else if (state == ::ButtonLed::State::Pressed) 
  {
    this->button.in.stop();
    this->led.in.turnOff();
    state = ::ButtonLed::State::Idle;
  }
  else if (state == ::ButtonLed::State::Released) 
  {
    this->button.in.stop();
    state = ::ButtonLed::State::Idle;
  }
  else if ((!(state == ::ButtonLed::State::Released) && (!(state == ::ButtonLed::State::Pressed) && (!(state == ::ButtonLed::State::Starting) && !(state == ::ButtonLed::State::Idle))))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ButtonLed::button_turnedOn()
{
  if (state == ::ButtonLed::State::Starting) 
  {
    this->led.in.turnOn();
    state = ::ButtonLed::State::Pressed;
  }
  else if (state == ::ButtonLed::State::Released) 
  {
    this->led.in.turnOn();
    state = ::ButtonLed::State::Pressed;
  }
  else if ((!(state == ::ButtonLed::State::Released) && !(state == ::ButtonLed::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ButtonLed::button_turnedOff()
{
  if (state == ::ButtonLed::State::Starting) 
  {
    state = ::ButtonLed::State::Released;
  }
  else if (state == ::ButtonLed::State::Pressed) 
  {
    this->led.in.turnOff();
    state = ::ButtonLed::State::Released;
  }
  else if ((!(state == ::ButtonLed::State::Pressed) && !(state == ::ButtonLed::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void ButtonLed::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void ButtonLed::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

LedTestSystem::LedTestSystem(const dzn::locator& dzn_locator)
: dzn_meta{"","LedTestSystem",0,0,{& buttonGlue.meta,& ledGlue.meta},{& buttonLed.dzn_meta,& button.dzn_meta},{[this]{api.check_bindings();},[this]{buttonGlue.check_bindings();},[this]{ledGlue.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, buttonLed(dzn_locator)
, button(dzn_locator)

, api(buttonLed.api)
, buttonGlue(button.glue), ledGlue(buttonLed.led)
{


  buttonLed.dzn_meta.parent = &dzn_meta;
  buttonLed.dzn_meta.name = "buttonLed";
  button.dzn_meta.parent = &dzn_meta;
  button.dzn_meta.name = "button";


  connect(button.api, buttonLed.button);

  dzn::rank(api.meta.provides.meta, 0);

}

void LedTestSystem::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void LedTestSystem::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////
//SYSTEM

LedTestSystemInt::LedTestSystemInt(const dzn::locator& dzn_locator)
: dzn_meta{"","LedTestSystemInt",0,0,{& button.meta,& ledGlue.meta},{& buttonLed.dzn_meta},{[this]{api.check_bindings();},[this]{button.check_bindings();},[this]{ledGlue.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, buttonLed(dzn_locator)

, api(buttonLed.api)
, button(buttonLed.button), ledGlue(buttonLed.led)
{


  buttonLed.dzn_meta.parent = &dzn_meta;
  buttonLed.dzn_meta.name = "buttonLed";



  dzn::rank(api.meta.provides.meta, 0);

}

void LedTestSystemInt::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void LedTestSystemInt::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1