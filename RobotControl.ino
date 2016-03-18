//Robot arm Arduino controller
//Programmed by T. Nordström
//Version 0.2.0

//This programm controlles an robot arm using an Arduino Mega
//Please note that it requires the correct hardware in order to function.
//Hardware can be built using the parts documented in the TGM diploma project "Roboter Arm"

#include <Servo.h>//Provides support for servo PWM control
#include <os48.h> //The only file you have to include
#include <math.h> //Provides mathematical functions

using namespace os48; //This line is necessary

//Scheduler is a singleton, you don't have to create an instance because only one can exist.
Scheduler* scheduler = Scheduler::get(); //Get the instance

//Declare the task pointers as global vars to use them in the task functions.
Task* task1 = NULL; 
Task* task2 = NULL;
Task* task3 = NULL;
Task* task4 = NULL;
Task* task5 = NULL;


const int MilliPerSecond = 1000;
const int MaxCooSpeed = 50; // mm/s

//Used Pins
const int PinInterrupt[3] = {19, 20, 21}; //Pins for interrupt
const int PinStepperDir[3] = {30, 36, 42}; //Pins for stepper direction
const int PinStepperStep[3] = {31, 37, 43}; //Pins for stepper signals
const int PinServoClaw[2] = {48, 49}; //Pins for the claw

const int StepperStepTotal = 200; //Number of full steps
const int StepperMicrosteps = 16; //Microstepping multiplier 
volatile int StepperStepPerSecond[3] = {500, 500, 500}; //max. amount of steps per second
volatile int StepperStepAngle[3]; //Angle in ° per step
volatile int StepperStepTime[3]; //Time inbetween each step

volatile int SpeedTime;
volatile int CooTime;

volatile int CoordinatesTarget[2] = {0, 0}; //Coordinates (x, y) in mm (input from user)

volatile int RealAnglesTarget[5] = {0, 0, 0, 80, 80}; //The angle target: x, y, z, a, c

volatile int StepCoordinatesTarget[3] = {0, 0, 0}; //Stepper target in steps from 0 point
volatile int StepCoordinatesIs[3] = {0, 0, 0}; //Stepper position in steps from 0 point
volatile int StepCoordinatesAdd[3] = {1, 1, 1}; //What to add when turning a stepper (x, y, z)

Servo ServoAngle; //Makes 'ServoAngle' an instance of Servo
Servo ServoClaw; //Makes 'ServoClaw' an instance of Servo

int alpha;
int beta;
int gamma;
int delta;
int C;
int Lx;
int Ly;

int i; //variable used in loops
//data_string : Communication string; Build: g/speed/x/y/z/a/b;     or Coordinates: time/x/y/z/a/b


void setup() {

  Setup();

  Serial.println("Starting...");
  delay(50);
  
  scheduler->start(); //Starts the scheduler. At this point you enter in a multi tasking context.

  //...
  //Nothing will be executed here
}


void StepCooChg()
{
  for(;;)
  {

    OS48_ATOMIC_BLOCK
    {
      StepCoordinatesTarget[0] = 0;
      StepCoordinatesTarget[1] = 00;
      StepCoordinatesTarget[2] = 0;
    }
    Serial.println(StepCoordinatesTarget[0]);
    Serial.println(StepCoordinatesTarget[1]);
    Serial.println(StepCoordinatesTarget[2]);
    task() -> sleep(10000);
    
    OS48_ATOMIC_BLOCK
    {
      StepCoordinatesTarget[0] = 0;
      StepCoordinatesTarget[1] = 0;
    }
    Serial.println(StepCoordinatesTarget[0]);
    Serial.println(StepCoordinatesTarget[1]);
    Serial.println(StepCoordinatesTarget[2]);
    task() -> sleep(10000);
  }
}


void CooChg()
{
  for(;;)
  {
    OS48_ATOMIC_BLOCK
    {
      for(i = 0; i <= 3; i++)
      {
        CooTime = MaxCooSpeed * (SpeedTime / 100);
      }
    }

    
  }
}




void loop() {} //Useless now but you have to let it defined.
