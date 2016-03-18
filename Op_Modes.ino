void ModeKey()
{
  if ((StepsTarget[0] + StepsTarget[1]) >= 5770)
  {
    switch (data2.z) {
      case 1:
        StepsTarget[0]--;
        break;
      case 2:
        break;
      default:
        break;
    }
    switch (data2.x) {
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
    switch (data2.z) {
      case 1:
        break;
      case 2:
        StepsTarget[0]++;
        break;
      default:
        break;
    }
    switch (data2.x) {
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
    switch (data2.z) {
      case 1:
        StepsTarget[0]--;
        break;
      case 2:
        StepsTarget[0]++;
        break;
      default:
        break;
    }
    switch (data2.x) {
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

  switch (data2.y) {
    case 1:
      StepsTarget[2]--;
      break;
    case 2:
      StepsTarget[2]++;
      break;
    default:
      break;
  }
  
  if (data2.claw_tilt == 1 && AnglesTarget[3] < 190)
  {
    AnglesTarget[3] += 0.2;
  }
  else if (data2.claw_tilt == 2 && AnglesTarget[3] > 0)
  {
    AnglesTarget[3] -= 0.2;
  }

  if (data2.claw_width == 1 && AnglesTarget[4] < 165)
  {
    AnglesTarget[4] += 0.2;
  }
  else if (data2.claw_width == 2 && AnglesTarget[4] > 100)
  {
    AnglesTarget[4] -= 0.2;
  }
}

void ModeP2P()
{
  AnglesTarget[0] = double(data.alpha);
  AnglesTarget[1] = double(data.beta);
  AnglesTarget[2] = double(data.y);
  AnglesTarget[3] = double(data.claw_tilt) + 5.0;
  AnglesTarget[4] = double(data.claw_width);
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

