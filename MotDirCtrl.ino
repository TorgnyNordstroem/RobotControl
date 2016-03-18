// Controls all Motor directions
void MotDir()
{
  // Run forever
  for(;;)
  {
    // X-axis: If Target is less then current position, set DIR-pin to LOW
    // and make sure that MotCtrlX() can count in the correct direction.
    if (StepCoordinatesTarget[0] < StepCoordinatesIs[0])
    {
      digitalWrite(PinStepperDir[0], LOW);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[0] = -1;
      }
    }
    else
    {
      digitalWrite(PinStepperDir[0], HIGH);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[0] = 1;
      }
    }
    // Y-axis
    if (StepCoordinatesTarget[1] < StepCoordinatesIs[1])
    {
      digitalWrite(PinStepperDir[1], LOW);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[1] = -1;
      }
    }
    else
    {
      digitalWrite(PinStepperDir[1], HIGH);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[1] = 1;
      }
    }
    // Z-axis
    if (StepCoordinatesTarget[2] < StepCoordinatesIs[2])
    {
      digitalWrite(PinStepperDir[2], LOW);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[2] = -1;
      }
    }
    else
    {
      digitalWrite(PinStepperDir[2], HIGH);
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesAdd[2] = 1;
      }
    }
    task() -> sleep(10);
  }
}

