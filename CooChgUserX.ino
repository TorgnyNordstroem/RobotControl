void CooChgUserX()
{
  for(;;)
  {
    if(digitalRead(PinInput[0]) == LOW)
    {
      OS48_ATOMIC_BLOCK
      {
      ++StepCoordinatesTarget[0];
      }
    }
    if(digitalRead(PinInput[1]) == LOW)
    {
      OS48_ATOMIC_BLOCK
      {
      --StepCoordinatesTarget[0];
      }
    }
    
    task() -> sleep(5);
  }
}
