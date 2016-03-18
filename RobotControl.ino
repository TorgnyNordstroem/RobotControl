#include <Servo.h>

const int PinSense[3] = {37, 27, 47};
const int PinStepperStep[3] = {50, 40, 30};
const int PinStepperDir[3] = {52, 42, 32}; 
const int PinServos[2] = {6, 7};
const int PinSpeed[3][3] =
{
  //Mode 0, Mode 1, Mode 2
  // z-axis
  {49, 51, 53},
  // x-axis
  {39, 41, 43},
  // y-axis
  {29, 31, 33},
};

/* Array used to determin speed of motors
    {Mode 0, Mode 1, Mode 2, Multiplier}
    Mode referes to the speed controlers on the stepper chip
*/
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
int StartPosTarget[5] = {200, 100, 90, 90, 90}; // 0, 1, 2: Axis coordinates; 3, 4: Servo angles

int CoordsTarget[3] = {0, 0, 0};

int AnglesTarget[5] = {0, 0, 0, 90, 90}; //The angle target: z, x, y, a, c
int AnglesIs[3] = {0, 0, 0};

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

int CycleTime = 10;



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
  unsigned int x;
  unsigned int y;
  unsigned int z;
  unsigned int claw_tilt;
  unsigned int claw_width;
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

Coords data;


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
    if (CoordsTarget[0] == 180 && CoordsTarget[1] == 200 && StepsTarget[0] == StepsIs[0] && StepsTarget[1] == StepsIs[1] && StepsTarget[2] == StepsIs[2])
    {
    //Serial.println("One");
    CoordsTarget[0] = 270;
    CoordsTarget[1] = 50;
    }
    else if (CoordsTarget[0] == 270 && CoordsTarget[1] == 50 && StepsTarget[0] == StepsIs[0] && StepsTarget[1] == StepsIs[1] && StepsTarget[2] == StepsIs[2])
    {
    //Serial.println("Two");
    CoordsTarget[0] = 180;
    CoordsTarget[1] = 200;
    }

    if (CoordsTarget[0] == 200 && CoordsTarget[1] == 100 && StepsTarget[0] == StepsIs[0] && StepsTarget[1] == StepsIs[1] && StepsTarget[2] == StepsIs[2])
    {
    CoordsTarget[0] = 180;
    CoordsTarget[1] = 200;
    }*//*
    // Debug info
    Serial.println("");
    Serial.println("");
    Serial.println("Coords");
    Serial.println(CoordsTarget[0]);
    Serial.println(CoordsTarget[1]);
    Serial.println(CoordsTarget[2]);
    Serial.println("Target");
    Serial.println(StepsTarget[0]);
    Serial.println(StepsTarget[1]);
    Serial.println(StepsTarget[2]);
    Serial.println("Is");
    Serial.println(StepsIs[0]);
    Serial.println(StepsIs[1]);
    Serial.println(StepsIs[2]);
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
*/
  Communication();
  ConvCoordsToAngle();
  ConvAngleStep();
  CalcAbsDiff();
  //SensorCheck();
  CtrlSpeed();
  CtrlMotor();
  CtrlServo();

  delay(CycleTime);
}

