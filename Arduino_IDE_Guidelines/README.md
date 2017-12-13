# Install the Arduino plugin

In Dezyne goto menu item 'help', 'welcome'.
On the welcome page select 'Third party tools'
Select install and follow the steps of the installer

# Create a new project

Select new, project, Arduino Sketch, next
(in the Arduino perspective this can be done in one step)
Select your board type & processor, next
Select 'Default cpp file', finish

The first time you might prefer to start from an example.

# Configure the project

If you want you can put all source files in the main directory of the project. 
However, it quickly becomes crowded so it makes sense to separate some parts in subdirectories. 
For that purpose create a local directory dzn_runtime and download the Dezyne runtime C files into it. 
Use following procedure: Select the Arduino project, click right mouse button, select ‘new’, select ‘source folder’.
Similarly create a local directory dzn_generated.
When generating code in a Dezyne project select this location to generate the source files into. 
Arduino IDE then automatically keeps track of those files and maintains the proposer build dependencies.
This is the resulting project explorer view in Eclipse:


![](Arduino_project_setup1.png)

Figure 1: project explorer view
 
These 2 directories also contain include files that must become findable by the compiler. 
Edit the project properties and add these directories.
You can do it for both the C and C++ compiler (whether you want to work in C or C++ or both if you use a mixture).
While adding them make sure you select the workspace as root of the path.


![](Arduino_project_setup2.png)

Figure 2: include directories settings

To keep the generated code as small as possible you can add an undocumented flag (Tiny-C) to the compiler settings.
You can see its usage in the runtime files.


![](Arduino_project_setup3.png)

Figure 3: add Tiny-C compile flag

# Code generation
Code generation needs to be done in ”C”, whereas most hardware drivers (and the Serial driver, for
trace output) for the Arduino are coded in C++. A way to work around this is to:
1) Create an additional C file that does dzn run time initialization, and implements the callback
functions.
2) In your main CPP file (that contains the setup() and loop() functions) define a C function that in
their turn calls the C++ functions. 

# Closing remarks
before you can upload to your board you must ensure to have the right USB drivers installed. There is ample info on the web.
See Arduino-IDE (Sloeber) documentation how to use the serail monitor etc.



