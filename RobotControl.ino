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
int StepTimeOn = 10; //Time step will be 'HIGH' in micro
int MicroPerSecond = 1000000; //Microseconds in a second

void setup() {
  Serial.begin(9600);
  Serial.println("Creating tasks...");

  task1 = scheduler->createTask(&Main, 60); //Creates a task associated to the 'Main' function with 60 bytes of stack memory.
  task2 = scheduler->createTask(&PosCalc, 60); //Creates a task associated to the 'PosCalc' (PositionCalculation of Stepper) function with 60 bytes of stack memory.
  task3 = scheduler->createTask(&CooChg, 60); //Creates a task associated to the 'CooChg' (CoordinateChanger) function with 60 bytes of stack memory.
  task4 = scheduler->createTask(&Comm, 60); //Creates a task associated to the 'Comm' (Communication) function with 60 bytes of stack memory.
  task5 = scheduler->createTask(&MotCtrl, 60); //Creates a task associated to the 'MotCtrl' (MotorControl) function with 60 bytes of stack memory.

  Serial.println("Starting...");
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

    //task() is a function returing the instance of the current running task. 
    task()->sleep(1000); //The current task will sleep during 1s.
  }
}

void PosCalc()
{
  task()->sleep(1000);  
}

void CooChg()
{
  task()->sleep(1000);  
}

void Comm()
{
  task()->sleep(1000);  
}

void MotCtrl()
{
  Message* MotPos = task()->waitNextMessage(PosCalc, MotPos);
  Serial.println((char*) MotPos->getBody());
  task()->sleep(1000);
}

void loop() {} //Useless now but you have to let it defined.
