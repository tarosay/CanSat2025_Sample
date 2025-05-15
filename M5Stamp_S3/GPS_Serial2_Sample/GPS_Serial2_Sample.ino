#include <Arduino.h>
#include <TinyGPSPlus.h>

// The TinyGPSPlus object
TinyGPSPlus gps;
unsigned long lastRead = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 44, 43);  //G44->RXD, G43->TXD

  delay(500);
  Serial.println(F("Stamp S3 GPS Sample"));
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
    }
  }

  delay(10);
}
