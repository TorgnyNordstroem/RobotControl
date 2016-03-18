void CtrlMotor()
{
  for (int i = 0; i < 3; i++)
  {
    if (StepsTarget[i] < StepsIs[i])
    {
      digitalWrite(PinStepperDir[i], LOW);
      digitalWrite(PinStepperStep[i], HIGH);
      StepsIs[i] -= SpeedArray[Speed[i]][3];
      ++SpeedSteps[i];
      digitalWrite(PinStepperStep[i], LOW);
    }
    else if (StepsTarget[i] > StepsIs[i])
    {
      digitalWrite(PinStepperDir[i], HIGH);
      digitalWrite(PinStepperStep[i], HIGH);
      StepsIs[i] += SpeedArray[Speed[i]][3];
      ++SpeedSteps[i];
      digitalWrite(PinStepperStep[i], LOW);
    }
    else
    {
      SpeedSteps[i] = 0;
    }
  }
}

