#include <roombaDefines.h>

#include <SoftwareSerial.h>

int rxPin = 10;
int txPin = 11;

byte incomming_byte_array[10];

int bumper, buttons;
int cliff_left_value, cliff_center_left_value;
int cliff_right_value, cliff_center_right_value;
bool bumper_left, bumper_right;

unsigned long sensor_request_time = 0;
SoftwareSerial Roomba(rxPin, txPin);
void setup() {
  Roomba.begin(19200);
  Serial.begin(19200);
  pinMode(ddPin, OUTPUT);
  delay(2000);
  Serial.print("Gatavs darbam");
  wakeUp();   // Wake-up Roomba
  startFull();
  setPowerLED(128, 0);
  setDebrisLED(OFF);
  setSpotLED(OFF);
}
void loop() {
  setSpotLED(OFF);
  setDebrisLED(OFF);
  delay(100);
  setDebrisLED(ON);
  setSpotLED(ON);
  delay(100);
  sensorTest();
}
void wakeUp(void)

{

  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);
}
//Uzstādam, ka arduino nosūta datu paku, lai irobots pamostos drošā režīmā
void startSafe()

{
  Roomba.write(128);  //Start
  Roomba.write(131);  //Safe mode
  delay(1000);
}

void startFull()

{
  Roomba.write(128);  //Start
  Roomba.write(132);  //Safe mode
  delay(1000);

}

void setPowerLED(byte setColor, byte setIntensity)
{
  color = setColor;
  intensity = setIntensity;
  Roomba.write(139);
  Roomba.write((byte)0x00);
  Roomba.write((byte)color);
  Roomba.write((byte)intensity);
}

void setDebrisLED(bool enable)
{
  debrisLED = enable;
  Roomba.write(139);
  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));
  Roomba.write((byte)color);
  Roomba.write((byte)intensity);
}

void setSpotLED(bool enable)
{
  spotLED = enable;
  Roomba.write(139);
  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));
  Roomba.write((byte)color);
  Roomba.write((byte)intensity);
}

//MOTORI
void drive(int velocity, int radius)
{
  clamp(velocity, -500, 500); //def max and min velocity in mm/s
  clamp(radius, -2000, 2000); //def max and min radius in mm
  Roomba.write(137);
  Roomba.write(velocity >> 8);
  Roomba.write(velocity);
  Roomba.write(radius >> 8);
  Roomba.write(radius);
}

void driveWheels(int right, int left)

{
  clamp(right, -500, 500);
  clamp(left, -500, 500);
  Roomba.write(145);
  Roomba.write(right >> 8);
  Roomba.write(right);
  Roomba.write(left >> 8);
  Roomba.write(left);
}

void driveWheelsPWM(int rightPWM, int leftPWM)
{
  clamp(rightPWM, -255, 255);
  clamp(leftPWM, -255, 255);
  Roomba.write(146);
  Roomba.write(rightPWM >> 8);
  Roomba.write(rightPWM);
  Roomba.write(leftPWM >> 8);
  Roomba.write(leftPWM);
}

void turnCW(unsigned short velocity, unsigned short degrees)
{
  drive(velocity, -1);
  clamp(velocity, 0, 500);
  delay(1500);
  //delay((1580 + 2.25*velocity)/velocity*degrees);
  //delay((-0.03159720835 * velocity + 21.215270835) * degrees);
  drive(0, 0);
}

void turnCCW(unsigned short velocity, unsigned short degrees)
{
  drive(velocity, 1);
  clamp(velocity, 0, 500);
  delay(1500);
  //delay(2708.3333/velocity*degrees);
  //delay((1580 + 2.25*velocity)/velocity*degrees);
  //delay((-0.03159720835 * velocity + 21.215270835) * degrees);
  drive(0, 0);
}

void driveStop(void)
{
  drive(0, 0);
}

void driveLeft(int left)
{
  driveWheels(left, 0);
}

void driveRight(int right)
{
  driveWheels(0, right);
}
int getSensorData(byte sensorID)

{

  int returnVal;

  byte packetID = 0;

  if (sensorID > 100)

  {

    switch (sensorID)

    {

      case 101:

      case 102:

      case 103:

      case 104:

        packetID = 7;

        break;

      case 105:

      case 106:

      case 107:

      case 108:

        packetID = 14;

        break;

      case 109:

      case 110:

      case 111:

      case 112:

      case 113:

      case 114:

      case 115:

      case 116:

        packetID = 18;

        break;

      case 117:

      case 118:

      case 119:

      case 120:

      case 121:

      case 122:

        packetID = 45;

        break;

    }



  }

  else {

    packetID = sensorID;

  }

  byte MSB = 0;

  byte LSB = 0;

  Roomba.write(142);

  Roomba.write(packetID);

  if (is_in_array(packetID)) {

    while (!Roomba.available());

    returnVal = Roomba.read();

  } else {

    while (!Roomba.available());

    MSB = Roomba.read();

    LSB = Roomba.read();

    returnVal = (MSB << 7) | LSB;

  }



  return returnVal;

}


bool is_in_array(byte val)

{

  for (int i = 0; i < 22; i++) {

    if (val == single_byte_packets[i])

    {

      return true;

    }

  }

  return false;

}








void sensorTest() {
  Serial.print("Bumperis:");
  if (getSensorData(7) == 3) {
    Serial.println("sitiens");
  }
  if (getSensorData(7) == 2) {
    Serial.println("labais");
    turnCCW (90, 1);
  }
  if (getSensorData(7) == 1) {
    Serial.println("kreisais");
     turnCCW (-90, -1);

  }
  else {
 
}
}
// put your main code here, to run repeatedly:
