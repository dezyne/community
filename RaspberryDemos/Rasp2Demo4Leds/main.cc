#include "ledcontrol.hh"
#include <wiringPi.h>
#include <TimerID.hh>

int main()
{
  // Initializations for Raspberry
  wiringPiSetupGpio () ; // Broadcom pin numbering!

#define PWM_PIN 18 //wiringPi P1 (physical pin 12) PWM
  pinMode (PWM_PIN, PWM_OUTPUT) ;
  static int pwmValue = 0;
  pwmWrite (PWM_PIN, 0);

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
  
  digitalWrite (OUT1, LOW); 
  digitalWrite (OUT2, LOW); 
  digitalWrite (OUT3, LOW); 
  digitalWrite (OUT4, LOW); 

//Motor
#define M_FORWARD 22 //P3 (physical pin 15)
#define M_BACKWARD 23 //P4 (physical pin 16)

  pinMode (M_FORWARD, OUTPUT);
  pinMode (M_BACKWARD, OUTPUT);
  
 // Initializations for Dezyne
  std::clog.rdbuf(0); //suppress dezyne tracing
  
  dzn::locator l;
  dzn::runtime r;
  dzn::pump pump;
  TimerID timerid;
  std::cout << "create system"  << std::endl;
  ledControlSystem lcs(l.set(r).set(pump).set(timerid));

  // Bindings
  lcs.button1.in.get = []{ return (digitalRead (IN1)? ::IButtonGlue::State::type::Off : ::IButtonGlue::State::type::On); };
  lcs.button2.in.get = []{ return (digitalRead (IN2)? ::IButtonGlue::State::type::Off : ::IButtonGlue::State::type::On); };
  lcs.button3.in.get = []{ return (digitalRead (IN3)? ::IButtonGlue::State::type::Off : ::IButtonGlue::State::type::On); };
  lcs.button4.in.get = []{ return (digitalRead (IN4)? ::IButtonGlue::State::type::Off : ::IButtonGlue::State::type::On); };

  lcs.leds.in.led1On = []{ digitalWrite (OUT1, HIGH); std::cout << "led1 on"  << std::endl;};
  lcs.leds.in.led2On = []{ digitalWrite (OUT2, HIGH); std::cout << "led2 on"  << std::endl;};
  lcs.leds.in.led3On = []{ digitalWrite (OUT3, HIGH); std::cout << "led3 on"  << std::endl;};
  lcs.leds.in.led4On = []{ digitalWrite (OUT4, HIGH); std::cout << "led4 on"  << std::endl;};
  lcs.leds.in.led1Off = []{ digitalWrite (OUT1, LOW); std::cout << "led1 off"  << std::endl;};
  lcs.leds.in.led2Off = []{ digitalWrite (OUT2, LOW); std::cout << "led2 off"  << std::endl;};
  lcs.leds.in.led3Off = []{ digitalWrite (OUT3, LOW); std::cout << "led3 off"  << std::endl;};
  lcs.leds.in.led4Off = []{ digitalWrite (OUT4, LOW); std::cout << "led4 off"  << std::endl;};
  lcs.check_bindings();
  
  // Start loop
  std::cout << "press button i for led i << std::endl;"  << std::endl;

	dzn::shell(pump, lcs.api.in.start);
	std::this_thread::sleep_for(std::chrono::hours(1));
	dzn::shell(pump, lcs.api.in.stop);
}



