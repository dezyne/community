# Cyclic IO pattern

This example showcases a pattern that can be appliend when dealing with cyclic control systems, for example PLC or bus 
controlled systems. Typically these systems have a control function that is run every fixed time interval.

To deal with IO changes, normally a construction as follows is used:
```objectivec
void processCyclic(int time_interval)
{
  if (IOChannel.Value() != _previousIOValue)
  {
    // Act on value change.
  }
}
```

Out of the box Dezyne does not provide a polling mechanism for detecting IO changes.

There are two ways to approach this issue, both will be illustrated here. The example has a basic implementation for 
both.

## The IO eventbased interface

Let's first define an event based IO interface that we can use in our Dezyne models
```
interface IInput
{
  in void activate();
  in void deactivate();
  in bool getValue();
  out void changed();
  enum eState {idle, active};
  enum eIO {high, low};

  behaviour
  {
    eState state = eState.idle;
    eIO io = eIO.high;

    [state.idle]
    {
      on activate:
      {
        [true] {state = eState.active; io = eIO.low;}
        [true] {state = eState.active; io = eIO.high;}
      }
      on deactivate: {}
      on getValue: reply(false);
      on getValue: reply(true);
    }
    [state.active]
    {
      on deactivate: state = eState.idle;
      on activate: illegal;
      on getValue:
      {
        [io.low] reply(false);
        [io.high] reply(true);
      }
      on optional:
      {
        [io.low] {changed; io = eIO.high;}
        [io.high] {changed; io = eIO.low;}
      }
    }
  }
}
```

In the idle state, this interface can be used to get the current value of the IO with the `getValue` event. When 
activated the component will produce an `changed` event when the state of the IO changes.

## Polling with Dezyne

First we can implement a polling mechanism in dezyne. We need to create a trigger interface for this. The trigger event
needs to be called by the C++ application every cycle in order to validate the IO input. In addition we need to have an 
interface to obtain the current status of the IO.
````
interface ITrigger
{
  out void trigger();

  behaviour
  {
    on inevitable: trigger;
  }
}

interface IIO
{
  in bool getValue();

  behaviour
  {
    on getValue: reply(true);
    on getValue: reply(false);
  }
}
````

With these interfaces we can provide a component that provides the IInput interface.
```
component InputController
{
  provides IInput api;
  requires ITrigger trigger;
  requires IIO io;

  behaviour
  {
    IInput.eState state = IInput.eState.idle;
    bool value = false;

    [state.idle]
    {
      on api.activate(): state = IInput.eState.active;
      on api.deactivate(): {}
      on trigger.trigger(): value = io.getValue();
    }
    [state.active]
    {
      on api.deactivate(): state = IInput.eState.idle;
      on trigger.trigger():
      {
        bool newVal = io.getValue();
        if (newVal != value)
        {
          value = newVal;
          api.changed();
        }
      }
    }
    on api.getValue(): reply(value);
  }
}
```

Advantages of this approach:
 * Model checking on the polling behaviour of the IO.
 
Disadvantages:
 * The cyclic trigger generated C++ causes for a lot of overhead in C++ execution team each cycle.
 * The Trigger event has to be addressed in each state, cluttering your model.
 * The recurring trigger event could possibly hide livelock situations.
 
## C++ polling

In this approach we create a hand written component in C++ that takes care of all the polling and only gives an event 
when something actually changes.
```
component InputCpp
{
  provides IInput api;
}
```
And the C++ code:
```objectivec
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
```
Advantages of this approach:
 * Much better performance.
 * Less clutter in Dezyne components.
 * Clear seperation, polling in C++ and all event-based control in Dezyne.

Disadvantages:
 * No model checking on handwritten component.
 * Handwritten component has to be tested very carefully.