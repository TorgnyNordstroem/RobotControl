#include <os48.h> //The only file you have to include

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

int StepSpeedKonst = 15; //Constant for calculating stepper speed in MotCtrl
int MilliPerSecond = 1000; //Microseconds in a second
int StepCoordinatesIs[5] = {10, 10, 10, 10, 10};
int StepCoordinatesShould[5] = {10, 10, 10, 10, 10};
int CoordDiff[3] = {0, 0, 0};

int StepperPinsDir[3] = {32, 36, 40}; //Stepper direction pins (x, y, z)
int StepperPinsStep[3] = {33, 37, 41}; //Stepper step pins (x, y, z)
int StepperStepPerSecond[3] = {500, 500, 500};
int StepperTime[3];

int i;
//data_string : Communication string; Build: speed/x/y/z/a/b;     or Coordinates: time/x/y/z/a/b

void setup() {
  Serial.begin(9600);
  Serial.println("Creating tasks...");
  task1 = scheduler->createTask(&Main, 60); //Creates a task associated to the 'Main' function with 60 bytes of stack memory.
  task2 = scheduler->createTask(&PosCalc, 60); //Creates a task associated to the 'PosCalc' (PositionCalculation of Stepper) function with 60 bytes of stack memory.

  Serial.println("Setting IO-Ports...");
  for(i=0; i<=3; i++) //Set pinmode of stepper motor pins (Dir, Step)
  {
    pinMode(StepperPinsDir[i], OUTPUT);
    pinMode(StepperPinsStep[i], OUTPUT);
  }

  Serial.println("Starting...");
  
  StepperTime[0] = ((MilliPerSecond / StepperStepPerSecond[0]) / 2);
  StepperTime[1] = ((MilliPerSecond / StepperStepPerSecond[1]) / 2);
  StepperTime[2] = ((MilliPerSecond / StepperStepPerSecond[2]) / 2);
  
  scheduler->start(); //Starts the scheduler. At this point you enter in a multi tasking context.

  //...
  //Nothing will be executed here
}

void Main()
{
  for(;;)
  {    
    OS48_ATOMIC_BLOCK //Ensures that the code between braces won't be interrupted.
    {
      Serial.println("I am task 1");
    }
    MotPulseY();

    //task() is a function returing the instance of the current running task. 
    task()->sleep(1000); //The current task will sleep during 1s.
  }
}

void PosCalc()
{
  for(;;)
  {    
    OS48_ATOMIC_BLOCK //Ensures that the code between braces won't be interrupted.
    {
      Serial.println("Test");
    }
    //task() is a function returing the instance of the current running task. 
    task()->sleep(300); //The current task will sleep during 1s.
  }
}

void CooChg()
{
  task()->suspend();
}

void Comm()
{ 
  task()->suspend();
}

void MotCtrl()
{
  task()->suspend();
}

void MotPulseX()
{
  for(;;)
  {
    while(StepCoordinatesShould[0] > StepCoordinatesIs[0])
    {
      digitalWrite(StepperPinsDir[0], HIGH);
      digitalWrite(StepperPinsStep[0], HIGH);
      task()->sleep(StepperTime[0]);
      digitalWrite(StepperPinsStep[0], LOW);
      task()->sleep(StepperTime[0]);

      ++StepCoordinatesIs[0];
    }
    while(StepCoordinatesShould[0] < StepCoordinatesIs[0])
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
    while(StepCoordinatesShould[1] > StepCoordinatesIs[1])
    {
      digitalWrite(StepperPinsDir[1], HIGH);
      digitalWrite(StepperPinsStep[1], HIGH);
      task()->sleep(StepperTime[1]);
      digitalWrite(StepperPinsStep[1], LOW);
      task()->sleep(StepperTime[1]);

      ++StepCoordinatesIs[1];
    }
    while(StepCoordinatesShould[1] < StepCoordinatesIs[1])
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
  for(;;)
  {
    while(StepCoordinatesShould[2] > StepCoordinatesIs[2])
    {
      digitalWrite(StepperPinsDir[2], HIGH);
      digitalWrite(StepperPinsStep[2], HIGH);
      task()->sleep(StepperTime[2]);
      digitalWrite(StepperPinsStep[2], LOW);
      task()->sleep(StepperTime[2]);

      ++StepCoordinatesIs[2];
    }
    while(StepCoordinatesShould[2] < StepCoordinatesIs[2])
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

void loop() {} //Useless now but you have to let it defined.
