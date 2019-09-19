#include "motorled.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



motorLed::motorLed(const dzn::locator& dzn_locator)
: dzn_meta{"","motorLed",0,0,{& imotor.meta,& ileds.meta},{},{[this]{iml.check_bindings();},[this]{imotor.check_bindings();},[this]{ileds.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::motorLed::State::Idle), lc(0)

, iml({{"iml",this,&dzn_meta},{"",0,0}})

, imotor({{"",0,0},{"imotor",this,&dzn_meta}})
, ileds({{"",0,0},{"ileds",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  iml.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->iml) = false; return iml_start();}, this->iml.meta, "start");};
  iml.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->iml) = false; return iml_stop();}, this->iml.meta, "stop");};
  imotor.out.speedUp = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->imotor) = false; return imotor_speedUp();}, this->imotor.meta, "speedUp");};
  imotor.out.speedDown = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->imotor) = false; return imotor_speedDown();}, this->imotor.meta, "speedDown");};





}

void motorLed::iml_start()
{
  if (state == ::motorLed::State::Idle) 
  {
    this->imotor.in.start();
    state = ::motorLed::State::Running;
  }
  else if (!(state == ::motorLed::State::Idle)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void motorLed::iml_stop()
{
  if (state == ::motorLed::State::Idle) ;
  else if (state == ::motorLed::State::Running) 
  {
    this->imotor.in.stop();
    state = ::motorLed::State::Idle;
  }
  else if ((!(state == ::motorLed::State::Running) && !(state == ::motorLed::State::Idle))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void motorLed::imotor_speedUp()
{
  if ((state == ::motorLed::State::Running && lc < 5)) 
  {
    lc = lc + 1;
    {
      if (lc == 4) this->ileds.in.led4On();
    }
    {
      if (lc == 3) this->ileds.in.led3On();
    }
    {
      if (lc == 2) this->ileds.in.led2On();
    }
    {
      if (lc == 1) this->ileds.in.led1On();
    }
  }
  else if (!((state == ::motorLed::State::Running && lc < 5))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void motorLed::imotor_speedDown()
{
  if ((state == ::motorLed::State::Running && lc > 0)) 
  {
    {
      if (lc == 4) this->ileds.in.led4Off();
    }
    {
      if (lc == 3) this->ileds.in.led3Off();
    }
    {
      if (lc == 2) this->ileds.in.led2Off();
    }
    {
      if (lc == 1) this->ileds.in.led1Off();
    }
    lc = lc - 1;
  }
  else if (!((state == ::motorLed::State::Running && lc > 0))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void motorLed::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void motorLed::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

motorLedSystem::motorLedSystem(const dzn::locator& dzn_locator)
: dzn_meta{"","motorLedSystem",0,0,{& itoggle.meta,& ispeedup.meta,& ispeeddown.meta,& imotor.meta,& ileds.meta},{& mts.dzn_meta,& ml.dzn_meta},{[this]{iml.check_bindings();},[this]{itoggle.check_bindings();},[this]{ispeedup.check_bindings();},[this]{ispeeddown.check_bindings();},[this]{imotor.check_bindings();},[this]{ileds.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, mts(dzn_locator)
, ml(dzn_locator)

, iml(ml.iml)
, itoggle(mts.itoggle), ispeedup(mts.ispeedup), ispeeddown(mts.ispeeddown), imotor(mts.imotor), ileds(ml.ileds)
{


  mts.dzn_meta.parent = &dzn_meta;
  mts.dzn_meta.name = "mts";
  ml.dzn_meta.parent = &dzn_meta;
  ml.dzn_meta.name = "ml";


  connect(mts.api, ml.imotor);

  dzn::rank(iml.meta.provides.meta, 0);

}

void motorLedSystem::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void motorLedSystem::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1