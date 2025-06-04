#include <Arduino_BMI270_BMM150.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;  // I2C
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

#define SEALEVELPRESSURE_HPA 1013.25
#define SEALEVELPRESSURE_HPA 1009.0

void setup() {
  Serial.begin(115200);
  Serial1.begin(19200, SERIAL_8N1, 1, 3);  //G1->RXD, G3->TXD);
  delay(15000);
  Serial.println(F("IMU PRO Write OpenLog"));

  if (!IMU.begin()) {
    Serial.println(F("Failed to initialize BMI270!"));
    while (1)
      ;
  }

  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    Serial.println(F("Failed to initialize BMP280!"));
    while (1)
      ;
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  /* Default settings from datasheet. */
  bmp.setSampling(
    //Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
    Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  //BMP280センサの詳細を表示
  bmp_temp->printSensorDetails();
  bmp_pressure->printSensorDetails();
}

float Ax = 0;
float Ay = 0;
float Az = 0;
float Gx = 0;
float Gy = 0;
float Gz = 0;
float Temp, Press, Alti;

void loop() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(Ax, Ay, Az);
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(Gx, Gy, Gz);
  }

  Temp = bmp.readTemperature();
  Press = bmp.readPressure();
  //Alti = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  Alti = calculateAltitude(Press, Temp);

  String log = String(millis() / 1000.0, 2) + ",";
  log += String(Ax) + "," + String(Ay) + "," + String(Az) + ",";
  log += String(Gx) + "," + String(Gy) + "," + String(Gz) + ",";
  log += String(Temp) + "," + String(Press) + "," + String(Alti) + ",";
  Serial.println(log);
  Serial1.println(log);
  delay(250);
}

/*
* 気圧と温度から高度を計算します
*/
float calculateAltitude(float pressure, float temperature) {
  // 摂氏からケルビンへ変換
  float T = temperature + 273.15;
  // 海面気圧をパスカル単位に変換
  float P0 = SEALEVELPRESSURE_HPA * 100.0;
  // 気温の減率
  float L = 0.0065;
  // 基準温度
  float T0 = 288.15;
  // 気体定数
  float R = 8.3144598;
  // 重力加速度
  float g = 9.80665;
  // 空気のモル質量
  float M = 0.0289644;

  // 高度計算
  return (T / L) * (pow((P0 / pressure), (R * L) / (g * M)) - 1);
}
