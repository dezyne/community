#include <dzn/meta.hh>

namespace dzn {
  struct locator;
  struct runtime;
}



#include <iostream>
#include <map>

/********************************** INTERFACE *********************************/
#ifndef ITRIGGER_HH
#define ITRIGGER_HH




struct ITrigger
{

  struct
  {
  } in;

  struct
  {
    std::function< void()> trigger;
  } out;

  dzn::port::meta meta;
  inline ITrigger(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {

    if (! out.trigger) throw dzn::binding_error(meta, "out.trigger");

  }
};

inline void connect (ITrigger& provided, ITrigger& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}





#endif // ITRIGGER_HH

/********************************** INTERFACE *********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_TRIGGER_HH
#define SKEL_TRIGGER_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>




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


//version: 2.8.2
