//Interrupts are below this point

//Resets all coordinates of the x-axis
void ResetCoorX()
{
  StepCoordinatesIs[0] = 0;
  StepCoordinatesTarget[0] = 0;
  RealAnglesTarget[0] = 0;
}


//function responsible at memory overflow 
void fncSO()
{
  Serial.println("Stack overflow!");
  Serial.print("ID of the task affected: ");
  Serial.println(task()->getId());
  Serial.print("Free stack size: ");
  Serial.println(task()->getUserFreeStackSize()); 
  Serial.flush();
}
