#include <dzn/meta.hh>

namespace dzn {
  struct locator;
  struct runtime;
}



#include <iostream>
#include <map>

/********************************** INTERFACE *********************************/
#ifndef IOUTPUT_HH
#define IOUTPUT_HH




struct IOutput
{

  struct
  {
    std::function< void()> high;
    std::function< void()> low;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline IOutput(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.high) throw dzn::binding_error(meta, "in.high");
    if (! in.low) throw dzn::binding_error(meta, "in.low");


  }
};

inline void connect (IOutput& provided, IOutput& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}





#endif // IOUTPUT_HH

/********************************** INTERFACE *********************************/


//version: 2.8.2
