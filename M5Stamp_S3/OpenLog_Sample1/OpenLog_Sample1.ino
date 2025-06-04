#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 1, 3);  //G1->RXD, G3->TXD);

  delay(1500);
  Serial.println(F("Stamp S3 OpenLog Sample"));
  Serial1.println(F("Stamp S3 OpenLog Sample"));
}

void loop() {
  delay(10);
}
