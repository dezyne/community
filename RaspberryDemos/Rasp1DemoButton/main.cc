#include "buttonled.hh"
#include <wiringPi.h>
#include <TimerID.hh>

int main()
{
  // Initializations for Raspberry
  wiringPiSetupGpio () ; // Broadcom pin numbering!

#define PWM_PIN 18 //wiringPi P1 (physical pin 12) PWM
//  pinMode (PWM_PIN, PWM_OUTPUT) ;

#define IN1 5  //P21 (physical pin 29)
#define IN2 6  //P22 (physical pin 31)
#define IN3 13 //P23 (physical pin 33)
#define IN4 19 //P24 (physical pin 35)

  pinMode (IN1, INPUT) ; 
  pinMode (IN2, INPUT) ;
  pinMode (IN3, INPUT) ;
  pinMode (IN4, INPUT) ;

  pullUpDnControl (IN1, PUD_UP);
  pullUpDnControl (IN2, PUD_UP);
  pullUpDnControl (IN3, PUD_UP);
  pullUpDnControl (IN4, PUD_UP);

//LEDs
#define OUT1 26 //P25 (physical pin 37)
#define OUT2 12 //P26 (physical pin 32)
#define OUT3 16 //P27 (physical pin 36)
#define OUT4 20 //P28 (physical pin 38)

  pinMode (OUT1, OUTPUT) ;
  pinMode (OUT2, OUTPUT) ;
  pinMode (OUT3, OUTPUT) ;
  pinMode (OUT4, OUTPUT) ;
  
 // Initializations for Dezyne
  std::clog.rdbuf(0); //suppress dezyne tracing
  
  dzn::locator l;
  dzn::runtime r;
  dzn::pump pump;
  TimerID timerid;
  LedTestSystem lts(l.set(r).set(pump).set(timerid));

  // Bindings
  lts.buttonGlue.in.get = []{ return (digitalRead (IN1)? ::IButtonGlue::State::type::Off : ::IButtonGlue::State::type::On); };
  lts.ledGlue.in.turnOn = []{ digitalWrite (OUT1, HIGH); };
  lts.ledGlue.in.turnOff = []{ digitalWrite (OUT1, LOW); };
  lts.check_bindings();
  
  // Start loop
  std::cout << "press key0 for led0" << std::endl;
  dzn::shell(pump, lts.api.in.start);
  std::this_thread::sleep_for(std::chrono::hours(1));
  dzn::shell(pump, lts.api.in.stop);
}
