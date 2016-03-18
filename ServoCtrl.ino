// Controls servo angles
void ServoCtrl()
{
  ServoAngle.write(RealAnglesTarget[3]);
  ServoClaw.write(RealAnglesTarget[4]);
  task() -> sleep(5);
}
