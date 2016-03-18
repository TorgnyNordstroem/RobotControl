void CooChgUserX()
{
  for(;;)
  {
    if(digitalRead(PinManInput[0]) == LOW)
    {
      OS48_ATOMIC_BLOCK
      {
      ++StepCoordinatesTarget[0];
      }
    }
    if(digitalRead(PinManInput[1]) == LOW)
    {
      OS48_ATOMIC_BLOCK
      {
      --StepCoordinatesTarget[0];
      }
    }
    
    task() -> sleep(5);
  }
}

void CooChgUserY()
{
  for(;;)
  {
    if(digitalRead(PinManInput[2]) == LOW)
    {
      OS48_ATOMIC_BLOCK
      {
      ++StepCoordinatesTarget[1];
      }
    }
    if(digitalRead(PinManInput[3]) == LOW)
    {
      OS48_ATOMIC_BLOCK
      {
      --StepCoordinatesTarget[1];
      }
    }
    
    task() -> sleep(5);
  }
}

void CooChgUserZ()
{
  for(;;)
  {
    if(digitalRead(PinManInput[4]) == LOW)
    {
      OS48_ATOMIC_BLOCK
      {
      ++StepCoordinatesTarget[2];
      }
    }
    if(digitalRead(PinManInput[5]) == LOW)
    {
      OS48_ATOMIC_BLOCK
      {
      --StepCoordinatesTarget[2];
      }
    }
    
    task() -> sleep(5);
  }
}
