Invariant Pattern
=================

Using the invariant pattern it is possible to check conditions between each trace steps.

```dezyne
interface Invariant {
    out void check();
    in void dummy();

    behaviour {
        on optional: check;
        on dummy: {}
    }
}

component InvariantDummy {
    provides Invariant dummy;
}
```

[Download Invariant.dzn](Invariant.dzn)

Require the invariant interface in an component. By connecting a function to the check callback, this function will be called between all trace steps. The function should check if an given invariant holds, if not call `illegal`.

[Example Monitor with Timer](Example.dzn)

The example shows an timer with an exposed state variable. The invariant is used to check that the timer is armed when the monitor is running. In the case of this example the user forgot to restart the timer.

Discussion
----------

This is an simplified example for which the problem can be solved in a better way.

Adding the invariant interface can make the verification time significantly longer.

Because the invariant can always call the check event, this could prevent deadlock detection in the component. It is best to do a verification with and without the variant pattern enabled.

For runtime you can implement an component that implements the invariant interface. Because check will never be fired, it will have no impact on runtime speed.

Resources
---------

* [Presentation (pdf)](DezyneInvariant.pdf)
* [Invariant.dzn](Invariant.dzn)
* [Example.dzn](Example.dzn)

---
Joran Jessurun <joran.jessurun@sioux.eu>
