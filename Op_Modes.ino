void ModeKey()
{
  if ((StepsTarget[0] + StepsTarget[1]) >= 5770)
  {
    switch (data2.z) 
    {
      case 1:
        StepsTarget[0]--;
        break;
      case 2:
        break;
      default:
        break;
    }
    switch (data2.x) 
    {
      case 1:
        StepsTarget[1]--;
        break;
      case 2:
        break;
      default:
        break;
    }
  }
  else if ((StepsTarget[0] + StepsTarget[1]) <= 4090)
  {
    switch (data2.z) 
    {
      case 1:
        break;
      case 2:
        StepsTarget[0]++;
        break;
      default:
        break;
    }
    switch (data2.x) 
    {
      case 1:
        break;
      case 2:
        StepsTarget[1]++;
        break;
      default:
        break;
    }
  }
  else
  {
    switch (data2.z) 
    {
      case 1:
        StepsTarget[0]--;
        break;
      case 2:
        StepsTarget[0]++;
        break;
      default:
        break;
    }
    switch (data2.x) 
    {
      case 1:
        StepsTarget[1]--;
        break;
      case 2:
        StepsTarget[1]++;
        break;
      default:
        break;
    }
  }

  switch (data2.y) 
  {
    case 1:
      StepsTarget[2]--;
      break;
    case 2:
      StepsTarget[2]++;
      break;
    default:
      break;
  }
  switch (data2.claw_tilt) 
  {
    case 1:
      AnglesTarget[3] += 0.2;
    case 2:
      AnglesTarget[3] -= 0.2;
    default:
      AnglesTarget[3] = constrain(AnglesTarget[3], 0, 190);
      break;
  }
  switch (data2.claw_width) 
  {
    case 1:
      AnglesTarget[4] += 0.2;
    case 2:
      AnglesTarget[4] -= 0.2;
    default:
      AnglesTarget[4] = constrain(AnglesTarget[4], 95, 175);
      break;
  }
}

void ModeP2P()
{
  AnglesTarget[0] = double(data.alpha);
  AnglesTarget[1] = double(data.beta);
  AnglesTarget[2] = double(data.y);
  //y = map(x, 1, 50, 50, 1);
  AnglesTarget[3] = double(data.claw_tilt) + 12.0;
  AnglesTarget[4] = map(double(data.claw_width), 0, 76, 175, 95);
}

void ModeStartUp()
{
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
    ConvAngleStepP2P();
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
    StepsIs[i] = StartPosAngles[i] / 0.05625;
  }
}

