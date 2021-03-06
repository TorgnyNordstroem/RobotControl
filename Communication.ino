//im setup am ende muss sein: und time_received=millis();

void Communication()
{
  while (connected == false)
  {
    digitalWrite(PinLED[0], HIGH);
    digitalWrite(PinLED[1], HIGH);

    if (udpServer.available())
    {

      char buffer[UDP_READ_BUFFER_SIZE];
      int n = udpServer.readData(buffer, UDP_READ_BUFFER_SIZE);
      //Serial.println(n);// n contains # of bytes read into buffer
      for (int i = 0; i < n; i++)
      {
        //Serial.print(buffer[i]);
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
      }//Computer_alive

      else
      {
        Serial.println("Data not matching!");
      }
    }
  }
  digitalWrite(PinLED[0], LOW);
  digitalWrite(PinLED[1], LOW);


  /* add main program code here */
  if (udpServer.available())
  {
    STR_COORDS dta_str;
    char buffer[UDP_READ_BUFFER_SIZE];
    byte bufferx[UDP_READ_BUFFER_SIZE];
    int n = udpServer.readData(buffer, UDP_READ_BUFFER_SIZE);  // n contains # of bytes read into buffer
    
    if (buffer[7] == 1 | buffer[7] == 2)
    {
      mode = buffer[7];
    }

    if (mode == 2 & buffer[7] != 'r')
    {
      if (buffer[0] < 0)
      {
        bufferx[0] = 256 + buffer[0];
      }
      else
      {
        bufferx[0] = buffer[0];
      }

      if (buffer[1] < 0)
      {
        bufferx[1] = 256 + buffer[1];
      }
      else
      {
        bufferx[1] = buffer[1];
      }

      if (buffer[2] < 0)
      {
        bufferx[2] = 256 + buffer[2];
      }
      else
      {
        bufferx[2] = buffer[2];
      }

      if (buffer[3] < 0)
      {
        bufferx[3] = 256 + buffer[3];
      }
      else
      {
        bufferx[3] = buffer[3];
      }

      if (buffer[4] < 0)
      {
        bufferx[4] = 256 + buffer[4];
      }
      else
      {
        bufferx[4] = buffer[4];
      }
      if (buffer[5] < 0)
      {
        bufferx[5] = 256 + buffer[5];
      }
      else
      {
        bufferx[5] = buffer[5];
      }
      if (buffer[6] < 0)
      {
        bufferx[6] = 256 + buffer[6];
      }
      else
      {
        bufferx[6] = buffer[6];
      }

      data.alpha = (bufferx[2] * 100 + bufferx[3]) / 100;
      data.beta = (bufferx[0] * 100 + bufferx[1]) / 100;
      data.y = bufferx[4];
      data.claw_width = bufferx[5];
      data.claw_tilt = bufferx[6];
    }
    else if (buffer[0] == 'C' && buffer[1] == 'o' && buffer[2] == 'm' && buffer[3] == 'p' && buffer[4] == 'u' && buffer[5] == 't' && buffer[6] == 'e' && buffer[7] == 'r' && buffer[8] == '_' && buffer[9] == 'a' && buffer[10] == 'l' && buffer[11] == 'i' && buffer[12] == 'v' && buffer[13] == 'e')
    {
      Serial.println("Alive_Data matching!");
      time_received = millis();
    }
    else if (mode == 1 & buffer[7] != 'r')
    {
      data2.x = buffer[0];
      data2.y = buffer[1];
      data2.z = buffer[2];
      data2.claw_width = buffer[3];
      data2.claw_tilt = buffer[4];
    }
  }

  //hier kennte ihr code stehen mit data als coordinatenhalter
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
