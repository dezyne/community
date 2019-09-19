#include "ledcontrol.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



ledsControl::ledsControl(const dzn::locator& dzn_locator)
: dzn_meta{"","ledsControl",0,0,{& btn1.meta,& btn2.meta,& btn3.meta,& btn4.meta,& device.meta},{},{[this]{api.check_bindings();},[this]{btn1.check_bindings();},[this]{btn2.check_bindings();},[this]{btn3.check_bindings();},[this]{btn4.check_bindings();},[this]{device.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::ledsControl::State::Idle)

, api({{"api",this,&dzn_meta},{"",0,0}})

, btn1({{"",0,0},{"btn1",this,&dzn_meta}})
, btn2({{"",0,0},{"btn2",this,&dzn_meta}})
, btn3({{"",0,0},{"btn3",this,&dzn_meta}})
, btn4({{"",0,0},{"btn4",this,&dzn_meta}})
, device({{"",0,0},{"device",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  api.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_start();}, this->api.meta, "start");};
  api.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_stop();}, this->api.meta, "stop");};
  btn1.out.turnedOn = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->btn1) = false; return btn1_turnedOn();}, this->btn1.meta, "turnedOn");};
  btn1.out.turnedOff = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->btn1) = false; return btn1_turnedOff();}, this->btn1.meta, "turnedOff");};
  btn2.out.turnedOn = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->btn2) = false; return btn2_turnedOn();}, this->btn2.meta, "turnedOn");};
  btn2.out.turnedOff = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->btn2) = false; return btn2_turnedOff();}, this->btn2.meta, "turnedOff");};
  btn3.out.turnedOn = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->btn3) = false; return btn3_turnedOn();}, this->btn3.meta, "turnedOn");};
  btn3.out.turnedOff = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->btn3) = false; return btn3_turnedOff();}, this->btn3.meta, "turnedOff");};
  btn4.out.turnedOn = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->btn4) = false; return btn4_turnedOn();}, this->btn4.meta, "turnedOn");};
  btn4.out.turnedOff = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->btn4) = false; return btn4_turnedOff();}, this->btn4.meta, "turnedOff");};





}

