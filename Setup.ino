void Setup()
{
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
}

