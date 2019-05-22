#include <dzn/meta.hh>

namespace dzn {
  struct locator;
  struct runtime;
}



#include <iostream>
#include <map>

/********************************** INTERFACE *********************************/
#ifndef ISTART_HH
#define ISTART_HH




struct IStart
{

  struct
  {
    std::function< void()> start;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline IStart(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");


  }
};

inline void connect (IStart& provided, IStart& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}





#endif // ISTART_HH

/********************************** INTERFACE *********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_TRIGGER_HH
#define SKEL_TRIGGER_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "ITrigger.hh"



namespace skel {
  struct Trigger
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::ITrigger api;


    Trigger(const dzn::locator& dzn_locator)
    : dzn_meta{"","Trigger",0,0,{},{},{[this]{api.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , api({{"api",this,&dzn_meta},{"",0,0}})


    {


    }
    virtual ~ Trigger() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const Trigger& m)  {
      return m.stream_members(os);
    }
    private:

  };
}

#endif // TRIGGER_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_INPUTCPP_HH
#define SKEL_INPUTCPP_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "IInput.hh"



namespace skel {
  struct InputCpp
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::IInput api;


    InputCpp(const dzn::locator& dzn_locator)
    : dzn_meta{"","InputCpp",0,0,{},{},{[this]{api.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , api({{"api",this,&dzn_meta},{"",0,0}})


    {
      api.in.activate = [&](){return dzn::call_in(this,[=]{ return api_activate();}, this->api.meta, "activate");};
      api.in.deactivate = [&](){return dzn::call_in(this,[=]{ return api_deactivate();}, this->api.meta, "deactivate");};

      api.in.getValue = [&](){return dzn::call_in(this,[=]{ return api_getValue();}, this->api.meta, "getValue");};

    }
    virtual ~ InputCpp() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const InputCpp& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void api_activate () = 0;
    virtual void api_deactivate () = 0;
    virtual bool api_getValue () = 0;

  };
}

#endif // INPUTCPP_HH

/***********************************  FOREIGN  **********************************/
/********************************** COMPONENT *********************************/
#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include "IInput.hh"
#include "IOutput.hh"
#include "IInput.hh"



struct Controller
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;
#ifndef ENUM_Controller_eState
#define ENUM_Controller_eState 1


  struct eState
  {
    enum type
    {
      idle,active
    };
  };


#endif // ENUM_Controller_eState

  ::Controller::eState::type state;

  bool reply_bool;

  std::function<void ()> out_api;

  ::IStart api;

  ::IInput cpp;
  ::IOutput io;
  ::IInput slow;


  Controller(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const Controller& m)  {
    (void)m;
    return os << "[" << m.state <<"]" ;
  }
  private:
  void api_start();
  void cpp_changed();
  void slow_changed();

  void eval ();
};

#endif // CONTROLLER_HH

/********************************** COMPONENT *********************************/
/***********************************  SYSTEM  ***********************************/
#ifndef CONTROLSYSTEM_HH
#define CONTROLSYSTEM_HH


#include <dzn/locator.hh>

#include "InputDezyne.hh"
#include "InputCpp.hh"



struct ControlSystem
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::Controller control;
  ::InputDezyne dezyne;
  ::InputCpp cpp;

  ::IStart& api;

  ::IOutput& io;
  ::IIO& input;

  ControlSystem(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // CONTROLSYSTEM_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.8.2
