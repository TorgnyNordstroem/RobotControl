void Setup()
{
  Serial.begin(9600);

  Serial.println("Setup starting");

  Serial.println("Initializing IO-Ports");
  for (int i = 0; i < 3; i++)
  {
    pinMode(PinStepperDir[i], OUTPUT);
    pinMode(PinStepperStep[i], OUTPUT);
  }
  for (int i = 0; i < 3; i++)
  {
    pinMode(PinSpeed[i][0], OUTPUT);
    pinMode(PinSpeed[i][1], OUTPUT);
    pinMode(PinSpeed[i][2], OUTPUT);
  }
  for (int i = 0; i < 3; i++)
  {
    pinMode(PinSense[i], INPUT_PULLUP);
  }
  for (int i = 0; i < 2; i++)
  {
    pinMode(PinLED[i], OUTPUT);
  }
  digitalWrite(PinLED[0], HIGH);
  digitalWrite(PinLED[1], HIGH);
  delay(100);
  digitalWrite(PinLED[0], LOW);
  digitalWrite(PinLED[1], LOW);

  Serial.println("Initializing servos");
  ServoAngle.attach(PinServos[0]);
  ServoClaw.attach(PinServos[1]);
  CtrlServo();

  Serial.println(F("Hello, CC3000!\n"));

  displayDriverMode();
  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);

  /* Initialise the module */
  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    while (1);
  }

  digitalWrite(PinLED[0], HIGH);
  digitalWrite(PinLED[1], HIGH);
  delay(100);
  digitalWrite(PinLED[0], LOW);
  digitalWrite(PinLED[1], LOW);

  uint16_t firmware = checkFirmwareVersion();
  if (firmware < 0x113) {
    Serial.println(F("Wrong firmware version!"));
    for (;;);
  }

  displayMACAddress();

  /* Optional: Get the SSID list (not available in 'tiny' mode) */
#ifndef CC3000_TINY_DRIVER
  listSSIDResults();
#endif

  digitalWrite(PinLED[0], HIGH);
  digitalWrite(PinLED[1], HIGH);
  delay(100);
  digitalWrite(PinLED[0], LOW);
  digitalWrite(PinLED[1], LOW);
  /* Delete any old connection data on the module */
  Serial.println(F("\nDeleting old connection profiles"));
  if (!cc3000.deleteProfiles()) {
    Serial.println(F("Failed!"));
    while (1);
  }


  char *ssid = WLAN_SSID;             /* Max 32 chars */
  Serial.print(F("\nAttempting to connect to ")); Serial.println(ssid);


  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while (1);
  }

  Serial.println(F("Connected!"));

  digitalWrite(PinLED[0], HIGH);
  digitalWrite(PinLED[1], HIGH);
  delay(100);
  digitalWrite(PinLED[0], LOW);
  digitalWrite(PinLED[1], LOW);

  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }

  /* Display the IP address DNS, Gateway, etc. */
  while (!displayConnectionDetails()) {
    delay(1000);
  }

  for (int i = 0; i < lenght; i++)
  {
    buf[i] = sending[i];
  }

  Adafruit_CC3000_Client udpclient = cc3000.connectUDP(IPAd, port);


  udpclient.write(buf, size);

  for (int i = 0; i < lenght; i++)
  {
    buf[i] = pw[i];
  }
  /* add setup code here */
  udpServer.begin();

  digitalWrite(PinLED[0], HIGH);
  digitalWrite(PinLED[1], HIGH);
  delay(100);
  digitalWrite(PinLED[0], LOW);
  digitalWrite(PinLED[1], LOW);

  data.alpha = 162.33;
  data.beta = 139.66;
  data.y = 90;
  data.claw_tilt = 90;
  data.claw_width = 90;

  Serial.println("Calibrating");
  ModeStartUp();

  time_received = millis();

  ModeP2P();
  ConvAngleStepP2P();
  Serial.println("Setup finished");

  delay(20);
}

