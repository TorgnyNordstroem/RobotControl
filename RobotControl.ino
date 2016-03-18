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


int StartComplete = 0;
const int MilliPerSecond = 1000;

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

  Serial.begin(9600);
  
  Serial.println("Setting IO-Ports...");
  for (i = 0; i <= 3; i++)
  {
    pinMode(PinInterrupt[i], INPUT);
  }
  for (i = 0; i <= 3; i++)
  {
    pinMode(PinStepperDir[i], OUTPUT);
  }
  for (i = 0; i <= 3; i++)
  {
    pinMode(PinStepperStep[i], OUTPUT);
  }
  for (i = 0; i <= 2; i++)
  {
    pinMode(PinServoClaw[i], OUTPUT);
  }

  Serial.println("Creating tasks...");
  task1 = scheduler -> createTask(&MotCtrlX, 100, PrHigh);
  task2 = scheduler -> createTask(&MotCtrlY, 100, PrHigh);
  task3 = scheduler -> createTask(&MotCtrlZ, 100, PrHigh);
  task4 = scheduler -> createTask(&MotDir, 100, PrLow);
  task5 = scheduler -> createTask(&StepCooChg, 100, PrLow);
  
  scheduler -> setStackOverflowFnc(&fncSO);
  scheduler -> setSchedulingPolicy(SchPolicyIntelligent);

  Serial.println("Calculating initial stepper settings...");
  
  Serial.println("Attaching servos");
  ServoAngle.attach(PinServoClaw[0]);
  ServoClaw.attach(PinServoClaw[1]);

  Serial.println("Attaching interrupts...");
  attachInterrupt(digitalPinToInterrupt(PinInterrupt[0]), ResetCoorX, FALLING); //ResetCoor0 is executed when InterruptPins[0] receives a falling edge signal
  

  Serial.println("Starting...");
  delay(50);
  
        Serial.println(millis());
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
      StepCoordinatesTarget[0] = 100;
      StepCoordinatesTarget[1] = 100;
      StepCoordinatesTarget[2] = 100;
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




void SetServo()
{
  ServoAngle.write(RealAnglesTarget[3]);
  ServoClaw.write(RealAnglesTarget[4]);
}


void MotDir()
{
  for(;;)
  {
    if (StepCoordinatesTarget[0] < StepCoordinatesIs[0])
    {
      digitalWrite(PinStepperDir[0], LOW);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[0] = -1;
      }
    }
    else
    {
      digitalWrite(PinStepperDir[0], HIGH);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[0] = 1;
      }
    }
    if (StepCoordinatesTarget[1] < StepCoordinatesIs[1])
    {
      digitalWrite(PinStepperDir[1], LOW);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[1] = -1;
      }
    }
    else
    {
      digitalWrite(PinStepperDir[1], HIGH);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[1] = 1;
      }
    }
    if (StepCoordinatesTarget[2] < StepCoordinatesIs[2])
    {
      digitalWrite(PinStepperDir[2], LOW);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[2] = -1;
      }
    }
    else
    {
      digitalWrite(PinStepperDir[2], HIGH);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[2] = 1;
      }
    }
    task() -> sleep(10);
  }
}

void MotCtrlX()
{
  for(;;)
  {
    while(StepCoordinatesTarget[0] != StepCoordinatesIs[0])
    {
      digitalWrite(PinStepperStep[0], HIGH);
      delay(2);
      digitalWrite(PinStepperStep[0], LOW);
      delay(10);
      
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesIs[0] += StepCoordinatesAdd[0];
      }
      Serial.print("X Is: ");
      Serial.println(StepCoordinatesIs[0]);
    }
    task() -> sleep(50);
  }
}

void MotCtrlY()
{
  for(;;)
  {
    while(StepCoordinatesTarget[1] != StepCoordinatesIs[1])
    {
      digitalWrite(PinStepperStep[1], HIGH);
      delay(2);
      digitalWrite(PinStepperStep[1], LOW);
      delay(10);
      
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesIs[1] += StepCoordinatesAdd[1];
      }
      Serial.print("Y Is: ");
      Serial.println(StepCoordinatesIs[1]);
    }
    task() -> sleep(50);
  }
}

void MotCtrlZ()
{
  for(;;)
  {
    while(StepCoordinatesTarget[2] != StepCoordinatesIs[2])
    {
      digitalWrite(PinStepperStep[2], HIGH);
      delay(2);
      digitalWrite(PinStepperStep[2], LOW);
      delay(10);
      
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesIs[2] += StepCoordinatesAdd[2];
      }
      Serial.print("Z Is: ");
      Serial.println(StepCoordinatesIs[2]);
    }
    task() -> sleep(50);
  }
}




//Interrupts are below this point

//Resets all coordinates of the x-axis
void ResetCoorX()
{
  StepCoordinatesIs[0] = 0;
  StepCoordinatesTarget[0] = 0;
  RealAnglesTarget[0] = 0;
}


//function responsible at memory overflow 
void fncSO()
{
  Serial.println("Stack overflow!");
  Serial.print("ID of the task affected: ");
  Serial.println(task()->getId());
  Serial.print("Free stack size: ");
  Serial.println(task()->getUserFreeStackSize()); 
  Serial.flush();
}


void loop() {} //Useless now but you have to let it defined.
