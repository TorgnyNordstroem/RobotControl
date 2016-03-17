#include <Servo.h>
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

int PinInterrupt[3] = {19, 20, 21};
int PinStepperSet[2] = {26, 27};
int PinStepperDir[3] = {30, 34, 38};
int PinStepperStep[3] = {31, 35, 39};
int PinServoClaw[2] = {42, 43};




int StartComplete = 0;
int InterruptPins[3] = {16, 18, 20};

Servo ServoAngle;
Servo ServoClaw;

int MilliPerSecond = 1000; //Microseconds in a second
volatile int RealCoordinatesTarget[5] = {0, 0, 0, 0, 0}; //Real-world Coordinates in {mm; mm; °; °; mm}
int RealAngleTarget[5]; //Real-world angles in °
volatile int StepCoordinatesIs[3] = {0, 0, 0}; //Stepper coordinates in steps from origin.
volatile int StepCoordinatesTarget[5] = {0, 0, 0, 0, 0}; //Stepper & Servo target coordinates in {stp, stp, stp, °, °}

int StepperStepTotal = 200;
int StepperMicrosteps = 16;
int StepperStepAngle;
int StepperPinsDir[3] = {32, 36, 40}; //Stepper direction pins (x, y, z)
int StepperPinsStep[3] = {33, 37, 41}; //Stepper step pins (x, y, z)
int StepperStepPerSecond[3] = {500, 500, 500};
int StepperTime[3];

int alpha;
int beta;
int gamma;
int delta;
int C;
int Lx;
int Ly;

int i;
//data_string : Communication string; Build: g/speed/x/y/z/a/b;     or Coordinates: time/x/y/z/a/b

void setup() {
  
  Serial.begin(9600);
  Serial.println("Creating tasks...");
  task1 = scheduler->createTask(&Main, 60); //Creates a task associated to the 'Main' function with 60 bytes of stack memory.
  task2 = scheduler->createTask(&PosCalc, 60); //Creates a task associated to the 'PosCalc' (PositionCalculation of Stepper) function with 60 bytes of stack memory.
  task3 = scheduler->createTask(&MotPulseX, 60); //Creates a task associated to the 'MotPulseX' (Stepper controller x-axis) function with 60 bytes of stack memory.
  task4 = scheduler->createTask(&MotPulseY, 60); //Creates a task associated to the 'MotPulseY' (Stepper controller y-axis) function with 60 bytes of stack memory.
  task5 = scheduler->createTask(&MotPulseZ, 60); //Creates a task associated to the 'MotPulseZ' (Stepper controller z-axis) function with 60 bytes of stack memory.
  task6 = scheduler->createTask(&AngleCalc, 60);

  Serial.println("Calculating initial stepper settings...");
  for (i = 0; i <= 3; i++);
  {
    StepperTime[i] = ((MilliPerSecond / StepperStepPerSecond[i]) / 2);
  }
  StepperStepAngle = 360/(StepperStepTotal*StepperMicrosteps);

  Serial.println("Setting IO-Ports...");
  for(i = 0; i <= 3; i++) //Set pinmode of stepper motor pins (Dir, Step) to OUTPUT
  {
    pinMode(StepperPinsDir[i], OUTPUT);
    pinMode(StepperPinsStep[i], OUTPUT);
  }
  for(i = 0; i <= 3; i++) //Set pinmode of positioning buttons to INPUT
  {
    pinMode(InterruptPins[i], INPUT);
  }

  Serial.println("Attaching servos");
  ServoAngle.attach(42);
  ServoClaw.attach(46);

  Serial.println("Attaching interrupts...");
  attachInterrupt(digitalPinToInterrupt(InterruptPins[0]), ResetCoor0, FALLING); //ResetCoor0 is executed when InterruptPins[0] receives a falling edge signal
  attachInterrupt(digitalPinToInterrupt(InterruptPins[1]), ResetCoor1, FALLING); //ResetCoor1 is executed when InterruptPins[1] receives a falling edge signal
  attachInterrupt(digitalPinToInterrupt(InterruptPins[2]), ResetCoor2, FALLING); //ResetCoor2 is executed when InterruptPins[2] receives a falling edge signal
  

  Serial.println("Starting...");
  
  scheduler->start(); //Starts the scheduler. At this point you enter in a multi tasking context.

  //...
  //Nothing will be executed here
}

void Main()
{
}

