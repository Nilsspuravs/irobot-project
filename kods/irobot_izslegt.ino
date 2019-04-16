#include <roombaDefines.h>

#include <SoftwareSerial.h>

int rxPin = 10;
int txPin = 11;
SoftwareSerial Roomba(rxPin, txPin);
void setup() {
    Roomba.begin(19200);
  Serial.begin(19200);
  pinMode(ddPin, OUTPUT);
  delay(2000);
  Serial.print("Gatavs darbam");
  wakeUp();
turnoff();
  // put your setup code here, to run once:

}
void wakeUp (void)

{



  digitalWrite(ddPin, HIGH);

  delay(100);

  digitalWrite(ddPin, LOW);

  delay(500);

  digitalWrite(ddPin, HIGH);

  delay(2000);

}

void turnoff (void)
{
 Roomba.write(173);
}
void loop() {
  // put your main code here, to run repeatedly:

}
