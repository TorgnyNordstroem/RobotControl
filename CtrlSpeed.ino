void CtrlSpeed()
{
  for (int a = 0; a < 3; a++)
  {
    if (AbsDiff[a] > 5 * (SpeedArray[Speed[a] + 1][3]) && Speed[a] < SpeedMax[a] && SpeedSteps[a] >= 20)
    {
      Speed[a]++;
      SpeedSteps[a] = 0;
    }

    if (AbsDiff[a] < 4 * (SpeedArray[Speed[a]][3]) && Speed[a] > 0)
    {
      Speed[a]--;
      SpeedSteps[a] = 0;
    }
    for (int b = 0; b < 3; b++)
    {
      digitalWrite(PinSpeed[a][b], SpeedArray[Speed[a]][b]);
    }
  }
}

