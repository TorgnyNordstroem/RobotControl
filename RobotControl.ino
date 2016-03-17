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

void setup() {
  Serial.begin(9600);
  Serial.println("Creating tasks...");

  task1 = scheduler->createTask(&Main, 60); //Creates a task associated to the 'Main' function with 60 bytes of stack memory.
  task2 = scheduler->createTask(&func2, 60);
  task3 = scheduler->createTask(&func3, 60); //Creates a task associated to the 'Main' function with 60 bytes of stack memory.
  task4 = scheduler->createTask(&func4, 60);
  task5 = scheduler->createTask(&func5, 60); //Creates a task associated to the 'Main' function with 60 bytes of stack memory.
  task6 = scheduler->createTask(&func6, 60);

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

void func2()
{
  task()->sleep(500);  

  for(;;)
  {    
    OS48_ATOMIC_BLOCK
    {
      Serial.println("I am task 2");
    }
    task()->sleep(1000);    
  }
}

void func3()
{
  
}

void func4()
{
  
}

void func5()
{
  
}

void func6()
{
  
}

void loop() {} //Useless now but you have to let it defined.
