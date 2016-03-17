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
Task* task6 = NULL;


int StartComplete = 0;
int MilliPerSecond = 1000;

int PinInterrupt[3] = {19, 20, 21};
int PinStepperSet[2] = {26, 27};
int PinStepperDir[3] = {30, 34, 38};
int PinStepperStep[3] = {31, 35, 39};
int PinServoClaw[2] = {42, 43};

int StepperStepTotal = 200; //Number of full steps
int StepperMicrosteps = 16; //Microstepping multiplier 
int StepperStepPerSecond[3] = {500, 500, 500}; //max. amount of steps per second
int StepperStepAngle[3]; //Angle in ° per step
int StepperStepTime[3]; //Time inbetween each step

int CoordinatesTarget[3] = {0, 0, 0};

int RealAnglesTarget[5] = {0, 0, 0, 0, 0}; //The angle target: x, y, z, a, c

int StepCoordinatesTarget[3] = {0, 0, 0}; //Stepper target in steps from 0 point
int StepCoordinatesIs[3] = {0, 0, 0}; //Stepper position in steps from 0 point

Servo ServoAngle;
Servo ServoClaw;

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
  for (i = 0; i <= 2; i++)
  {
    pinMode(PinStepperSet[i], OUTPUT);
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
  task1 = scheduler -> createTask(&SetServo, 10);
  task2 = scheduler -> createTask(&MotCtrlX, 50);
  task3 = scheduler -> createTask(&MotCtrlY, 50);
  task4 = scheduler -> createTask(&MotCtrlZ, 50);
  task5 = scheduler -> createTask(&StepperPosCalc, 50);

  Serial.println("Calculating initial stepper settings...");
  
  Serial.println("Attaching servos");
  ServoAngle.attach(42);
  ServoClaw.attach(46);

  Serial.println("Attaching interrupts...");
  attachInterrupt(digitalPinToInterrupt(PinInterrupt[0]), ResetCoorX, FALLING); //ResetCoor0 is executed when InterruptPins[0] receives a falling edge signal
  attachInterrupt(digitalPinToInterrupt(PinInterrupt[1]), ResetCoorY, FALLING); //ResetCoor1 is executed when InterruptPins[1] receives a falling edge signal
  attachInterrupt(digitalPinToInterrupt(PinInterrupt[2]), ResetCoorZ, FALLING); //ResetCoor2 is executed when InterruptPins[2] receives a falling edge signal
  

  Serial.println("Starting...");
  
  scheduler->start(); //Starts the scheduler. At this point you enter in a multi tasking context.

  //...
  //Nothing will be executed here
}


//Sets servo positions based on RealAnglesTarget
void SetServo()
{
  for(;;)
  {
    ServoAngle.write(RealAnglesTarget[4]); //Controls PWM output using "Servo" library
    ServoClaw.write(RealAnglesTarget[5]); //Controls PWM output using "Servo" library
  }
}

void StepperPosCalc()
{
  for(;;)
  {    
    StepCoordinatesTarget[0] = RealAnglesTarget[0]/StepperStepAngle[0];
    StepCoordinatesTarget[1] = RealAnglesTarget[1]/StepperStepAngle[1];
    StepCoordinatesTarget[2] = RealAnglesTarget[2]/StepperStepAngle[2];
  }
}

void MotCtrlX()
{
  for(;;)
  {
    while(StepCoordinatesTarget[0] > StepCoordinatesIs[0])
    {
      digitalWrite(PinStepperDir[0], HIGH);
      MotPulseX();

      ++StepCoordinatesIs[0];
    }
    while(StepCoordinatesTarget[0] < StepCoordinatesIs[0])
    {
      digitalWrite(PinStepperDir[0], LOW);
      MotPulseX();

      --StepCoordinatesIs[0];
    }
    task()->sleep(50);
  }
}

void MotCtrlY()
{
  for(;;)
  {
    while(StepCoordinatesTarget[1] > StepCoordinatesIs[1])
    {
      digitalWrite(PinStepperDir[1], HIGH);
      MotPulseY();

      ++StepCoordinatesIs[1];
    }
    while(StepCoordinatesTarget[1] < StepCoordinatesIs[1])
    {
      digitalWrite(PinStepperDir[1], LOW);
      MotPulseY();

      --StepCoordinatesIs[1];
    }
    task()->sleep(50);
  }
}

void MotCtrlZ()
{
  for(;;)
  {
    while(StepCoordinatesTarget[2] > StepCoordinatesIs[2])
    {
      digitalWrite(PinStepperDir[2], HIGH);
      MotPulseZ();

      ++StepCoordinatesIs[2];
    }
    while(StepCoordinatesTarget[2] < StepCoordinatesIs[2])
    {
      digitalWrite(PinStepperDir[2], LOW);
      MotPulseZ();

      --StepCoordinatesIs[2];
    }
    task()->sleep(50);
  }
}







void AngleCalc()
{
  for(;;)
  {
    C = sqrt(pow(CoordinatesTarget[0], 2) + pow(CoordinatesTarget[1], 2));
    beta = -(180/PI)*acos((pow(CoordinatesTarget[0], 2)+pow(CoordinatesTarget[1], 2)-(pow(Lx, 2)+pow(Ly, 2)))/(2*Lx*Ly));
    gamma = (180/PI)*acos(CoordinatesTarget[0]/C);
    delta = (180/PI)*acos((pow(Lx, 2)-pow(Ly, 2)+pow(C, 2))/(2*Lx*C));
    alpha = gamma + delta;
  }
}





//From this point on, only non-threads exists

// Pulses x-axis Stepper once
void MotPulseX() 
{
  digitalWrite(PinStepperStep[0], HIGH);
  task()->sleep(StepperStepTime[0]);
  digitalWrite(PinStepperStep[0], LOW);
  task()->sleep(StepperStepTime[0]);
}

// Pulses y-axis Stepper once
void MotPulseY()
{
  digitalWrite(PinStepperStep[1], HIGH);
  task()->sleep(StepperStepTime[1]);
  digitalWrite(PinStepperStep[1], LOW);
  task()->sleep(StepperStepTime[1]);
}

// Pulses z-axis Stepper once
void MotPulseZ()
{
  digitalWrite(PinStepperStep[2], HIGH);
  task()->sleep(StepperStepTime[2]);
  digitalWrite(PinStepperStep[2], LOW);
  task()->sleep(StepperStepTime[2]);
}


//Interrupts are below this point

//Resets all coordinates of the x-axis
void ResetCoorX()
{
  StepCoordinatesIs[0] = 0;
  StepCoordinatesTarget[0] = 0;
  RealAnglesTarget[0] = 0;
}

//Resets all coordinates of the y-axis
void ResetCoorY()
{
  StepCoordinatesIs[1] = 0;
  StepCoordinatesTarget[1] = 0;
  RealAnglesTarget[1] = 0;
}

//Resets all coordinates of the z-axis
void ResetCoorZ()
{
  StepCoordinatesIs[2] = 0;
  StepCoordinatesTarget[2] = 0;
  RealAnglesTarget[2] = 0;
}


void loop() {} //Useless now but you have to let it defined.