void ledsControl::api_start()
{
  if (state == ::ledsControl::State::Idle) 
  {
    this->btn1.in.start();
    this->btn2.in.start();
    this->btn3.in.start();
    this->btn4.in.start();
    this->device.in.led1Off();
    this->device.in.led2Off();
    this->device.in.led3Off();
    this->device.in.led4Off();
    state = ::ledsControl::State::Starting;
  }
  else if (!(state == ::ledsControl::State::Idle)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ledsControl::api_stop()
{
  if (state == ::ledsControl::State::Idle) ;
  else if (state == ::ledsControl::State::Starting) 
  {
    this->btn1.in.stop();
    this->btn2.in.stop();
    this->btn3.in.stop();
    this->btn4.in.stop();
    state = ::ledsControl::State::Idle;
  }
  else if (state == ::ledsControl::State::Active) 
  {
    this->btn1.in.stop();
    this->btn2.in.stop();
    this->btn3.in.stop();
    this->btn4.in.stop();
    this->device.in.led1Off();
    this->device.in.led2Off();
    this->device.in.led3Off();
    this->device.in.led4Off();
    state = ::ledsControl::State::Idle;
  }
  else if ((!(state == ::ledsControl::State::Active) && (!(state == ::ledsControl::State::Starting) && !(state == ::ledsControl::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ledsControl::btn1_turnedOn()
{
  if (state == ::ledsControl::State::Starting) 
  {
    this->device.in.led1On();
    state = ::ledsControl::State::Active;
  }
  else if (state == ::ledsControl::State::Active) 
  {
    this->device.in.led1On();
  }
  else if ((!(state == ::ledsControl::State::Active) && !(state == ::ledsControl::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ledsControl::btn1_turnedOff()
{
  if (state == ::ledsControl::State::Starting) ;
  else if (state == ::ledsControl::State::Active) 
  {
    this->device.in.led1Off();
  }
  else if ((!(state == ::ledsControl::State::Active) && !(state == ::ledsControl::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ledsControl::btn2_turnedOn()
{
  if (state == ::ledsControl::State::Starting) 
  {
    this->device.in.led2On();
    state = ::ledsControl::State::Active;
  }
  else if (state == ::ledsControl::State::Active) 
  {
    this->device.in.led2On();
  }
  else if ((!(state == ::ledsControl::State::Active) && !(state == ::ledsControl::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ledsControl::btn2_turnedOff()
{
  if (state == ::ledsControl::State::Starting) ;
  else if (state == ::ledsControl::State::Active) 
  {
    this->device.in.led2Off();
  }
  else if ((!(state == ::ledsControl::State::Active) && !(state == ::ledsControl::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ledsControl::btn3_turnedOn()
{
  if (state == ::ledsControl::State::Starting) 
  {
    this->device.in.led3On();
    state = ::ledsControl::State::Active;
  }
  else if (state == ::ledsControl::State::Active) 
  {
    this->device.in.led3On();
  }
  else if ((!(state == ::ledsControl::State::Active) && !(state == ::ledsControl::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ledsControl::btn3_turnedOff()
{
  if (state == ::ledsControl::State::Starting) ;
  else if (state == ::ledsControl::State::Active) 
  {
    this->device.in.led3Off();
  }
  else if ((!(state == ::ledsControl::State::Active) && !(state == ::ledsControl::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ledsControl::btn4_turnedOn()
{
  if (state == ::ledsControl::State::Starting) 
  {
    this->device.in.led4On();
    state = ::ledsControl::State::Active;
  }
  else if (state == ::ledsControl::State::Active) 
  {
    this->device.in.led4On();
  }
  else if ((!(state == ::ledsControl::State::Active) && !(state == ::ledsControl::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void ledsControl::btn4_turnedOff()
{
  if (state == ::ledsControl::State::Starting) ;
  else if (state == ::ledsControl::State::Active) 
  {
    this->device.in.led4Off();
  }
  else if ((!(state == ::ledsControl::State::Active) && !(state == ::ledsControl::State::Starting))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void ledsControl::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void ledsControl::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

ledControlSystem::ledControlSystem(const dzn::locator& dzn_locator)
: dzn_meta{"","ledControlSystem",0,0,{& button1.meta,& button2.meta,& button3.meta,& button4.meta,& leds.meta},{& lc.dzn_meta,& btn1.dzn_meta,& btn2.dzn_meta,& btn3.dzn_meta,& btn4.dzn_meta},{[this]{api.check_bindings();},[this]{button1.check_bindings();},[this]{button2.check_bindings();},[this]{button3.check_bindings();},[this]{button4.check_bindings();},[this]{leds.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, lc(dzn_locator)
, btn1(dzn_locator)
, btn2(dzn_locator)
, btn3(dzn_locator)
, btn4(dzn_locator)

, api(lc.api)
, button1(btn1.glue), button2(btn2.glue), button3(btn3.glue), button4(btn4.glue), leds(lc.device)
{


  lc.dzn_meta.parent = &dzn_meta;
  lc.dzn_meta.name = "lc";
  btn1.dzn_meta.parent = &dzn_meta;
  btn1.dzn_meta.name = "btn1";
  btn2.dzn_meta.parent = &dzn_meta;
  btn2.dzn_meta.name = "btn2";
  btn3.dzn_meta.parent = &dzn_meta;
  btn3.dzn_meta.name = "btn3";
  btn4.dzn_meta.parent = &dzn_meta;
  btn4.dzn_meta.name = "btn4";


  connect(btn1.api, lc.btn1);
  connect(btn2.api, lc.btn2);
  connect(btn3.api, lc.btn3);
  connect(btn4.api, lc.btn4);

  dzn::rank(api.meta.provides.meta, 0);

}

void ledControlSystem::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void ledControlSystem::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1