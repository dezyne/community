#include "motor.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



MotorControl::MotorControl(const dzn::locator& dzn_locator)
: dzn_meta{"","MotorControl",0,0,{& toggle.meta,& speedup.meta,& speeddown.meta,& timer.meta,& device.meta},{},{[this]{api.check_bindings();},[this]{toggle.check_bindings();},[this]{speedup.check_bindings();},[this]{speeddown.check_bindings();},[this]{timer.check_bindings();},[this]{device.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, targetSpeed(0), actualSpeed(0), state(::MotorControl::State::Idle), targetForward(true), actualForward(true), timertick(2000)

, api({{"api",this,&dzn_meta},{"",0,0}})

, toggle({{"",0,0},{"toggle",this,&dzn_meta}})
, speedup({{"",0,0},{"speedup",this,&dzn_meta}})
, speeddown({{"",0,0},{"speeddown",this,&dzn_meta}})
, timer({{"",0,0},{"timer",this,&dzn_meta}})
, device({{"",0,0},{"device",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  api.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_start();}, this->api.meta, "start");};
  api.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_stop();}, this->api.meta, "stop");};
  toggle.out.turnedOn = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->toggle) = false; return toggle_turnedOn();}, this->toggle.meta, "turnedOn");};
  toggle.out.turnedOff = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->toggle) = false; return toggle_turnedOff();}, this->toggle.meta, "turnedOff");};
  speedup.out.turnedOn = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->speedup) = false; return speedup_turnedOn();}, this->speedup.meta, "turnedOn");};
  speedup.out.turnedOff = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->speedup) = false; return speedup_turnedOff();}, this->speedup.meta, "turnedOff");};
  speeddown.out.turnedOn = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->speeddown) = false; return speeddown_turnedOn();}, this->speeddown.meta, "turnedOn");};
  speeddown.out.turnedOff = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->speeddown) = false; return speeddown_turnedOff();}, this->speeddown.meta, "turnedOff");};
  timer.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->timer) = false; return timer_timeout();}, this->timer.meta, "timeout");};





}

void MotorControl::api_start()
{
  if (state == ::MotorControl::State::Idle) 
  {
    this->timer.in.create(timertick);
    state = ::MotorControl::State::Running;
    this->toggle.in.start();
    this->speedup.in.start();
    this->speeddown.in.start();
  }
  else if (!(state == ::MotorControl::State::Idle)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void MotorControl::api_stop()
{
  if (state == ::MotorControl::State::Idle) ;
  else if (state == ::MotorControl::State::Running) 
  {
    this->timer.in.cancel();
    state = ::MotorControl::State::Idle;
    this->toggle.in.stop();
    this->speedup.in.stop();
    this->speeddown.in.stop();
  }
  else if ((!(state == ::MotorControl::State::Running) && !(state == ::MotorControl::State::Idle))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void MotorControl::toggle_turnedOn()
{
  if (state == ::MotorControl::State::Idle) ;
  else if (state == ::MotorControl::State::Running) 
  {
    {
      if (targetForward == actualForward) targetForward = !(targetForward);
    }
  }
  else if ((!(state == ::MotorControl::State::Running) && !(state == ::MotorControl::State::Idle))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void MotorControl::toggle_turnedOff()
{

  {
    ;
  }

  return;

}
void MotorControl::speedup_turnedOn()
{
  if (state == ::MotorControl::State::Idle) ;
  else if (state == ::MotorControl::State::Running) 
  {
    {
      if (targetSpeed < 5) targetSpeed = targetSpeed + 1;
    }
  }
  else if ((!(state == ::MotorControl::State::Running) && !(state == ::MotorControl::State::Idle))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void MotorControl::speedup_turnedOff()
{

  {
    ;
  }

  return;

}
void MotorControl::speeddown_turnedOn()
{
  if (state == ::MotorControl::State::Idle) ;
  else if (state == ::MotorControl::State::Running) 
  {
    {
      if (targetSpeed > 0) targetSpeed = targetSpeed - 1;
    }
  }
  else if ((!(state == ::MotorControl::State::Running) && !(state == ::MotorControl::State::Idle))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void MotorControl::speeddown_turnedOff()
{

  {
    ;
  }

  return;

}
void MotorControl::timer_timeout()
{
  if (state == ::MotorControl::State::Running) 
  {
    this->timer.in.create(timertick);
    {
      if (targetForward == actualForward) 
      {
        {
          if (actualSpeed < targetSpeed) 
          {
            actualSpeed = actualSpeed + 1;
            this->device.in.speedUp();
            this->api.out.speedUp();
          }
          else {
            if (actualSpeed > targetSpeed) 
            {
              actualSpeed = actualSpeed - 1;
              this->device.in.speedDown();
              this->api.out.speedDown();
            }
          }
        }
      }
      else 
      {
        {
          if (actualSpeed > 0) 
          {
            actualSpeed = actualSpeed - 1;
            this->device.in.speedDown();
            this->api.out.speedDown();
          }
          else {
            if (actualSpeed == 0) 
            {
              actualForward = !(actualForward);
              {
                if (actualForward) this->device.in.forward();
                else this->device.in.backward();
              }
            }
          }
        }
      }
    }
  }
  else if (!(state == ::MotorControl::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void MotorControl::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void MotorControl::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

motorTestSystem::motorTestSystem(const dzn::locator& dzn_locator)
: dzn_meta{"","motorTestSystem",0,0,{& itoggle.meta,& ispeedup.meta,& ispeeddown.meta,& imotor.meta},{& mc.dzn_meta,& toggle.dzn_meta,& speedup.dzn_meta,& timer.dzn_meta,& speeddown.dzn_meta},{[this]{api.check_bindings();},[this]{itoggle.check_bindings();},[this]{ispeedup.check_bindings();},[this]{ispeeddown.check_bindings();},[this]{imotor.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, mc(dzn_locator)
, toggle(dzn_locator)
, speedup(dzn_locator)
, timer(dzn_locator)
, speeddown(dzn_locator)

, api(mc.api)
, itoggle(toggle.glue), ispeedup(speedup.glue), ispeeddown(speeddown.glue), imotor(mc.device)
{


  mc.dzn_meta.parent = &dzn_meta;
  mc.dzn_meta.name = "mc";
  toggle.dzn_meta.parent = &dzn_meta;
  toggle.dzn_meta.name = "toggle";
  speedup.dzn_meta.parent = &dzn_meta;
  speedup.dzn_meta.name = "speedup";
  timer.dzn_meta.parent = &dzn_meta;
  timer.dzn_meta.name = "timer";
  speeddown.dzn_meta.parent = &dzn_meta;
  speeddown.dzn_meta.name = "speeddown";


  connect(toggle.api, mc.toggle);
  connect(speedup.api, mc.speedup);
  connect(speeddown.api, mc.speeddown);
  connect(timer.api, mc.timer);

  dzn::rank(api.meta.provides.meta, 0);

}

void motorTestSystem::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void motorTestSystem::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1