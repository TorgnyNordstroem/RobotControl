void SensorCheck()
{
  for (int i = 0; i < 3; i++)
  {
    if (digitalRead(PinSense[i]) == HIGH && AbsDiff[i] <= 10)
    {
      StepsIs[i] = StartPosAngles[i] / 0.05625;
      StepsTarget[i] = StepsIs[0];
    }
    else if (digitalRead(PinSense[i]) == HIGH && AbsDiff[i] > 10)
    {
      StepsIs[i] = StartPosAngles[i] / 0.05625;
      StepsTarget[i] = StepsIs[0];
    }
  }
}

