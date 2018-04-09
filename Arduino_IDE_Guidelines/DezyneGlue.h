//
// Interface of Dezyne glue layer
//
//void DG_setup(void (*DigitalWrite)(uint8_t, uint8_t),
void DG_setup(
		      void (*SerialPrint)(const int),
			  void (*ServoFocus) (int),
			  void (*SetLeftMotor) (int),
			  void (*SetRightMotor) (int));

void DG_ObstacleScan();
void DG_SensorScan();

// events into Dezyne
void DG_RobotStraight();
void DG_RobotRight();
void DG_RobotLeft();

void DG_Danger();
void DG_NoDanger ();

// absolute boundaries for servo angle
#define SERVOMID  90
#define SERVORANGE 20



