#include "Arduino_BMI270_BMM150.h"

class Bmi270 : public BoschSensorClass {  // ← 好きなクラス名
protected:
  int8_t configure_sensor(struct bmi2_dev* dev) override {
    bmi2_sens_config cfg{};
    cfg.type = BMI2_ACCEL;
    cfg.cfg.acc.range = BMI2_ACC_RANGE_8G;
    cfg.cfg.acc.odr = BMI2_ACC_ODR_100HZ;
    cfg.cfg.acc.bwp = BMI2_ACC_NORMAL_AVG4;
    cfg.cfg.acc.filter_perf = BMI2_PERF_OPT_MODE;
    if (bmi2_set_sensor_config(&cfg, 1, dev) != BMI2_OK) return -1;
    const uint8_t list[1] = { BMI2_ACCEL };
    return bmi2_sensor_enable(list, 1, dev);
  }
};

Bmi270 bmi;

void setup() {
  Serial.begin(115200);
  delay(1500);

  while (!Serial)
    ;
  Serial.println("Started");

  if (!bmi.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(bmi.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.print("Gyroscope sample rate = ");
  Serial.print(bmi.gyroscopeSampleRate());
  Serial.println(" Hz");
}

float Ax = 0;
float Ay = 0;
float Az = 0;
float Gx = 0;
float Gy = 0;
float Gz = 0;

void loop() {
  if (bmi.accelerationAvailable()) {
    bmi.readAcceleration(Ax, Ay, Az);
  }

  // if (bmi.gyroscopeAvailable()) {
  //   bmi.readGyroscope(Gx, Gy, Gz);
  // }

  String log = "";
  log += String(Ax) + "," + String(Ay) + "," + String(Az) + ",5,-5";
  //log += String(Gx) + "," + String(Gy) + "," + String(Gz) + ",500,-500";
  Serial.println(log);
  delay(100);
}
