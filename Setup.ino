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

  Serial.println("Initializing servos");
  ServoAngle.attach(PinServos[0]);
  ServoClaw.attach(PinServos[1]);


  // Program part: computer/Arduino (Alexander Seiler)
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

  /* Optional: Update the Mac Address to a known value */
  /*
    uint8_t macAddress[6] = { 0x08, 0x00, 0x28, 0x01, 0x79, 0xB7 };
    if (!cc3000.setMacAddress(macAddress))
    {
    Serial.println(F("Failed trying to update the MAC address"));
    while(1);
    }
  */

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
  /*
    #ifndef CC3000_TINY_DRIVER
    /* Try looking up www.adafruit.com *
    uint32_t ip = 0;
    Serial.print(F("www.adafruit.com -> "));
    /*
    while (ip == 0) {
      if (!cc3000.getHostByName("www.adafruit.com", &ip)) {
        Serial.println(F("Couldn't resolve!"));
      }
      delay(500);
    }
    cc3000.printIPdotsRev(ip);

    /* Do a quick ping test on adafruit.com *
    Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");
    uint8_t replies = cc3000.ping(ip, 5);
    Serial.print(replies); Serial.println(F(" replies"));
    if (replies)
      Serial.println(F("Ping successful!"));
    #endif

  */
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

  Serial.println("Calibrating");
  ModeStartUp();

  data.x = 140;
  data.y = 90;
  data.z = 162;
  data.claw_width = 90;
  data.claw_tilt = 90;
  time_received = millis();

  Serial.println("Setup finished");

  delay(20);
}

