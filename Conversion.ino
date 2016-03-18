void ConvCoordsToAngle()
{
  CoordsTarget[0] = int(data.x);
  CoordsTarget[1] = int(data.y);
  CoordsTarget[2] = int(data.z);

  Serial.println(CoordsTarget[0]);
  Serial.println(CoordsTarget[1]);
  Serial.println(CoordsTarget[2]);
  Rz = CoordsTarget[0] - e4;
  Rx = CoordsTarget[1] - e1;
  
  Rabs = sqrt(pow(Rx, 2) + pow(Rz, 2));

  float Gamma = atan(Rx / Rz) * (180 / PI);
  float Phi = acos((pow(e2, 2) + pow(Rabs, 2) - pow(e3, 2)) / (2*e2*Rabs)) * (180 / PI);

  float Beta = acos((pow(e2, 2) + pow(e3, 2) - pow(Rabs, 2)) / (2*e2*e3)) * (180 / PI);
  
  AnglesTarget[0] = 90 + Gamma + Phi;
  AnglesTarget[1] = (360 - (Beta + AnglesTarget[0]));
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

