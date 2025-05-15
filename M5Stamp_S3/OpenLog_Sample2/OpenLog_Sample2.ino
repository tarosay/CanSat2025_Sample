#include <Arduino.h>
#include <TinyGPSPlus.h>

// The TinyGPSPlus object
TinyGPSPlus gps;
unsigned long lastRead = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(19200, SERIAL_8N1, 1, 3);     //G1->RXD, G3->TXD);
  Serial2.begin(115200, SERIAL_8N1, 44, 43);  //G44->RXD, G43->TXD

  delay(1500);
  Serial.println(F("Stamp S3 OpenLog Sample"));
  Serial1.println(F("Stamp S3 OpenLog Sample"));
}

void loop() {
  if (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }
  unsigned long now = millis();
  if (now - lastRead > 1000)  // 毎秒更新
  {
    lastRead = now;

    if (gps.location.isValid() && gps.date.isValid() && gps.time.isValid()) {
      int utcHour = gps.time.hour();
      int utcMinute = gps.time.minute();
      int utcSecond = gps.time.second();
      int jstHour = utcHour + 9;
      int jstDay = gps.date.day();
      int jstMonth = gps.date.month();
      int jstYear = gps.date.year();

      if (jstHour >= 24) {
        jstHour -= 24;
        jstDay += 1;
        // 月末処理（略式）
        if ((jstMonth == 4 || jstMonth == 6 || jstMonth == 9 || jstMonth == 11) && jstDay > 30) {
          jstDay = 1;
          jstMonth++;
        } else if (jstMonth == 2 && jstDay > 28) {
          jstDay = 1;
          jstMonth++;
        } else if (jstDay > 31) {
          jstDay = 1;
          jstMonth++;
        }
        if (jstMonth > 12) {
          jstMonth = 1;
          jstYear++;
        }
      }

      Serial.println(F("------ GNSS Data ------"));
      Serial.print(F("UTC: "));
      Serial.print(gps.date.year());
      Serial.print("/");
      Serial.print(gps.date.month());
      Serial.print("/");
      Serial.print(gps.date.day());
      Serial.print(F(" "));
      Serial.print(gps.time.hour());
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.println(gps.time.second());

      Serial.print(F("JST: "));
      Serial.print(jstYear);
      Serial.print("/");
      Serial.print(jstMonth);
      Serial.print("/");
      Serial.print(jstDay);
      Serial.print(F(" "));
      Serial.print(jstHour);
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.println(gps.time.second());

      Serial.print(F("Latitude: "));
      Serial.print(gps.location.lat(), 6);
      Serial.print(F(" Longitude: "));
      Serial.println(gps.location.lng(), 6);

      Serial.print(F("Speed (km/h): "));
      Serial.println(gps.speed.kmph());

      Serial.print(F("Course (deg): "));
      Serial.println(gps.course.deg());

      Serial.print(F("Satellites: "));
      Serial.println(gps.satellites.value());

      Serial.print(F("Altitude (m): "));
      Serial.println(gps.altitude.meters());

      Serial.print(F("HDOP: "));
      Serial.println(gps.hdop.value());
      Serial.println(F("-----------------------\n"));

      Serial1.print(millis());
      Serial1.print(F(","));
      Serial1.print(gps.date.year());
      Serial1.print(F(","));
      Serial1.print(gps.date.month());
      Serial1.print(F(","));
      Serial1.print(gps.date.day());
      Serial1.print(F(","));
      Serial1.print(gps.time.hour());
      Serial1.print(F(","));
      Serial1.print(gps.time.minute());
      Serial1.print(F(","));
      Serial1.print(gps.time.second());
      Serial1.print(F(","));
      Serial1.print(jstYear);
      Serial1.print(F(","));
      Serial1.print(jstMonth);
      Serial1.print(F(","));
      Serial1.print(jstDay);
      Serial1.print(F(","));
      Serial1.print(jstHour);
      Serial1.print(F(","));
      Serial1.print(gps.time.minute());
      Serial1.print(F(","));
      Serial1.print(gps.time.second());
      Serial1.print(F(","));
      Serial1.print(gps.location.lat(), 6);
      Serial1.print(F(","));
      Serial1.print(gps.location.lng(), 6);
      Serial1.print(F(","));
      Serial1.print(gps.speed.kmph());
      Serial1.print(F(","));
      Serial1.print(gps.course.deg());
      Serial1.print(F(","));
      Serial1.print(gps.satellites.value());
      Serial1.print(F(","));
      Serial1.print(gps.altitude.meters());
      Serial1.print(F(","));
      Serial1.println(gps.hdop.value());
    }
  }

  delay(10);
}
