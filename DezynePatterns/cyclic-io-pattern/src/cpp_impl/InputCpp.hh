
#pragma once

#include "InputCpp.hh"

class InputCpp : public skel::InputCpp
{
public:
  explicit InputCpp(const dzn::locator& dzn_locator) :
    skel::InputCpp(dzn_locator),
    _channel(nullptr),
    state(IInput::eState::idle)
  {
    _previous = _channel;
  }

  void setChannel(bool* channel) { _channel = channel; }

  void run()
  {
    if (_previous != *_channel)
    {
      _previous = _channel;
      if (state == IInput::eState::active)
      {
        api.out.changed();
      }
    }
  }

private:
  void api_activate() override
  {
    state = IInput::eState::active;
  }

  void api_deactivate() override
  {
    state = IInput::eState::idle;
  }

  bool api_getValue() override
  {
    return _previous;
  }

  bool* _channel;
  bool _previous;
  IInput::eState::type state;
};
