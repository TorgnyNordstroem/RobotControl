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

  for (int i = 2; i > 0; i--)
  {
    while (digitalRead(PinSense[i]) == LOW)
    {
      StepsTarget[i]--;
      CtrlSpeed();
      CtrlMotor();
      delay(3);
    }
  }
  
  AnglesTarget[0] = -65;
  AnglesTarget[1] = 65;

  do
  {
    ConvAngleStep();
    StepsTarget[2] = StepsIs[2];
    CtrlMotor();
    delay(3);
  } while (StepsTarget[0] != StepsIs[0] && StepsTarget[1] != StepsIs[1]);

  while (digitalRead(PinSense[0]) == LOW)
  {
    StepsTarget[0]++;
    CtrlMotor();
  }

  for (int i = 0; i < 3; i++)
  {
    CoordsTarget[i] = StartPosTarget[i];
    StepsIs[i] = StartPosAngles[i] / 0.05625;
  }
}

