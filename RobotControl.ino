#include <Servo.h>


const int PinStepperDir[3] = {32, 42, 52};
const int PinStepperStep[3] = {30, 40, 50};
const int PinServos[2] = {2, 3};
const int PinEnable[3] = {28, 38, 48};
const int PinSpeed[3][3] =
{
  //Mode 0, Mode 1, Mode 2
  // x-axis
  {29, 31, 33},
  // y-axis
  {39, 41, 43},
  // z-axis
  {49, 51, 53}
};

int RealCoordsTarget[3] = {0, 0, 0};
int RealAnglesTarget[5] = {0, 0, 0, 90, 90}; //The angle target: x, y, z, a, c

int StepCoordinatesTarget[3] = {0, 0, 0}; //Stepper target in steps from 0 point
int StepCoordinatesIs[3] = {0, 0, 0}; //Stepper position in steps from 0 point

int AbsDiff;

int Speed[3] = {0, 0, 0};
int SpeedSteps[3] = {0, 0, 0};
/* Array used to determin speed of motors
    {Mode 0, Mode 1, Mode 2, Multiplier}
    Mode referes to the speed controlers on the stepper chip
*/
const int SpeedArray[6][4] =
{
  // 1/32 Step
  {HIGH, LOW, HIGH, 1},
  // 1/16 Step
  {LOW, LOW, HIGH, 2},
  // 1/8 Step
  {HIGH, HIGH, LOW, 4},
  // 1/4 Step
  {LOW, HIGH, LOW, 8},
  // 1/2 Step
  {HIGH, LOW, LOW, 16},
  // 1 Step
  {LOW, LOW, LOW, 32}
};

Servo ServoAngle; //Makes 'ServoAngle' an instance of Servo
Servo ServoClaw; //Makes 'ServoClaw' an instance of Servo

void setup() {
  Setup();
}

void loop() {
  if (StepCoordinatesIs[0] >= 750 && StepCoordinatesIs[1] <= -400 && StepCoordinatesIs[2] <= -250)
  {
    StepCoordinatesTarget[0] = 0;
    StepCoordinatesTarget[1] = 0;
    StepCoordinatesTarget[2] = 0;
    RealAnglesTarget[3] = 60;
    RealAnglesTarget[4] = 60;
  }

  if (StepCoordinatesIs[0] == 0 && StepCoordinatesIs[1] == 0 && StepCoordinatesIs[2] == 0)
  {
    StepCoordinatesTarget[0] = 750;
    StepCoordinatesTarget[1] = -400;
    StepCoordinatesTarget[2] = -250;
    RealAnglesTarget[3] = 150;
    RealAnglesTarget[4] = 150;
  }

  CtrlSpeed();
  CtrlMotor();
  CtrlServo();

  delay(10);
}
