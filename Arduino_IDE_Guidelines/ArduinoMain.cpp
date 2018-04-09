/*
 * all the low level drivers for Arduino
 * but also the setup and loop functions
 */
#include <arduino.h>
#include <servo.h>

extern "C"
{
#include "DezyneGlue.h"
}

#define MOTOR_RIGHT_ENABLE  (5)
#define MOTOR_RIGHT_IN_1    (7)
#define MOTOR_RIGHT_IN_2    (8)
#define MOTOR_LEFT_IN_3     (10)
#define MOTOR_LEFT_IN_4     (9)
#define MOTOR_LEFT_ENABLE   (6)

#define MOTOR_IN1_FORWARD (1)
#define MOTOR_IN2_FORWARD (0)

#define MOTOR_IN1_BACKWARD (0)
#define MOTOR_IN2_BACKWARD (1)

#define US_TRIGGER          (12)
#define US_ECHO             (13)

#define LINESENSOR_LEFT		(4)
#define LINESENSOR_MIDDLE	(3)
#define LINESENSOR_RIGHT	(2)

#define SERVOPIN			(11)

// focus for servo angle
int servo_focus;

// serial debug/info prints
const char string_0[] PROGMEM = "Motor LeftForward";
const char string_1[] PROGMEM = "Motor RightForward";
const char string_2[] PROGMEM = "Motor LeftBackward";
const char string_3[] PROGMEM = "Motor RightBackward";
const char string_4[] PROGMEM = "Motor LeftStop";
const char string_5[] PROGMEM = "Motor RightStop";
const char string_6[] PROGMEM = "Motor Sleep";
const char* const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5, string_6};

Servo servo;

// Local function Definitions
void SerialPrint(const int i);
void ServoFocus(int Angle);
void ServoRotate(int Angle);
void SetLeftMotor(int Speed);
void SetRightMotor(int Speed);
void ObstacleScan();
void SensorScan();

// Do initial setup function, including setup of dezyne glue layer
void setup() {

	Serial.begin(9600);
    Serial.println(F("Start setup"));

    pinMode (MOTOR_RIGHT_ENABLE, OUTPUT);
    pinMode (MOTOR_RIGHT_IN_1, OUTPUT);
    pinMode (MOTOR_RIGHT_IN_2, OUTPUT);
    pinMode (MOTOR_LEFT_IN_3, OUTPUT);
    pinMode (MOTOR_LEFT_IN_4, OUTPUT);
    pinMode (MOTOR_LEFT_ENABLE, OUTPUT);

    pinMode(US_TRIGGER, OUTPUT); // Sets the trigPin as an Output
    pinMode(US_ECHO, INPUT); // Sets the echoPin as an Input

    pinMode(LINESENSOR_LEFT,INPUT);
    pinMode(LINESENSOR_MIDDLE,INPUT);
    pinMode(LINESENSOR_RIGHT,INPUT);

    pinMode(SERVOPIN,OUTPUT);

    //    servo.attach (SERVOPIN);
    ServoFocus (SERVOMID);

    // make some functions known to the Dezyne glue code & start robot
    DG_setup(SerialPrint, ServoFocus, SetLeftMotor, SetRightMotor);

    Serial.println(F("End setup"));
}

const long loopTime = 100;
long now = 0;
long prev = 0;
int servoAngle = SERVOMID;
int servoDirection = 1;

void loop()
{
	ObstacleScan();
	SensorScan();

	now = millis();
	if (now - prev > loopTime) {
		ServoRotate (servoAngle);
		servoAngle = servoAngle + 10 * servoDirection;

//		Serial.print(F("Servo Angle: "));
//		Serial.print(servoAngle);
//		Serial.print(F("   Servo Direction: "));
//		Serial.println(servoDirection);

		if (servoAngle > servo_focus + SERVORANGE) servoDirection = -1;
		else if (servoAngle < servo_focus - SERVORANGE) servoDirection = 1;

		prev = now;
	}
}

void SerialPrint(const int i)
{
	char buffer[30];
	strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i])));
	Serial.println(buffer);
}

