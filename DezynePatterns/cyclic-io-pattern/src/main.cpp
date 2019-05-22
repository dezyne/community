#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>


#include "dzn_code/Controller.hh"

bool running;

int main()
{
  using namespace std::chrono_literals;
  running = true;

  signal(SIGINT, [](int) { running = false; });
  signal(SIGTERM, [](int) { running = false; });

  // setup "channels"
  bool input1 = false;
  bool input2 = false;
  bool output = false;

  // a counter for some modulos to change input value.
  int counter = 0;

  // Setup the system.
  dzn::locator loc;
  dzn::runtime run;
  ControlSystem control(loc.set(run));

  // connect.
  control.input.in.getValue = [&] { return input1; };
  control.cpp.setChannel(&input2);
  control.io.in.high = [&] { output = true; };
  control.io.in.low = [&] { output = false; };

  // Validate connections.
  control.check_bindings();

  std::clog.rdbuf(nullptr); // no dezyne logging.


  // The initial system trigger:
  control.api.in.start();
  while (running)
  {
    // Do the cyclic triggers for dezyne trigger:
    control.dezyne.trigger.run();

    // run the pure c++ component.
    control.cpp.run();

    // give a system output.

    std::cout << std::boolalpha << "input1(" << input1 << ") && input2(" << input2 << ") = " << output << std::endl;


    // some input manipulation.
    counter++;
    input1 = (counter % 3) == 0;
    input2 = (counter % 2) == 0;
    std::this_thread::sleep_for(1s);
  }
}