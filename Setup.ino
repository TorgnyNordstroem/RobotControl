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

  Serial.println("Moving to starting position");
  StartPos();
  
  delay(20);
}

// Movement to starting position
void StartPos()
{
  ServoAngle.write(AnglesTarget[3]);
  ServoClaw.write(AnglesTarget[4]);




  Serial.println("0");
  while (digitalRead(PinSense[2]) == LOW)
  {
    StepsTarget[2]--;
    CtrlSpeed();
    CtrlMotor();
    delay(3);
  }
    
  Serial.println("1");
  while (digitalRead(PinSense[1]) == LOW)
  {
    StepsTarget[1]--;
    CtrlSpeed();
    CtrlMotor();
    delay(3);
  }
  
  Serial.println("2");
  AnglesTarget[0] = -65;
  AnglesTarget[1] = 65;
  StepsTarget[0] = 10;

  do
  {
    ConvAngleStep();
    StepsTarget[2] = StepsIs[2];
    CtrlMotor();
    delay(3);
    //Serial.println(AnglesIs[0]);
  } while (StepsTarget[0] != StepsIs[0] && StepsTarget[1] != StepsIs[1]);

  Serial.println("3");

  while (digitalRead(PinSense[0]) == LOW)
  {
    StepsTarget[0]++;
    CtrlMotor();
  }

  for (int i = 0; i < 3; i++)
  {
    CoordsTarget[i] = StartPosTarget[i];
    StepsIs[i] = StartPosAngles[i]/0.05625;
  }
  
  ConvCoordsToAngle();
  ConvAngleStep();
}
