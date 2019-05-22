#include <dzn/meta.hh>

namespace dzn {
  struct locator;
  struct runtime;
}



#include <iostream>
#include <map>

/********************************** INTERFACE *********************************/
#ifndef IIO_HH
#define IIO_HH




struct IIO
{

  struct
  {
    std::function< bool()> getValue;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline IIO(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.getValue) throw dzn::binding_error(meta, "in.getValue");


  }
};

inline void connect (IIO& provided, IIO& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}





#endif // IIO_HH

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
/********************************** COMPONENT *********************************/
#ifndef INPUTCONTROLLER_HH
#define INPUTCONTROLLER_HH

#include "IInput.hh"
#include "ITrigger.hh"



struct InputController
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;

  ::IInput::eState::type state;
  bool value;

  bool reply_bool;

  std::function<void ()> out_api;

  ::IInput api;

  ::ITrigger trigger;
  ::IIO io;


  InputController(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const InputController& m)  {
    (void)m;
    return os << "[" << m.state <<", " << m.value <<"]" ;
  }
  private:
  void api_activate();
  void api_deactivate();
  bool api_getValue();
  void trigger_trigger();

};

#endif // INPUTCONTROLLER_HH

/********************************** COMPONENT *********************************/
/***********************************  SYSTEM  ***********************************/
#ifndef INPUTDEZYNE_HH
#define INPUTDEZYNE_HH


#include <dzn/locator.hh>

#include "Trigger.hh"



struct InputDezyne
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::Trigger trigger;
  ::InputController control;

  ::IInput& api;

  ::IIO& input;

  InputDezyne(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // INPUTDEZYNE_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.8.2
