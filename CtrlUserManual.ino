void CtrlUserManual()
{
  if(digitalRead(PinManInput[0]) == LOW) ++StepCoordinatesTarget[0];
  if(digitalRead(PinManInput[1]) == LOW) --StepCoordinatesTarget[0];
  
  if(digitalRead(PinManInput[2]) == LOW) ++StepCoordinatesTarget[1];
  if(digitalRead(PinManInput[3]) == LOW) --StepCoordinatesTarget[1];
    
  if(digitalRead(PinManInput[4]) == LOW) ++StepCoordinatesTarget[2];
  if(digitalRead(PinManInput[5]) == LOW) --StepCoordinatesTarget[2];
    
  if(digitalRead(PinManInput[6]) == LOW) ++RealAnglesTarget[3];
  if(digitalRead(PinManInput[7]) == LOW) --RealAnglesTarget[3];
  
  if(digitalRead(PinManInput[8]) == LOW) ++RealAnglesTarget[4];
  if(digitalRead(PinManInput[9]) == LOW) --RealAnglesTarget[4];
}

