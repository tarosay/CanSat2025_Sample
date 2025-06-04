#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;  // I2C
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
#define SEALEVELPRESSURE_HPA 1013.25
//#define SEALEVELPRESSURE_HPA 1009.0

void setup() {
  Serial.begin(115200);
  delay(15000);
  Serial.println(F("BMP280 Sample"));

  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(
    //Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
    Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  bmp_temp->printSensorDetails();
  bmp_pressure->printSensorDetails();
}

float Temp, Press, Alti;

void loop() {
  //温度
  Temp = bmp.readTemperature();
  //気圧
  Press = bmp.readPressure();
  //高度
  //Alti = bmp.readAltitude(SEALEVELPRESSURE_HPA)); /* Adjusted to local forecast! */
  Alti = calculateAltitude(Press, Alti);

  String log = "";
  log += String(Temp) + ", ";
  log += String(Press) + ", ";
  log += String(Alti) + ", ";
  log += "20, 35";
  Serial.println(log);
  delay(1000);
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
