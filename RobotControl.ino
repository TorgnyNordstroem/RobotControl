#include <Servo.h>


const int PinStepperDir[3] ={22, 28, 34};
const int PinStepperStep[3] = {23, 29, 35};
const int PinServos[2] = {2, 3};
const int PinManInput[10] = {44, 45, 46, 47, 48, 49, 50, 51, 52, 53};

volatile int RealAnglesTarget[5] = {0, 0, 0, 0, 0}; //The angle target: x, y, z, a, c

volatile int StepCoordinatesTarget[3] = {0, 0, 0}; //Stepper target in steps from 0 point
volatile int StepCoordinatesIs[3] = {0, 0, 0}; //Stepper position in steps from 0 point
volatile int StepCoordinatesAdd[3] = {1, 1, 1}; //What to add when turning a stepper (x, y, z)

Servo ServoAngle; //Makes 'ServoAngle' an instance of Servo
Servo ServoClaw; //Makes 'ServoClaw' an instance of Servo

void setup() {
  Setup();
}

void loop() {
  CtrlUserManual();
  CtrlMotor();
  CtrlServo();
}
