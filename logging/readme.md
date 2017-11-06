# Logging 


## how to create a StreamLogger class 

To redirect the logging output from dezyne to a custum logger framework a special object can be
added to the locator. An example implemenation is available as StreamLogger.h [here](https://github.com/VerumSoftwareTools/DezyneCommunity/blob/master/examples/StreamLogger.h)

```cpp
  void main()
  {
    auto f = [](const char* message) {
      std::cout << message << postfix_threadid();
    };

    dzn::locator loc;
    StreamLogger sl(f);
    loc.set(static_cast<std::ostream&>(sl));  // notice static_cast 

    dzn::runtime rt;
    loc.set(rt);

    AlarmSystem as(loc);
    
    ~~~ more code follows ~~~
   }
```
Notice: The custom class StreamLogger must be inserted into the locator as a 'std::ostream' typed object, otherwise, it will not be picked up by the runtime
 

## Customization of the runtime to make logging output more human readable

The information being output by dezyne models as of version 2.1.1-2.5.3 is complete in the sense that is mechanically possible to reconstruct the exact events from the output, if you have the model available. This is for example excellent if you want to direct this information back into the simualor for analysis.

However, if you want to read and understand the logging without cross-referencing it with a Dezyne model this is form is not ideal.
There is an [ongoing discussion](https://github.com/janwilmans/DezyneSamples/issues/1) and effort to make the output coming from dezyne models customizable.

A working prototype of a modified Runtime is available [here](https://github.com/VerumSoftwareTools/DezyneCommunity/tree/master/logging/modified_runtime_cpp_2.5.3)

Originally the output was:

```
<external>.arm -> .alarm.console.arm [0, 0]
sensor enabled
.alarm.console.return -> <external>.return [1, 0]
<external>.triggered -> .alarm.sensor.triggered [1, 0]
Detected!
siren on!
``` 

With the modified runtime it looks like:
```
<external>.console.arm -> .alarm.console.arm [enter] [0, 0] (tid: 9288)
sensor enabled (tid: 9288)
<external>.console.arm <- .alarm.console.arm [leave] [1, 0] (return) (tid: 9288)
.alarm.sensor.triggered -> <external>.sensor.triggered [enter] [1, 0] (tid: 9288)
Detected! (tid: 9288)
siren on! (tid: 9288)
<external>.sensor.triggered <- .alarm.sensor.triggered [leave] [1, 1] (tid: 9288)
```
Notice that the postfixed threadid is added in the main and is not part of the runtime modification.
The modification is quite simple and is contained only in Runtime.hh, it can also be customized as desired.
The change can be viewed [here](https://github.com/VerumSoftwareTools/DezyneCommunity/commit/f4b331fc8e816efcef6c78cf574472a7d9ceddaf).

# Future version

If the proposed changes in the code generation will be accepted and implemented the output could look like this:

```
<external>.console.arm -> .alarm.console.arm [enter] [state: Disarmed, sounding: false] (tid: 9288)
sensor enabled (tid: 9288)
<external>.console.arm <- .alarm.console.arm [leave] [state: Armed, sounding: false] (return) (tid: 9288)
.alarm.sensor.triggered -> <external>.sensor.triggered [enter] [state: Armed, sounding: false] (tid: 9288)
Detected! (tid: 9288)
siren on! (tid: 9288)
<external>.sensor.triggered <- .alarm.sensor.triggered [leave] [state: Armed, sounding: true] (tid: 9288)
```

Here the names of the component state enum have been added and the name of the variables prefixed.

