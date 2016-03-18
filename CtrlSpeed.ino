void CtrlSpeed()
{
  for (int a = 0; a < 3; a++)
  {
    AbsDiff = abs(max(StepCoordinatesTarget[a], StepCoordinatesIs[a])) - abs(min(StepCoordinatesTarget[a], StepCoordinatesIs[a]));
    AbsDiff = abs(AbsDiff);

    if (AbsDiff > 5 * (SpeedArray[Speed[a] + 1][3]) && Speed[a] < 3 && SpeedSteps[a] >= 10)
    {
      Speed[a]++;
      SpeedSteps[a] = 0;
    }

    if (AbsDiff < 4 * (SpeedArray[Speed[a]][3]) && Speed[a] > 0)
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
