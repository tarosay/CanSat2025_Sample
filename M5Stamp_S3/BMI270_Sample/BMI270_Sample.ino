#include "Arduino_BMI270_BMM150.h"

void setup() {
  Serial.begin(115200);
  delay(1500);

  while (!Serial)
    ;
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
}

float Ax = 0;
float Ay = 0;
float Az = 0;
float Gx = 0;
float Gy = 0;
float Gz = 0;

void loop() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(Ax, Ay, Az);
  }

  // if (IMU.gyroscopeAvailable()) {
  //   IMU.readGyroscope(Gx, Gy, Gz);
  // }

  String log = "";
  log += String(Ax) + "," + String(Ay) + "," + String(Az) + ",1,-1";
  //log += String(Gx) + "," + String(Gy) + "," + String(Gz) + ",500,-500";
  Serial.println(log);
  delay(100);
}
