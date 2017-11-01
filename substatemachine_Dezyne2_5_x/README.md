# Sub State Machine pattern

A Sub State Machine (SSM) is a means to encapsulate part of the behaviour inside a component.
Currently in Dezyne there is no syntax support for SSMs so the encapsulation is only for easier understanding.

## This example shows following features:
* an event arrives in the high level state machine and is forwarded to a SSM. 
Some processing might take place in the high level state machine and then be continued in the SSM before control is passed back
* an exit of the SSM is captured in the high level state machine which might do some postprocessing
* result values can be passed from a SSM to a higher level state machine by means of variables
* by default the SSM retains the value of the state variable between activations (deep). In the example the SSM state variable is initialized
on entry of the SSM and redundantly also on exit (shallow).
In other words the designer has full control over the choice for deep/shallow state behaviour.

## Due to the fact there is no explicit syntax this example also shows some limitations:
* we would like to add scoping to the variables but currently everything can only be global within the component.
For the result value passing we make use of this.
* we would like to put the code for the SSM in separate blocks (or even separate files) but currently everything has to be
within the behaviour block of the component. We have put it in separate code blocks protected by the appropriate guards.

## Event forwarding

This is implemented by adding a special requires port to the component that on every event does a synchronous call-back.
The forward raises that event and the call-back is stored in the output event queue of the component.
It will be processed once the full statement block that contained the forward has been executed.
The call back will be captured in the SSM and might e.g. be mapped on a dedicated "on entry" event.
As a result an event to the high level state machine results in some actions at high level state amchine level and
subsequently some actions at SSM level before control is passed back

The exit works similarly but then everything mirrored.

Below diagram shows an example with high level states 'one', 'two', 'three' and substates 'one_a', 'one_b' for SSM1 and 'two_a', 'two_b' for SSM2.
In the corresponding DZN file the SSM state variable is reused between the two SSMs since they are not active in parallel.

![](images/ssm.png)

