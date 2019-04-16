#include <roombaDefines.h>

#include <SoftwareSerial.h>

int rxPin = 2;
int txPin = 3;

byte incomming_byte_array[10];

int bumper, buttons;
int cliff_left_value, cliff_center_left_value;
int cliff_right_value, cliff_center_right_value;
bool bumper_left, bumper_right;

unsigned long sensor_request_time = 0;


SoftwareSerial Roomba(rxPin, txPin);


void setup() {
  // put your setup code here, to run once:
  Roomba.begin(19200);
  Serial.begin(19200);
  pinMode(ddPin, OUTPUT);
  delay(2000);

  Serial.print("Roomba Remote Control");
  wakeUp ();   // Wake-up Roomba
  // startSafe(); // Start Roomba in Safe Mode
  startFull();
  setPowerLED(128, 0);
  setDebrisLED(OFF);
  setDockLED(OFF);
  setSpotLED(OFF);
  setWarningLED(OFF);
  cleanDigitLED ();

}

void loop() {
  // put your main code here, to run repeatedly:

  setDebrisLED(OFF);
  setDockLED(OFF);
  setSpotLED(OFF);
  setWarningLED(OFF);
  cleanDigitLED ();
  delay(1000);
  setDebrisLED(ON);
  setDockLED(ON);
  setSpotLED(ON);
  setWarningLED(ON);
  cleanDigitLED ();
  delay(1000);
  //motorSquareTest();
  sensorTest();

}

void wakeUp (void)

{

  setWarningLED(ON);

  digitalWrite(ddPin, HIGH);

  delay(100);

  digitalWrite(ddPin, LOW);

  delay(500);

  digitalWrite(ddPin, HIGH);

  delay(2000);

}

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



//---------------------------------------------------------------

void setDebrisLED(bool enable)

{

  debrisLED = enable;

  Roomba.write(139);

  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));

  Roomba.write((byte)color);

  Roomba.write((byte)intensity);

}



//---------------------------------------------------------------

void setSpotLED(bool enable)

{

  spotLED = enable;

  Roomba.write(139);

  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));

  Roomba.write((byte)color);

  Roomba.write((byte)intensity);

}



//---------------------------------------------------------------

void setDockLED(bool enable)

{

  dockLED = enable;

  Roomba.write(139);

  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));

  Roomba.write((byte)color);

  Roomba.write((byte)intensity);

}



//---------------------------------------------------------------

void setWarningLED(bool enable)

{

  warningLED = enable;

  Roomba.write(139);

  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));

  Roomba.write((byte)color);

  Roomba.write((byte)intensity);

}



/*--------------------------------------------------------------------------

  This command controls the four 7 segment displays on the Roomba.*/

void setDigitLEDs(byte digit1, byte digit2, byte digit3, byte digit4)

{

  Roomba.write(163);

  Roomba.write(digit1);

  Roomba.write(digit2);

  Roomba.write(digit3);

  Roomba.write(digit4);

}



/*--------------------------------------------------------------------------

  This command controls the four 7 segment displays on the Roomba using ASCII character codes.*/

void setDigitLEDFromASCII(byte digit, char letter)

{

  switch (digit) {

    case 1:

      digit1 = letter;

      break;

    case 2:

      digit2 = letter;

      break;

    case 3:

      digit3 = letter;

      break;

    case 4:

      digit4 = letter;

      break;

  }

  Roomba.write(164);

  Roomba.write(digit1);

  Roomba.write(digit2);

  Roomba.write(digit3);

  Roomba.write(digit4);

}



//---------------------------------------------------------------

void cleanDigitLED (void)

{

  setDigitLEDFromASCII(1, ' ');

  setDigitLEDFromASCII(2, ' ');

  setDigitLEDFromASCII(3, ' ');

  setDigitLEDFromASCII(4, ' ');

}

//---------------------------------------------------------------

void writeLEDs (char a, char b, char c, char d)

{

  setDigitLEDFromASCII(1, a);

  setDigitLEDFromASCII(2, b);

  setDigitLEDFromASCII(3, c);

  setDigitLEDFromASCII(4, d);

}
void motorSquareTest(void)

{

  setPowerLED(0, 128);

  drive (60, 0);

  delay(5000);

  driveStop();

  turnCCW (30, 90);



  setPowerLED(64, 128);

  drive (60, 0);

  delay(5000);

  driveStop();

  turnCCW (30, 90);



  setPowerLED(128, 128);

  drive (60, 0);

  delay(5000);

  driveStop();

  turnCCW (30, 90);



  setPowerLED(190, 128);

  drive (60, 0);

  delay(5000);

  driveStop();

  setPowerLED(255, 128);

  turnCCW (30, 90);

  setPowerLED(0, 0);

}
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



//---------------------------------------------------------------



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



//---------------------------------------------------------------

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



//---------------------------------------------------------------

void turnCW(unsigned short velocity, unsigned short degrees)

