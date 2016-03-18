void CtrlMotor()
{
  for (int i = 0; i <= 2; i++)
  {
    if (StepCoordinatesTarget[i] < StepCoordinatesIs[i])
    {
      digitalWrite(PinStepperDir[i], LOW);
      digitalWrite(PinStepperStep[i], HIGH);
      StepCoordinatesIs[i] -= SpeedArray[Speed[i]][3];
      ++SpeedSteps[i];
      digitalWrite(PinStepperStep[i], LOW);
    }
    else if (StepCoordinatesTarget[i] > StepCoordinatesIs[i])
    {
      digitalWrite(PinStepperDir[i], HIGH);
      digitalWrite(PinStepperStep[i], HIGH);
      StepCoordinatesIs[i] += SpeedArray[Speed[i]][3];
      ++SpeedSteps[i];
      digitalWrite(PinStepperStep[i], LOW);
    }
    else
    {
      SpeedSteps[i] = 0;
    }
  }
}
