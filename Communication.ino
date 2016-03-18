void Communication()
{
  //update 11.02.2016: ich versuche bestimmte daten zu empfangen vom computer, sofern ich diese bekommen habe sende ich zureck und behandle mich als connected

  while (connected == false)
  {

    /*if (ones == false) { Serial.println("LOOP:");  }*/
    

    if (udpServer.available())
    {

      char buffer[UDP_READ_BUFFER_SIZE];
      int n = udpServer.readData(buffer, UDP_READ_BUFFER_SIZE);
      Serial.println(n);// n contains # of bytes read into buffer
      for (int i = 0; i < n; i++)
      {
        Serial.print(buffer[i]);

      }
      int i = 0;
      while (checked == false)
      {
        if (buffer[i] != retpw[i])
        {
          checked = true;
        }
        i++;
        if (i > 3)
        {
          checked = true;
        }
      }
      if (buffer[0] == retpw[0] && buffer[1] == retpw[1] && buffer[2] == retpw[2] && buffer[3] == retpw[3])
      {
        Adafruit_CC3000_Client udpclient = cc3000.connectUDP(IPAd, port);
        connected = true;
        Serial.println(" Connected");
        udpclient.write(buf, size);
      }
      else
      {
        Serial.println("Data not matching!");
      }
    }
    /*if (ones == false)
      {
      Serial.println("Again:");
      ones == true;
      }*/

  }


  /* add main program code here */
  if (udpServer.available())
  {
    STR_COORDS dta_str;
    char buffer[UDP_READ_BUFFER_SIZE];
    int n = udpServer.readData(buffer, UDP_READ_BUFFER_SIZE);  // n contains # of bytes read into buffer
    Serial.print("n: "); Serial.println(n);

    for (int i = 0; i < n; ++i) {
      uint8_t c = buffer[i];
      Serial.print("c: "); Serial.println(c);
      // ... Do whatever you want with 'c' here ...
      if (buffer[0] == '2')
      {
        for (int i = 2; i < UDP_READ_BUFFER_SIZE; i++)
        {
          int zehler = 0;
          if (buffer[i] != ',' && zehler == 0)
          {
            dta_str.x = dta_str.x + buffer[i];
          }
          else if (buffer[i] != ',' && zehler == 1)
          {
            dta_str.y = dta_str.y + buffer[i];
          }
          else if (buffer[i] != ',' && zehler == 2)
          {
            dta_str.z = dta_str.z + buffer[i];
          }
          else if (buffer[i] != ',' && zehler == 3)
          {
            dta_str.claw_tilt = dta_str.claw_tilt + buffer[i];
          }
          else if (buffer[i] != ',' && zehler == 4)
          {
            dta_str.claw_width = dta_str.claw_width + buffer[i];
          }
          else if (buffer[i] == ';')
          {
            zehler++;
            i = UDP_READ_BUFFER_SIZE;
          }
          else
          {
            zehler++;
          }
        }
        Serial.println(dta_str.x);
        Serial.println(dta_str.y);
        Serial.println(dta_str.z);
        Serial.println(dta_str.claw_tilt);
        Serial.println(dta_str.claw_width);

        data.x = dta_str.x.toInt();
        data.y = dta_str.y.toInt();
        data.z = dta_str.z.toInt();
        
        Serial.println(data.x);
        Serial.println(data.y);
        Serial.println(data.z);
        data.claw_tilt = dta_str.claw_tilt.toInt();
        data.claw_width = dta_str.claw_width.toInt();
      }
    }
    for (int i = 0; i < n; i++)
    {
      Serial.print(buffer[i]);
    }

    //hier kennte ihr code stehen mit data als coordinatenhalter
  }
}




void displayDriverMode(void)
{
#ifdef CC3000_TINY_DRIVER
  Serial.println(F("CC3000 is configure in 'Tiny' mode"));
#else
  Serial.print(F("RX Buffer : "));
  Serial.print(CC3000_RX_BUFFER_SIZE);
  Serial.println(F(" bytes"));
  Serial.print(F("TX Buffer : "));
  Serial.print(CC3000_TX_BUFFER_SIZE);
  Serial.println(F(" bytes"));
#endif
}

/**************************************************************************/
/*!
  @brief  Tries to read the CC3000's internal firmware patch ID
*/
/**************************************************************************/
uint16_t checkFirmwareVersion(void)
{
  uint8_t major, minor;
  uint16_t version;

#ifndef CC3000_TINY_DRIVER
  if (!cc3000.getFirmwareVersion(&major, &minor))
  {
    Serial.println(F("Unable to retrieve the firmware version!\r\n"));
    version = 0;
  }
  else
  {
    Serial.print(F("Firmware V. : "));
    Serial.print(major); Serial.print(F(".")); Serial.println(minor);
    version = major; version <<= 8; version |= minor;
  }
#endif
  return version;
}

/**************************************************************************/
/*!
  @brief  Tries to read the 6-byte MAC address of the CC3000 module
*/
/**************************************************************************/
void displayMACAddress(void)
{
  uint8_t macAddress[6];

  if (!cc3000.getMacAddress(macAddress))
  {
    Serial.println(F("Unable to retrieve MAC Address!\r\n"));
  }
  else
  {
    Serial.print(F("MAC Address : "));
    cc3000.printHex((byte*)&macAddress, 6);
  }
}


/**************************************************************************/
/*!
  @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if (!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

/**************************************************************************/
/*!
  @brief  Begins an SSID scan and prints out all the visible networks
*/
/**************************************************************************/

void listSSIDResults(void)
{
  uint32_t index;
  uint8_t valid, rssi, sec;
  char ssidname[33];

  if (!cc3000.startSSIDscan(&index)) {
    Serial.println(F("SSID scan failed!"));
    return;
  }

  Serial.print(F("Networks found: ")); Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);

    Serial.print(F("SSID Name    : ")); Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}
