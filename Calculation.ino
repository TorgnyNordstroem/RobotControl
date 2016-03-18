void CalcAbsDiff()
{
  for (int i = 0; i < 3; i++)
  {
    AbsDiff[i] = abs(max(StepsTarget[i], StepsIs[i])) - abs(min(StepsTarget[i], StepsIs[i]));
    AbsDiff[i] = abs(AbsDiff[i]);
  }
}

void ConvAngleStep()
{
  for (int i = 0; i < 3; i++)
  {
    StepsTarget[i] = AnglesTarget[i] / 0.05625;
  }
}
