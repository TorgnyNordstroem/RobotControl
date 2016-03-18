#include <Servo.h>
//beta 195°

const int PinSense[3] = {27, 37, 47};
const int PinEnable[3] = {28, 38, 48};
const int PinStepperStep[3] = {30, 40, 50};
const int PinStepperDir[3] = {32, 42, 52};
const int PinServos[2] = {2, 3};
const int PinSpeed[3][3] =
{
  //Mode 0, Mode 1, Mode 2
  // z-axis
  {29, 31, 33},
  // x-axis
  {39, 41, 43},
  // y-axis
  {49, 51, 53}
};

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

int StartPosAngles[3] = {90, 140, -90}; // Axis Angles
int StartPosTarget[5] = {200, 100, 0, 90, 90}; // 0, 1, 2: Axis coordinates; 3, 4: Servo angles

int CoordsTarget[3] = {0, 0, 0};

int AnglesTarget[5] = {0, 0, 0, 90, 90}; //The angle target: z, x, y, a, c
int AnglesIs[3] = {0, 0, 0};

int StepsTarget[3] = {0, 0, 0}; //Stepper target in steps from 0 point
int StepsIs[3] = {0, 0, 0}; //Stepper position in steps from 0 point

int AbsDiff;

int Speed[3] = {0, 0, 0};
int SpeedSteps[3] = {0, 0, 0};

Servo ServoAngle; //Makes 'ServoAngle' an instance of Servo
Servo ServoClaw; //Makes 'ServoClaw' an instance of Servo

//Values for Conversion
const int e1 = 100; //In mm
const int e2 = 80; //In mm
const int e3 = 100; //In mm
const int e4 = 111; //In mm
int Rabs;
int Gamma;
int Phi;
int Rx;
int Rz;

void setup() {
  Setup();
}

void loop() {

  
  if (CoordsTarget[0] == 180 && CoordsTarget[1] == 200 && StepsTarget[0] == StepsIs[0] && StepsTarget[1] == StepsIs[1] && StepsTarget[2] == StepsIs[2])
  {
    //Serial.println("One");
    CoordsTarget[0] = 270;
    CoordsTarget[1] = 50;
  }
  else if (CoordsTarget[0] == 270 && CoordsTarget[1] == 50 && StepsTarget[0] == StepsIs[0] && StepsTarget[1] == StepsIs[1] && StepsTarget[2] == StepsIs[2])
  {
    //Serial.println("Two");
    CoordsTarget[0] = 180;
    CoordsTarget[1] = 200;
  }

  if (CoordsTarget[0] == 200 && CoordsTarget[1] == 100 && StepsTarget[0] == StepsIs[0] && StepsTarget[1] == StepsIs[1] && StepsTarget[2] == StepsIs[2])
  {
    CoordsTarget[0] = 180;
    CoordsTarget[1] = 200;
  }
  /* Debug info
  Serial.println("");
  Serial.println("");
  Serial.println("Z");
  Serial.println(CoordsTarget[0]);
  Serial.println("Target");
  Serial.println(StepsTarget[0]);
  Serial.println(StepsTarget[1]);
  Serial.println("Is");
  Serial.println(StepsIs[0]);
  Serial.println(StepsIs[1]);
  Serial.println("Speed");
  Serial.println(Speed[0]);
  Serial.println(Speed[1]);
  */
  
  ConvCoordsToAngle();
  ConvAngleStep();
  CtrlSpeed();
  CtrlMotor();
  CtrlServo();

  delay(10);
}
