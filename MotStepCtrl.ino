
// Controls motor in X-axis
void MotCtrlX()
{
  // Run forever
  for(;;)
  {
    // Run while Motor is not on target position
    while(StepCoordinatesTarget[0] != StepCoordinatesIs[0])
    {
      // Pulse once on stepper control X-axis
      digitalWrite(PinStepperStep[0], HIGH);
      delay(2);
      digitalWrite(PinStepperStep[0], LOW);
      delay(5);

      // Blocks all but following read/write
      OS48_ATOMIC_BLOCK
      {
        // Count position change
        StepCoordinatesIs[0] += StepCoordinatesAdd[0];
      }

      // Print current X-axis position to serial
      Serial.print("X Is: ");
      Serial.println(StepCoordinatesIs[0]);
    }

    //sleep temporarly (probability to be needed quickly is low)
    task() -> sleep(50);
  }
}

// Controls motor in Y-axis
// Detailed description in MotCtrlX(), replace X with Y
void MotCtrlY()
{
  for(;;)
  {
    while(StepCoordinatesTarget[1] != StepCoordinatesIs[1])
    {
      digitalWrite(PinStepperStep[1], HIGH);
      delay(2);
      digitalWrite(PinStepperStep[1], LOW);
      delay(5);
      
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesIs[1] += StepCoordinatesAdd[1];
      }
      Serial.print("Y Is: ");
      Serial.println(StepCoordinatesIs[1]);
    }
    task() -> sleep(50);
  }
}

//Controls motor in Z-axis
// Detailed description in MotCtrlX(), replace X with Z
void MotCtrlZ()
{
  for(;;)
  {
    while(StepCoordinatesTarget[2] != StepCoordinatesIs[2])
    {
      digitalWrite(PinStepperStep[2], HIGH);
      delay(1);
      digitalWrite(PinStepperStep[2], LOW);
      delay(2);
      
      OS48_ATOMIC_BLOCK
      {
        StepCoordinatesIs[2] += StepCoordinatesAdd[2];
      }
      Serial.print("Z Is: ");
      Serial.println(StepCoordinatesIs[2]);
    }
    task() -> sleep(50);
  }
}

