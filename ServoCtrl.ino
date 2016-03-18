// Controls servo angles
void SetServo()
{
  ServoAngle.write(RealAnglesTarget[3]);
  ServoClaw.write(RealAnglesTarget[4]);
  task() -> sleep(5);
}
