#include <dzn/meta.hh>

namespace dzn {
  struct locator;
  struct runtime;
}



#include <iostream>
#include <map>


/********************************** INTERFACE *********************************/
#ifndef IBUTTONGLUE_HH
#define IBUTTONGLUE_HH



struct IButtonGlue
{
#ifndef ENUM_IButtonGlue_State
#define ENUM_IButtonGlue_State 1


  struct State
  {
    enum type
    {
      On,Off
    };
  };


#endif // ENUM_IButtonGlue_State

  struct
  {
    std::function< ::IButtonGlue::State::type()> get;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline IButtonGlue(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.get) throw dzn::binding_error(meta, "in.get");


  }
};

inline void connect (IButtonGlue& provided, IButtonGlue& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IButtonGlue_State
#define ENUM_TO_STRING_IButtonGlue_State 1
inline std::string to_string(::IButtonGlue::State::type v)
{
  switch(v)
  {
    case ::IButtonGlue::State::On: return "State_On";
    case ::IButtonGlue::State::Off: return "State_Off";

  }
  return "";
}
#endif // ENUM_TO_STRING_IButtonGlue_State

#ifndef STRING_TO_ENUM_IButtonGlue_State
#define STRING_TO_ENUM_IButtonGlue_State 1
inline ::IButtonGlue::State::type to_IButtonGlue_State(std::string s)
{
  static std::map<std::string, ::IButtonGlue::State::type> m = {
    {"State_On", ::IButtonGlue::State::On},
    {"State_Off", ::IButtonGlue::State::Off},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IButtonGlue_State


#endif // IBUTTONGLUE_HH

/********************************** INTERFACE *********************************/
/********************************** INTERFACE *********************************/
#ifndef ILEDGLUE_HH
#define ILEDGLUE_HH



struct ILedGlue
{
#ifndef ENUM_ILedGlue_State
#define ENUM_ILedGlue_State 1


  struct State
  {
    enum type
    {
      On,Off
    };
  };


#endif // ENUM_ILedGlue_State

  struct
  {
    std::function< void()> turnOn;
    std::function< void()> turnOff;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline ILedGlue(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.turnOn) throw dzn::binding_error(meta, "in.turnOn");
    if (! in.turnOff) throw dzn::binding_error(meta, "in.turnOff");


  }
};

inline void connect (ILedGlue& provided, ILedGlue& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_ILedGlue_State
#define ENUM_TO_STRING_ILedGlue_State 1
inline std::string to_string(::ILedGlue::State::type v)
{
  switch(v)
  {
    case ::ILedGlue::State::On: return "State_On";
    case ::ILedGlue::State::Off: return "State_Off";

  }
  return "";
}
#endif // ENUM_TO_STRING_ILedGlue_State

#ifndef STRING_TO_ENUM_ILedGlue_State
#define STRING_TO_ENUM_ILedGlue_State 1
inline ::ILedGlue::State::type to_ILedGlue_State(std::string s)
{
  static std::map<std::string, ::ILedGlue::State::type> m = {
    {"State_On", ::ILedGlue::State::On},
    {"State_Off", ::ILedGlue::State::Off},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_ILedGlue_State


#endif // ILEDGLUE_HH

/********************************** INTERFACE *********************************/
/********************************** INTERFACE *********************************/
#ifndef IMOTORGLUE_HH
#define IMOTORGLUE_HH



struct IMotorGlue
{

  struct
  {
    std::function< void()> speedUp;
    std::function< void()> speedDown;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline IMotorGlue(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.speedUp) throw dzn::binding_error(meta, "in.speedUp");
    if (! in.speedDown) throw dzn::binding_error(meta, "in.speedDown");


  }
};

inline void connect (IMotorGlue& provided, IMotorGlue& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}





#endif // IMOTORGLUE_HH

/********************************** INTERFACE *********************************/
/********************************** INTERFACE *********************************/
#ifndef IENCODERGLUE_HH
#define IENCODERGLUE_HH



struct IEncoderGlue
{
#ifndef ENUM_IEncoderGlue_Dir
#define ENUM_IEncoderGlue_Dir 1


  struct Dir
  {
    enum type
    {
      CW,CCW
    };
  };


#endif // ENUM_IEncoderGlue_Dir

  struct
  {
    std::function< void(int&)> position;
    std::function< ::IEncoderGlue::Dir::type()> direction;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline IEncoderGlue(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.position) throw dzn::binding_error(meta, "in.position");
    if (! in.direction) throw dzn::binding_error(meta, "in.direction");


  }
};

inline void connect (IEncoderGlue& provided, IEncoderGlue& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IEncoderGlue_Dir
#define ENUM_TO_STRING_IEncoderGlue_Dir 1
inline std::string to_string(::IEncoderGlue::Dir::type v)
{
  switch(v)
  {
    case ::IEncoderGlue::Dir::CW: return "Dir_CW";
    case ::IEncoderGlue::Dir::CCW: return "Dir_CCW";

  }
  return "";
}
#endif // ENUM_TO_STRING_IEncoderGlue_Dir

#ifndef STRING_TO_ENUM_IEncoderGlue_Dir
#define STRING_TO_ENUM_IEncoderGlue_Dir 1
inline ::IEncoderGlue::Dir::type to_IEncoderGlue_Dir(std::string s)
{
  static std::map<std::string, ::IEncoderGlue::Dir::type> m = {
    {"Dir_CW", ::IEncoderGlue::Dir::CW},
    {"Dir_CCW", ::IEncoderGlue::Dir::CCW},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IEncoderGlue_Dir


#endif // IENCODERGLUE_HH

/********************************** INTERFACE *********************************/
/********************************** INTERFACE *********************************/
#ifndef ITIMER_HH
#define ITIMER_HH



struct ITimer
{

  struct
  {
    std::function< void(int)> create;
    std::function< void()> cancel;
  } in;

  struct
  {
    std::function< void()> timeout;
  } out;

  dzn::port::meta meta;
  inline ITimer(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.create) throw dzn::binding_error(meta, "in.create");
    if (! in.cancel) throw dzn::binding_error(meta, "in.cancel");

    if (! out.timeout) throw dzn::binding_error(meta, "out.timeout");

  }
};

inline void connect (ITimer& provided, ITimer& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}





#endif // ITIMER_HH

/********************************** INTERFACE *********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_BUTTONGLUE_HH
#define SKEL_BUTTONGLUE_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>




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

  };
}

#endif // MOTORGLUE_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_ENCODERGLUE_HH
#define SKEL_ENCODERGLUE_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>




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


//version: 2.9.1
