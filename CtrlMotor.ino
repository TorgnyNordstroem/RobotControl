void CtrlMotor()
{
  if (StepCoordinatesTarget[0] < StepCoordinatesIs[0])
  {
    digitalWrite(PinStepperDir[0], LOW);
    digitalWrite(PinStepperStep[0], HIGH);
    --StepCoordinatesIs[0];
    digitalWrite(PinStepperStep[0], LOW);
  }
  else if (StepCoordinatesTarget[0] > StepCoordinatesIs[0])
  {
    digitalWrite(PinStepperDir[0], HIGH);
    digitalWrite(PinStepperStep[0], HIGH);
    ++StepCoordinatesIs[0];
    digitalWrite(PinStepperStep[0], LOW);
  }

  if (StepCoordinatesTarget[1] < StepCoordinatesIs[1])
  {
    digitalWrite(PinStepperDir[1], LOW);
    digitalWrite(PinStepperStep[1], HIGH);
    --StepCoordinatesIs[1];
    digitalWrite(PinStepperStep[1], LOW);
  }
  else if (StepCoordinatesTarget[1] > StepCoordinatesIs[1])
  {
    digitalWrite(PinStepperDir[1], HIGH);
    digitalWrite(PinStepperStep[1], HIGH);
    ++StepCoordinatesIs[1];
    digitalWrite(PinStepperStep[1], LOW);
  }

  if (StepCoordinatesTarget[2] < StepCoordinatesIs[2])
  {
    digitalWrite(PinStepperDir[2], LOW);
    digitalWrite(PinStepperStep[2], HIGH);
    --StepCoordinatesIs[2];
    digitalWrite(PinStepperStep[2], LOW);
  }
  else if (StepCoordinatesTarget[2] > StepCoordinatesIs[2])
  {
    digitalWrite(PinStepperDir[2], HIGH);
    digitalWrite(PinStepperStep[2], HIGH);
    ++StepCoordinatesIs[2];
    digitalWrite(PinStepperStep[2], LOW);
  }
}

