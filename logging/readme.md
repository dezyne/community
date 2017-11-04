# Logging 


## how to create a StreamLogger class 

To redirect the logging output from dezyne to a custum logger framework a special object can be
added to the locator. An example implemenation is available [here](https://github.com/VerumSoftwareTools/DezyneCommunity/blob/master/examples/StreamLogger.h)

## Customization of the runtime to make logging output more human readable

The information being output by dezyne models as of version 2.1.1-2.5.3 is complete in the sense that is mechanically possible to reconstruct the exact events from the output, if you have the model available. This is for example excellent if you want to direct this information back into the simualor for analysis.

However, if you want to read and understand the logging without cross-referencing it with a Dezyne model this is form is not ideal.
There is an [ongoing discussion](https://github.com/janwilmans/DezyneSamples/issues/1) and effort to make the output coming from dezyne models customizable.


