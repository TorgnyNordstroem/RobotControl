void Setup()
{
  Serial.begin(9600);

  Serial.println("Setup starting");

  Serial.println("Initializing IO-Ports");
  for (int i = 0; i < 3; i++)
  {
    pinMode(PinStepperDir[i], OUTPUT);
    pinMode(PinStepperStep[i], OUTPUT);
  }
  //for (int i = 0; i <= 10; i++)
  {
    //pinMode(PinManInput[i], INPUT_PULLUP);
  }
  for (int i = 0; i < 3; i++)
  {
    pinMode(PinSpeed[i][0], OUTPUT);
    pinMode(PinSpeed[i][1], OUTPUT);
    pinMode(PinSpeed[i][2], OUTPUT);
  }
  for (int i = 0; i < 3; i++)
  {
    pinMode(PinEnable[i], OUTPUT);
  }

  Serial.println("Enabling Steppers");
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(PinEnable[i], HIGH);
  }

  Serial.println("Initializing servos");
  ServoAngle.attach(PinServos[0]);
  ServoClaw.attach(PinServos[1]);

  Serial.println("Moving to starting positions");
  ServoAngle.write(AnglesTarget[3]);
  ServoClaw.write(AnglesTarget[4]);

  Serial.println("Setup finished");
  delay(20);
}
