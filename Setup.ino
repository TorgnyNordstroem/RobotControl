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
  for (int i = 0; i < 3; i++)
  {
    pinMode(PinSense[i], INPUT_PULLUP);
  }

  Serial.println("Enabling Steppers");
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(PinEnable[i], HIGH);
  }

  Serial.println("Initializing servos");
  ServoAngle.attach(PinServos[0]);
  ServoClaw.attach(PinServos[1]);

  Serial.println("Setup finished");

  Serial.println("Calibrating");
  ModeStartUp();
  
  delay(20);
}

