void ConvCoordsToAngle()
{
  Rz = CoordsTarget[0] - e4;
  Rx = CoordsTarget[1] - e1;
  
  Rabs = sqrt(pow(Rx, 2) + pow(Rz, 2));

  Gamma = atan(Rx / Rz);
  Phi = acos((pow(e2, 2) + pow(Rabs, 2) - pow(e3, 2)) / (2*e2*Rabs)) * (180 / PI);
  
  AnglesTarget[0] = 90 + Gamma + Phi;
  AnglesTarget[1] = -acos((pow(e2, 2) + pow(e3, 2) - pow(Rabs, 2)) / (2*e2*e3)) * (180 / PI);
}

void ConvAngleStep()
{
  for (int i = 0; i < 3; i++)
  {
    StepsTarget[i] = AnglesTarget[i] / 0.05625;
  }
  
  for (int i = 0; i < 3; i++)
  {
    AnglesIs[i] = StepsIs[i] * 0.05625;
  }
}
