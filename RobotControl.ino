#include <Servo.h>


const int PinStepperDir[3] ={22, 28, 34};
const int PinStepperStep[3] = {23, 29, 35};
const int PinServos[2] = {2, 3};
const int PinManInput[10] = {44, 45, 46, 47, 48, 49, 50, 51, 52, 53};
const int PinSpeed[3][3] =
  {
    //Mode 0, Mode 1, Mode 2
    // x-axis
    {24, 25, 26},
    // y-axis
    {30, 31, 32},
    // z-axis
    {36, 37, 38}
  };

int RealAnglesTarget[5] = {0, 0, 0, 0, 0}; //The angle target: x, y, z, a, c

int StepCoordinatesTarget[3] = {0, 0, 0}; //Stepper target in steps from 0 point
int StepCoordinatesIs[3] = {0, 0, 0}; //Stepper position in steps from 0 point

int AbsDiff;

int Speed[3] = {0, 0, 0};
int SpeedSteps[3] = {0, 0, 0};
/* Array used to determin speed of motors
 *  {Mode 0, Mode 1, Mode 2, Multiplier}
 *  Mode referes to the speed controlers on the stepper chip
 */
const int SpeedArray[6][4] = 
  {
    // 1/32 Step
    {1, 0, 1, 1},
    // 1/16 Step
    {0, 0, 1, 2},
    // 1/8 Step
    {1, 1, 0, 4},
    // 1/4 Step
    {0, 1, 0, 8},
    // 1/2 Step
    {1, 0, 0, 16},
    // 1 Step
    {0, 0, 0, 32}
  };

Servo ServoAngle; //Makes 'ServoAngle' an instance of Servo
Servo ServoClaw; //Makes 'ServoClaw' an instance of Servo

void setup() {
  Setup();
}

void loop() {
  if (StepCoordinatesIs[0] >= 2000) 
    {
      Serial.println("Set to 0");
      StepCoordinatesTarget[0] = 0;
      RealAnglesTarget[3] = 60;
      RealAnglesTarget[4] = 60;
    }
  
  if (StepCoordinatesIs[0] == 0) 
  {
    Serial.println("Set to 2000");
    StepCoordinatesTarget[0] = 2000;
      RealAnglesTarget[3] = 150;
      RealAnglesTarget[4] = 150;
  }

Serial.println(StepCoordinatesIs[0]);
  CtrlUserManual();
  CtrlSpeed();
  CtrlMotor();
  CtrlServo();

  delay(50);
}