{

  drive(velocity, -1);

  clamp(velocity, 0, 500);

  delay(6600);

  //delay((1580 + 2.25*velocity)/velocity*degrees);

  //delay((-0.03159720835 * velocity + 21.215270835) * degrees);

  drive(0, 0);

}



//---------------------------------------------------------------

void turnCCW(unsigned short velocity, unsigned short degrees)

{

  drive(velocity, 1);

  clamp(velocity, 0, 500);

  delay(6600);

  //delay(2708.3333/velocity*degrees);

  //delay((1580 + 2.25*velocity)/velocity*degrees);

  //delay((-0.03159720835 * velocity + 21.215270835) * degrees);

  drive(0, 0);

}



//---------------------------------------------------------------

void driveStop(void)

{

  drive(0, 0);

}



//---------------------------------------------------------------

void driveLeft(int left)

{

  driveWheels(left, 0);

}



//---------------------------------------------------------------

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

  Serial.print("sensors ID 45:");

  Serial.println(getSensorData(45));
  delay(1000);




  // nolasam Roombas sensorus

  // robotam pieprasam datus ne biežāk par 50 ms
  /* if (millis() - sensor_request_time >= 50) {

     sensor_request_time = millis();    // saglabājam pašreizējo laiku
     // mainīgais, kurš skaitīs saņemto baitu skaitu
     byte incomming_byte_counter = 0;

     Roomba.write(149);   // sensoru nolasīšanas komanda
     Roomba.write(6);     // nolasāmo sensoru skaits

     Roomba.write(7);   // bamperis
     Roomba.write(18);  // pogas
     Roomba.write(28);  // kreisais pakāpiena sensors
     Roomba.write(29);  // kreisais priekšējais pakāpiena sensors
     Roomba.write(30);  // labais priekšējais pakāpiena sensors
     Roomba.write(31);  // labais pakāpiena sensors

     // gaidam jaunus datus, ja saņemto baitu skaits ir mazāks par 10 un laiks, kurā gaidam datus ir mazāks par 50 ms
     while ((incomming_byte_counter < 10) && (millis() - sensor_request_time < 50)) {

       // ja dati ir pienākuši pa seriālo portu no robota
       if (Roomba.available()) {
         // nolasam vienu saņemto baitu un saglabājam datu masīvā
         incomming_byte_array[incomming_byte_counter] = Roomba.read();

         // palielinam baitu skaitītāju
         incomming_byte_counter++;
       }
     }

     // par cik bampera baitā ir iekšā arī riteņu izkrišanas dati, tad nolasam tikai vajadzīgos bitus
     if ((incomming_byte_array[0] & 2) == 0) {
       bumper_left = false;
     } else {
       bumper_left = true;
     }
     if ((incomming_byte_array[0] & 1) == 0) {
       bumper_right = false;
     } else {
       bumper_right = true;
     }

     buttons = incomming_byte_array[1];

     cliff_left_value = (incomming_byte_array[2] << 8) | incomming_byte_array[3];
     cliff_center_left_value = (incomming_byte_array[4] << 8) | incomming_byte_array[5];
     cliff_center_right_value = (incomming_byte_array[6] << 8) | incomming_byte_array[7];
     cliff_right_value = (incomming_byte_array[8] << 8) | incomming_byte_array[9];


     Serial.print("Kreisais bamperis: ");
     Serial.println(bumper_left, DEC);

     Serial.print("Labais bamperis: ");
     Serial.println(bumper_right, DEC);

     Serial.print("Pogas: ");
     Serial.println(buttons, DEC);

     Serial.print("Kreisais pakapiena sensors: ");
     Serial.println(cliff_left_value, DEC);

     Serial.print("Kreisais prieksejais pakapiena sensors: ");
     Serial.println(cliff_center_left_value, DEC);

     Serial.print("Labais prieksejais pakapiena sensors: ");
     Serial.println(cliff_center_right_value, DEC);

     Serial.print("Labais pakapiena sensors: ");
     Serial.println(cliff_right_value, DEC);

     Serial.println();
    }

    if (digitalRead(pogaAugsa) == LOW){
     fight= true;
     Serial.print("pogaaugsa");
     }

     if (digitalRead(pogaApaksa) == LOW){
     fight= false;
     Serial.print("pogaapaksa");
     drive(0,0);
     }
     if (fight){
     if (cliff_left_value > 1800 || cliff_center_left_value > 1800){
       drive(-250, -250);
       delay(900);
       drive(-250, 250);
       delay(800);
     }
     else if (cliff_right_value > 1800 || cliff_center_right_value > 1800 ){   //1800 vai lielaka, lai pamanitu balto liniju
       drive(-250, -250);
       delay(900);
       drive(250, -250);
       delay(800);
     }
     else if (bumper_left == 1 && bumper_right == 1){
       drive(200,200);
     }
     else if (bumper_left == 1){
       drive(200,200);
     }
     else if (bumper_right == 1){
       drive(200,200);
     }
     else {
       drive(200, 200);
     }
     }
  */
}
