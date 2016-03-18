void ModeContinous()
{
  // Vary CycleTime
}

void ModeP2P()
{
  CycleTime = 10;
}

void ModeStartUp()
{
  ServoAngle.write(AnglesTarget[3]);
  ServoClaw.write(AnglesTarget[4]);
  while (digitalRead(PinSense[2]) == LOW)
  {
    StepsTarget[2]--;
    CtrlSpeed();
    CtrlMotor();
    delay(3);
  }
  Serial.println("Y");
  while (digitalRead(PinSense[1]) == LOW)
  {
    StepsTarget[1]--;
    CtrlSpeed();
    CtrlMotor();
    delay(3);
  }
  Serial.println("X");

  AnglesTarget[0] = -65;
  AnglesTarget[1] = 65;

  do
  {
    ConvAngleStep();
    StepsTarget[2] = StepsIs[2];
    CtrlMotor();
    delay(3);
  } while (StepsTarget[0] != StepsIs[0]);
  Serial.println("A");

  StepsTarget[1] = StepsIs[1];
  
  while (digitalRead(PinSense[0]) == LOW)
  {
    StepsTarget[0]--;
    CtrlMotor();
    delay(3);
  }
  Serial.println("Z");

  for (int i = 0; i < 3; i++)
  {
    CoordsTarget[i] = StartPosTarget[i];
    StepsIs[i] = StartPosAngles[i] / 0.05625;
  }
}