void SetLeftMotor(int Speed)
{
  if (Speed == 0)
  {
    analogWrite(MOTOR_LEFT_ENABLE, 0);
  }
  else if (Speed > 0)
  {
    analogWrite(MOTOR_LEFT_ENABLE, Speed);
    digitalWrite(MOTOR_LEFT_IN_3, MOTOR_IN1_FORWARD);
    digitalWrite(MOTOR_LEFT_IN_4, MOTOR_IN2_FORWARD);
  }
  else
  {
    analogWrite(MOTOR_LEFT_ENABLE, -Speed);
    digitalWrite(MOTOR_LEFT_IN_3, MOTOR_IN1_BACKWARD);
    digitalWrite(MOTOR_LEFT_IN_4, MOTOR_IN2_BACKWARD);
  }
}

void SetRightMotor(int Speed)
{
  if (Speed == 0)
  {
    analogWrite(MOTOR_RIGHT_ENABLE, 0);
  }
  else if (Speed > 0)
  {
    analogWrite(MOTOR_RIGHT_ENABLE, Speed);
    digitalWrite(MOTOR_RIGHT_IN_1, MOTOR_IN1_FORWARD);
    digitalWrite (MOTOR_RIGHT_IN_2, MOTOR_IN2_FORWARD);
  }
  else
  {
    analogWrite(MOTOR_RIGHT_ENABLE, -Speed);
    digitalWrite(MOTOR_RIGHT_IN_1, MOTOR_IN1_BACKWARD);
    digitalWrite (MOTOR_RIGHT_IN_2, MOTOR_IN2_BACKWARD);
  }
}

/*
 * In this function an event into Dezyne is raised every time the function is called.
 * Filtering to only act if the status has changed: done in Dezyne.
 */
void ObstacleScan()
{
   long duration = 0;
   int distance = 0;
  digitalWrite(US_TRIGGER, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(US_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIGGER, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(US_ECHO, HIGH, 1000000);
  // Calculating the distance
  distance= duration*0.034/2;

	// keep status only for debug print
	static int wasCloseby = 0;
	if (distance < 15 && wasCloseby == 0) {
		Serial.println(F("-> Danger"));
		wasCloseby = 1;
	}
	else if (distance >= 15 && wasCloseby == 1) {
		Serial.println(F("-> NoDanger"));
		wasCloseby = 0;
	}
	// end debug code

  if (distance < 15)
  {
	  DG_Danger();
  }
  else
  {
	  DG_NoDanger();
  }
}

/*
 * In this function an event into Dezyne is only raised if there is a change of
 * status.
 * Filtering to only act if the status has changed in Dezyne is then not needed anymore
 */
void SensorScan()
{
	unsigned char SL;
//	unsigned char SM;
	unsigned char SR;

	SL = digitalRead(LINESENSOR_LEFT);
//	SM = digitalRead(LINESENSOR_MIDDLE);
	SR = digitalRead(LINESENSOR_RIGHT);

	static int status, wasStatus = -1;
 	if (SL == SR) status = 0;
	else if (SR)  status = 1;
	else /* SL */ status = 2;
	if (status != wasStatus) {
		if(SL == SR) {
			Serial.println(F("-> Line Straight"));
			DG_RobotStraight();
		}
		else if (SR) {
			Serial.println(F("-> Line Right"));
			DG_RobotRight();
		}
		else {
			Serial.println(F("-> Line Left"));
			DG_RobotLeft();
		}
		wasStatus = status;
	}
	else {
//		Serial.println(F("-> Line unchanged"));
	}
}

/*
 * Not so nice implementation with Delays
 * Would be cleaner to use a mechanism with timers & interrupts
 * Might look at servo library
 */
void ServoRotate(int myangle)
{
	int pulsewidth;

	pulsewidth=(myangle*11)+500;// 500-2480

    for(int i=0;i<=50;i++)
    {
      digitalWrite(SERVOPIN,HIGH);
      delayMicroseconds(pulsewidth);
      digitalWrite(SERVOPIN,LOW);
      delay(20-pulsewidth/1000);
    }
}

void ServoFocus (int focus)
{
	servo_focus = focus;
}
