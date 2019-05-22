#include <dzn/meta.hh>

namespace dzn {
  struct locator;
  struct runtime;
}



#include <iostream>
#include <map>

/********************************** INTERFACE *********************************/
#ifndef IINPUT_HH
#define IINPUT_HH




struct IInput
{
#ifndef ENUM_IInput_eState
#define ENUM_IInput_eState 1


  struct eState
  {
    enum type
    {
      idle,active
    };
  };


#endif // ENUM_IInput_eState
#ifndef ENUM_IInput_eIO
#define ENUM_IInput_eIO 1


  struct eIO
  {
    enum type
    {
      high,low
    };
  };


#endif // ENUM_IInput_eIO

  struct
  {
    std::function< void()> activate;
    std::function< void()> deactivate;
    std::function< bool()> getValue;
  } in;

  struct
  {
    std::function< void()> changed;
  } out;

  dzn::port::meta meta;
  inline IInput(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.activate) throw dzn::binding_error(meta, "in.activate");
    if (! in.deactivate) throw dzn::binding_error(meta, "in.deactivate");
    if (! in.getValue) throw dzn::binding_error(meta, "in.getValue");

    if (! out.changed) throw dzn::binding_error(meta, "out.changed");

  }
};

inline void connect (IInput& provided, IInput& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IInput_eState
#define ENUM_TO_STRING_IInput_eState 1
inline std::string to_string(::IInput::eState::type v)
{
  switch(v)
  {
    case ::IInput::eState::idle: return "eState_idle";
    case ::IInput::eState::active: return "eState_active";

  }
  return "";
}
#endif // ENUM_TO_STRING_IInput_eState
#ifndef ENUM_TO_STRING_IInput_eIO
#define ENUM_TO_STRING_IInput_eIO 1
inline std::string to_string(::IInput::eIO::type v)
{
  switch(v)
  {
    case ::IInput::eIO::high: return "eIO_high";
    case ::IInput::eIO::low: return "eIO_low";

  }
  return "";
}
#endif // ENUM_TO_STRING_IInput_eIO

#ifndef STRING_TO_ENUM_IInput_eState
#define STRING_TO_ENUM_IInput_eState 1
inline ::IInput::eState::type to_IInput_eState(std::string s)
{
  static std::map<std::string, ::IInput::eState::type> m = {
    {"eState_idle", ::IInput::eState::idle},
    {"eState_active", ::IInput::eState::active},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IInput_eState
#ifndef STRING_TO_ENUM_IInput_eIO
#define STRING_TO_ENUM_IInput_eIO 1
inline ::IInput::eIO::type to_IInput_eIO(std::string s)
{
  static std::map<std::string, ::IInput::eIO::type> m = {
    {"eIO_high", ::IInput::eIO::high},
    {"eIO_low", ::IInput::eIO::low},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IInput_eIO


#endif // IINPUT_HH

/********************************** INTERFACE *********************************/


//version: 2.8.2
