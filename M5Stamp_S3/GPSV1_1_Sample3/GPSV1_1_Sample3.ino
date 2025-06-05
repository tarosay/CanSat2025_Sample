#include <Arduino.h>
#include <TinyGPSPlus.h>

// The TinyGPSPlus object
TinyGPSPlus gps;
unsigned long lastRead = 0;

void setup() {
  Serial.begin(115200);
  //Serial1.begin(19200, SERIAL_8N1, 1, 3);     //G1->RXD, G3->TXD);
  Serial2.begin(115200, SERIAL_8N1, 44, 43);  //G44->RXD, G43->TXD
  delay(1500);

  Serial.println(F("GPS V1.1 Sample3"));
}

void loop() {
  if (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }
  unsigned long now = millis();
  if (now - lastRead > 250)  // 250ms更新
  {
    lastRead = now;

    if (gps.location.isValid()) {
      String log = "";
      log += String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
      Serial.println(log);
      
      //OpenLogへ
      //log = String(millis() / 1000.0, 2) + "," + log;
      //Serial1.println(log);
    }
  }

  delay(10);
}