void AngleCalc()
{
  for(;;)
  {
    C = sqrt(pow(RealCoordinatesTarget[0], 2) + pow(RealCoordinatesTarget[1], 2));
    beta = -(180/PI)*acos((pow(RealCoordinatesTarget[0], 2)+pow(RealCoordinatesTarget[1], 2)-(pow(Lx, 2)+pow(Ly, 2)))/(2*Lx*Ly));
    gamma = (180/PI)*acos(RealCoordinatesTarget[0]/C);
    delta = (180/PI)*acos((pow(Lx, 2)-pow(Ly, 2)+pow(C, 2))/(2*Lx*C));
    alpha = gamma + delta;
  }
}

void PosCalc()
{
  for(;;)
  {    
    StepCoordinatesTarget[0] = RealAngleTarget[0]/StepperStepAngle;
    StepCoordinatesTarget[1] = RealAngleTarget[1]/StepperStepAngle;
    StepCoordinatesTarget[2] = RealAngleTarget[2]/StepperStepAngle;
    ServoAngle.write(RealAngleTarget[4]);
  }
}

void Comm()
{ 
  task()->suspend();
}

void MotPulseX()
{ 
  for(;;)
  {
    while(StepCoordinatesTarget[0] > StepCoordinatesIs[0])
    {
      digitalWrite(StepperPinsDir[0], HIGH);
      digitalWrite(StepperPinsStep[0], HIGH);
      task()->sleep(StepperTime[0]);
      digitalWrite(StepperPinsStep[0], LOW);
      task()->sleep(StepperTime[0]);

      ++StepCoordinatesIs[0];
    }
    while(StepCoordinatesTarget[0] < StepCoordinatesIs[0])
    {
      digitalWrite(StepperPinsDir[0], LOW);
      digitalWrite(StepperPinsStep[0], HIGH);
      task()->sleep(StepperTime[0]);
      digitalWrite(StepperPinsStep[0], LOW);
      task()->sleep(StepperTime[0]);

      --StepCoordinatesIs[0];
    }
    task()->sleep(50);
  }
}

void MotPulseY()
{
  for(;;)
  {
    while(StepCoordinatesTarget[1] > StepCoordinatesIs[1])
    {
      digitalWrite(StepperPinsDir[1], HIGH);
      digitalWrite(StepperPinsStep[1], HIGH);
      task()->sleep(StepperTime[1]);
      digitalWrite(StepperPinsStep[1], LOW);
      task()->sleep(StepperTime[1]);

      ++StepCoordinatesIs[1];
    }
    while(StepCoordinatesTarget[1] < StepCoordinatesIs[1])
    {
      digitalWrite(StepperPinsDir[1], LOW);
      digitalWrite(StepperPinsStep[1], HIGH);
      task()->sleep(StepperTime[1]);
      digitalWrite(StepperPinsStep[1], LOW);
      task()->sleep(StepperTime[1]);

      --StepCoordinatesIs[1];
    }
    task()->sleep(50);
  }
}

void MotPulseZ()
{
  while (StartComplete = 0)
  {
    task()->sleep(1000);
  }
  
  for(;;)
  {
    while(StepCoordinatesTarget[2] > StepCoordinatesIs[2])
    {
      digitalWrite(StepperPinsDir[2], HIGH);
      digitalWrite(StepperPinsStep[2], HIGH);
      task()->sleep(StepperTime[2]);
      digitalWrite(StepperPinsStep[2], LOW);
      task()->sleep(StepperTime[2]);

      ++StepCoordinatesIs[2];
    }
    while(StepCoordinatesTarget[2] < StepCoordinatesIs[2])
    {
      digitalWrite(StepperPinsDir[2], LOW);
      digitalWrite(StepperPinsStep[2], HIGH);
      task()->sleep(StepperTime[2]);
      digitalWrite(StepperPinsStep[2], LOW);
      task()->sleep(StepperTime[2]);

      --StepCoordinatesIs[2];
    }
    task()->sleep(50);
  }
}

void ResetCoor0()
{
  StepCoordinatesIs[0] = 0;
  StepCoordinatesTarget[0] = 0;
  RealCoordinatesTarget[0] = 0;
}

void ResetCoor1()
{
  StepCoordinatesIs[1] = 0;
  StepCoordinatesTarget[1] = 0;
  RealCoordinatesTarget[1] = 0;
}

void ResetCoor2()
{
  StepCoordinatesIs[2] = 0;
  StepCoordinatesTarget[2] = 0;
  RealCoordinatesTarget[2] = 0;
}


void loop() {} //Useless now but you have to let it defined.
