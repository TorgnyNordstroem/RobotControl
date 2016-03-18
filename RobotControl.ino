#include <Servo.h>

int i = 0;
const int PinLED[2] = {0, 2};
const int PinSense[3] = {41, 33, 25};
const int PinStepperStep[3] = {43, 35, 27};
const int PinStepperDir[3] = {45, 37, 29};
const int PinServos[2] = {6, 7};
const int PinSpeed[3][3] =
{
  //Mode 0, Mode 1, Mode 2
  // z-axis
  {40, 42, 44},
  // x-axis
  {32, 34, 36},
  // y-axis
  {24, 26, 28},
};

/* Array used to determin speed of motors
    {Mode 0, Mode 1, Mode 2, Multiplier}
    Mode referes to the speed controlers on the stepper chip
*/
const int SpeedMax[3] = {2, 2, 2};
const int SpeedArray[6][4] =
{
  // 1/32 Step
  {HIGH, LOW, HIGH, 1},
  // 1/16 Step
  {LOW, LOW, HIGH, 2},
  // 1/8 Step
  {HIGH, HIGH, LOW, 4},
  // 1/4 Step
  {LOW, HIGH, LOW, 8},
  // 1/2 Step
  {HIGH, LOW, LOW, 16},
  // 1 Step
  {LOW, LOW, LOW, 32}
};

int StartPosAngles[3] = {90, 140, 0}; // Axis Angles

double AnglesTarget[5] = {162.33, 139.66, 90, 90, 90}; //The angle target: z, x, y, a, c

int StepsTarget[3] = {0, 0, 0}; //Stepper target in steps from 0 point
int StepsIs[3] = {0, 0, 0}; //Stepper position in steps from 0 point

int AbsDiff[3] = {0, 0, 0};

int Speed[3] = {0, 0, 0};
int SpeedSteps[3] = {0, 0, 0};

Servo ServoAngle; //Makes 'ServoAngle' an instance of Servo
Servo ServoClaw; //Makes 'ServoClaw' an instance of Servo

//Values for Conversion
const int e1 = 100; //In mm
const int e2 = 80; //In mm
const int e3 = 100; //In mm
const int e4 = 111; //In mm
int Rabs;
int Gamma;
int Phi;
int Rx;
int Rz;

int CycleTime = 7;



// Program part: computer/Arduino (Alexander Seiler)
#include <Adafruit_CC3000_Server.h>
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <ccspi.h>

#include <UDPServer.h>
#include <string.h>
#include "utility/debug.h"


#define UDP_READ_BUFFER_SIZE 30
#define LISTEN_PORT_UDP 11001
UDPServer udpServer(LISTEN_PORT_UDP);

char sending[] = "ROBOTIC-ARM";
char pw[] = "robotic_arm";
char retpw[] = "test";
const int lenght = 12;
bool connected = false;
bool checked = false;

struct coords
{
  double alpha;
  double beta;
  double y;
  double claw_tilt;
  double claw_width;
};

typedef struct coords Coords;

struct str_coords
{
  String x;
  String y;
  String z;
  String claw_tilt;
  String claw_width;
};

typedef struct str_coords STR_COORDS;

struct direction
{
  unsigned int x;
  unsigned int y;
  unsigned int z;
  unsigned int claw_tilt;
  unsigned int claw_width;
};

typedef struct direction DIRECTION;


Coords data;
DIRECTION data2;

int mode = 0;

size_t size = strlen(pw);
uint8_t buf[lenght];

#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER);

#define WLAN_SSID       "ROBOTIC_ARM"
#define WLAN_PASS       "robotest"
#define WLAN_SECURITY   WLAN_SEC_WPA2

IPAddress Empfangsadresse = IPAddress(192, 86, 43, 255);
uint32_t IPAd = cc3000.IP2U32(192, 168, 1, 255);
uint16_t port = 11000;

// Definitions end


void setup() {
  Setup();
}

void loop() {
  /*
    // Debug info
    Serial.println("");
    Serial.println("");
    Serial.println("Angles");
    Serial.println(AnglesTarget[0]);
    Serial.println(AnglesTarget[1]);
    Serial.println(AnglesTarget[2]);
    Serial.println("Target");
    Serial.println(StepsTarget[0]);
    Serial.println(StepsTarget[1]);
    Serial.println(StepsTarget[2]);
    Serial.println("Is");
    Serial.println(StepsIs[0]);
    Serial.println(StepsIs[1]);
    Serial.println(StepsIs[2]);/*
    Serial.println("Speed");
    Serial.println(Speed[0]);
    Serial.println(Speed[1]);
    Serial.println(Speed[2]);
    /*
    if ()
    {
    ModeContinous();
    }
    else
    {
    ModeP2P();
    }
  *//*
  while (i < 50)
  {
  digitalWrite(PinLED[0], HIGH);
  digitalWrite(PinLED[1], HIGH);
  i++;
  }
  while (i > 0)
  {
  digitalWrite(PinLED[0], LOW);
  digitalWrite(PinLED[1], LOW);
  i--;
  }*/
  
  Communication();
  
  if (mode = 0)
  {
    ModeP2P();
  }
  else if (mode = 1)
  {
    ModeKey();
  }
  
  ConvAngleStep();
  CalcAbsDiff();
  CtrlSpeed();
  CtrlMotor();
  CtrlServo();

  delay(CycleTime);
}

