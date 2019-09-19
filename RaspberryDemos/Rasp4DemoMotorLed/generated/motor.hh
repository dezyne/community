#include <dzn/meta.hh>

namespace dzn {
  struct locator;
  struct runtime;
}



#include <iostream>
#include <map>


/********************************** INTERFACE *********************************/
#ifndef IMOTORCONTROL_HH
#define IMOTORCONTROL_HH



struct IMotorControl
{
#ifndef ENUM_IMotorControl_State
#define ENUM_IMotorControl_State 1


  struct State
  {
    enum type
    {
      Idle,Running
    };
  };


#endif // ENUM_IMotorControl_State

  struct
  {
    std::function< void()> start;
    std::function< void()> stop;
  } in;

  struct
  {
    std::function< void()> speedUp;
    std::function< void()> speedDown;
  } out;

  dzn::port::meta meta;
  inline IMotorControl(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");
    if (! in.stop) throw dzn::binding_error(meta, "in.stop");

    if (! out.speedUp) throw dzn::binding_error(meta, "out.speedUp");
    if (! out.speedDown) throw dzn::binding_error(meta, "out.speedDown");

  }
};

inline void connect (IMotorControl& provided, IMotorControl& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IMotorControl_State
#define ENUM_TO_STRING_IMotorControl_State 1
inline std::string to_string(::IMotorControl::State::type v)
{
  switch(v)
  {
    case ::IMotorControl::State::Idle: return "State_Idle";
    case ::IMotorControl::State::Running: return "State_Running";

  }
  return "";
}
#endif // ENUM_TO_STRING_IMotorControl_State

#ifndef STRING_TO_ENUM_IMotorControl_State
#define STRING_TO_ENUM_IMotorControl_State 1
inline ::IMotorControl::State::type to_IMotorControl_State(std::string s)
{
  static std::map<std::string, ::IMotorControl::State::type> m = {
    {"State_Idle", ::IMotorControl::State::Idle},
    {"State_Running", ::IMotorControl::State::Running},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IMotorControl_State


#endif // IMOTORCONTROL_HH

/********************************** INTERFACE *********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_BUTTONGLUE_HH
#define SKEL_BUTTONGLUE_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "hal.hh"



namespace skel {
  struct ButtonGlue
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::IButtonGlue api;


    ButtonGlue(const dzn::locator& dzn_locator)
    : dzn_meta{"","ButtonGlue",0,0,{},{},{[this]{api.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , api({{"api",this,&dzn_meta},{"",0,0}})


    {

      api.in.get = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_get();}, this->api.meta, "get");};

    }
    virtual ~ ButtonGlue() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const ButtonGlue& m)  {
      return m.stream_members(os);
    }
    private:
    virtual ::IButtonGlue::State::type api_get () = 0;

  };
}

#endif // BUTTONGLUE_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_LEDGLUE_HH
#define SKEL_LEDGLUE_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "hal.hh"



namespace skel {
  struct LedGlue
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::ILedGlue api;


    LedGlue(const dzn::locator& dzn_locator)
    : dzn_meta{"","LedGlue",0,0,{},{},{[this]{api.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , api({{"api",this,&dzn_meta},{"",0,0}})


    {
      api.in.turnOn = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_turnOn();}, this->api.meta, "turnOn");};
      api.in.turnOff = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_turnOff();}, this->api.meta, "turnOff");};


    }
    virtual ~ LedGlue() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const LedGlue& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void api_turnOn () = 0;
    virtual void api_turnOff () = 0;

  };
}

#endif // LEDGLUE_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_MOTORGLUE_HH
#define SKEL_MOTORGLUE_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "hal.hh"



namespace skel {
  struct MotorGlue
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::IMotorGlue api;


    MotorGlue(const dzn::locator& dzn_locator)
    : dzn_meta{"","MotorGlue",0,0,{},{},{[this]{api.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , api({{"api",this,&dzn_meta},{"",0,0}})


    {
      api.in.speedUp = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_speedUp();}, this->api.meta, "speedUp");};
      api.in.speedDown = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_speedDown();}, this->api.meta, "speedDown");};
      api.in.forward = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_forward();}, this->api.meta, "forward");};
      api.in.backward = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_backward();}, this->api.meta, "backward");};


    }
    virtual ~ MotorGlue() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const MotorGlue& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void api_speedUp () = 0;
    virtual void api_speedDown () = 0;
    virtual void api_forward () = 0;
    virtual void api_backward () = 0;

  };
}

#endif // MOTORGLUE_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_ENCODERGLUE_HH
#define SKEL_ENCODERGLUE_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "hal.hh"



namespace skel {
  struct EncoderGlue
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::IEncoderGlue api;


    EncoderGlue(const dzn::locator& dzn_locator)
    : dzn_meta{"","EncoderGlue",0,0,{},{},{[this]{api.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , api({{"api",this,&dzn_meta},{"",0,0}})


    {
      api.in.position = [&](int& position){return dzn::call_in(this,[=, & position]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_position(position);}, this->api.meta, "position");};

      api.in.direction = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_direction();}, this->api.meta, "direction");};

    }
    virtual ~ EncoderGlue() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const EncoderGlue& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void api_position (int& position) = 0;
    virtual ::IEncoderGlue::Dir::type api_direction () = 0;

  };
}

#endif // ENCODERGLUE_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_TIMER_HH
#define SKEL_TIMER_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "hal.hh"



namespace skel {
  struct Timer
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::ITimer api;


    Timer(const dzn::locator& dzn_locator)
    : dzn_meta{"","Timer",0,0,{},{},{[this]{api.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , api({{"api",this,&dzn_meta},{"",0,0}})


    {
      api.in.create = [&](int miliseconds){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_create(miliseconds);}, this->api.meta, "create");};
      api.in.cancel = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->api) = false; return api_cancel();}, this->api.meta, "cancel");};


    }
    virtual ~ Timer() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const Timer& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void api_create (int miliseconds) = 0;
    virtual void api_cancel () = 0;

  };
}

#endif // TIMER_HH

/***********************************  FOREIGN  **********************************/
/********************************** COMPONENT *********************************/
#ifndef MOTORCONTROL_HH
#define MOTORCONTROL_HH

#include "button.hh"
#include "button.hh"
#include "button.hh"
#include "hal.hh"
#include "hal.hh"



struct MotorControl
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;
#ifndef ENUM_MotorControl_State
#define ENUM_MotorControl_State 1


  struct State
  {
    enum type
    {
      Idle,Running
    };
  };


#endif // ENUM_MotorControl_State

  int targetSpeed;
  int actualSpeed;
  ::MotorControl::State::type state;
  bool targetForward;
  bool actualForward;
  int timertick;


  std::function<void ()> out_api;

  ::IMotorControl api;

  ::IButton toggle;
  ::IButton speedup;
  ::IButton speeddown;
  ::ITimer timer;
  ::IMotorGlue device;


  MotorControl(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const MotorControl& m)  {
    (void)m;
    return os << "[" << m.targetSpeed <<", " << m.actualSpeed <<", " << m.state <<", " << m.targetForward <<", " << m.actualForward <<", " << m.timertick <<"]" ;
  }
  private:
  void api_start();
  void api_stop();
  void toggle_turnedOn();
  void toggle_turnedOff();
  void speedup_turnedOn();
  void speedup_turnedOff();
  void speeddown_turnedOn();
  void speeddown_turnedOff();
  void timer_timeout();

};

#endif // MOTORCONTROL_HH

/********************************** COMPONENT *********************************/
/***********************************  SYSTEM  ***********************************/
#ifndef MOTORTESTSYSTEM_HH
#define MOTORTESTSYSTEM_HH


#include <dzn/locator.hh>

#include "button.hh"
#include "button.hh"
#include "Timer.hh"
#include "button.hh"



struct motorTestSystem
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::MotorControl mc;
  ::Button toggle;
  ::Button speedup;
  ::Timer timer;
  ::Button speeddown;

  ::IMotorControl& api;

  ::IButtonGlue& itoggle;
  ::IButtonGlue& ispeedup;
  ::IButtonGlue& ispeeddown;
  ::IMotorGlue& imotor;

  motorTestSystem(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // MOTORTESTSYSTEM_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.9.1
