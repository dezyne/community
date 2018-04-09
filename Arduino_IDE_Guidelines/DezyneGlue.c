
/*
 * glue code between Arduino specific code and Dezyne generated code
 * prefix DG_ "Dezyne Glue" -> called from within Arduino code
 * prefix ARD_ "Arduino" -> called from Dezyne code
 */
#include <arduino.h>
#include "DezyneGlue.h"

#ifndef DZN_TINY
#error "Define DZN_TINY for building DZN code!"
#endif

//Dezyne includes
#include <assert.h>
#include <dzn/runtime.h>
#include <dzn/locator.h>

#include "Robot.h"

// debug print strings
#define MOTOR_LEFTFORWARD	(0)
#define MOTOR_RIGHTFORWARD	(1)
#define MOTOR_LEFTBACKWARD	(2)
#define MOTOR_RIGHTBACKWARD	(3)
#define MOTOR_LEFTSTOP		(4)
#define MOTOR_RIGHTSTOP		(5)
#define MOTOR_SLEEP			(6)

// Callback functions to call functions in C++ classes
//void (*myDigitalWrite)(uint8_t, uint8_t);
void (*mySerialPrint)(const int);
void (*ARD_Focus)(int);
void (*ARD_SetLeftMotor)(int);
void (*ARD_SetRightMotor)(int);

// Dezyne global variables
runtime dezyne_runtime;
Robot robot;
locator dezyne_locator;
bool SwitchLeftRight;

void LeftForward()
{
	mySerialPrint(MOTOR_LEFTFORWARD);
	ARD_SetLeftMotor(130);
}

void LeftBackward()
{
	mySerialPrint(MOTOR_LEFTBACKWARD);
	ARD_SetLeftMotor(-90);
}

void LeftStop()
{
	mySerialPrint(MOTOR_LEFTSTOP);
	ARD_SetLeftMotor(0);
}

void RightForward()
{
	mySerialPrint(MOTOR_RIGHTFORWARD);
	ARD_SetRightMotor(130);
}

void RightBackward()
{
	mySerialPrint(MOTOR_RIGHTBACKWARD);
	ARD_SetRightMotor(-90);
}

void RightStop()
{
	mySerialPrint(MOTOR_RIGHTSTOP);
	ARD_SetRightMotor(0);
}

void Sleep()
{
	mySerialPrint(MOTOR_SLEEP);
	delay(1000);
}

void lookStraight()
{
	ARD_Focus (SERVOMID);
}
void lookLeft()
{
	ARD_Focus (SERVOMID+SERVORANGE);
}
void lookRight()
{
	ARD_Focus (SERVOMID-SERVORANGE);
}

//void DG_setup(void (*DigitalWrite)(uint8_t, uint8_t),
void DG_setup(
		      void (*SerialPrint)(const int),
			  void (*ServoFocus) (int),
			  void (*SetLeftMotor) (int),
			  void (*SetRightMotor) (int))

{
//	myDigitalWrite = DigitalWrite;
	mySerialPrint = SerialPrint;
	ARD_Focus = ServoFocus;
	ARD_SetLeftMotor = SetLeftMotor;
	ARD_SetRightMotor = SetRightMotor;

	// Dezyne run time initialization
	runtime_init(&dezyne_runtime);
	locator_init(&dezyne_locator, &dezyne_runtime);
	Robot_init(&robot, &dezyne_locator);

	robot.motionController.motorLeft->in.forward = LeftForward;
	robot.motionController.motorLeft->in.backward = LeftBackward;
	robot.motionController.motorLeft->in.stop = LeftStop;

	robot.motionController.motorRight->in.forward = RightForward;
	robot.motionController.motorRight->in.backward = RightBackward;
	robot.motionController.motorRight->in.stop = RightStop;

	robot.motionController.motorLeft->in.sleep = Sleep;
	robot.motionController.motorRight->in.sleep = Sleep;

	robot.wallDetector.wdServo->in.lookLeft = lookLeft;
	robot.wallDetector.wdServo->in.lookRight = lookRight;
	robot.wallDetector.wdServo->in.lookStraight = lookStraight;

	robot.robotControl->in.start(robot.robotControl);
}

void DG_Danger() {
	robot.wallDetected->out.danger(robot.wallDetected);
}

void DG_NoDanger() {
	robot.wallDetected->out.noDanger(robot.wallDetected);
}

void DG_RobotStraight() {
	robot.lineDetected->out.Straight(robot.lineDetected);
}

void DG_RobotRight() {
	robot.lineDetected->out.Right(robot.lineDetected);
}

void DG_RobotLeft() {
	robot.lineDetected->out.Left(robot.lineDetected);
}

//
// for testing and debugging of motors only
//
void forward()
{
	ARD_SetLeftMotor(100);
	ARD_SetRightMotor(100);
}

void backward()
{
	ARD_SetLeftMotor(-100);
	ARD_SetRightMotor(-100);
}

void stop()
{
	ARD_SetLeftMotor(0);
	ARD_SetRightMotor(0);
}

void left()
{
	ARD_SetLeftMotor(-100);
	ARD_SetRightMotor(140);
}

void right()
{
	ARD_SetLeftMotor(140);
	ARD_SetRightMotor(-100);
}


