#include <Arduino.h>

#define A_OUT 5
#define D_OUT 7

void setup() {
  Serial.begin(115200);

  pinMode(A_OUT, INPUT);
  pinMode(D_OUT, INPUT);

  delay(500);
  Serial.println("Stamp-S3 LIGHT Demo Sample");
}

void loop() {
  Serial.print("0:");
  Serial.print(0);
  Serial.print(",");
  Serial.print("3.5:");
  Serial.print(3.5);
  Serial.print(",");
  float vinput = 3.3 * analogRead(A_OUT) / 4095;  //電圧Vに変換
  Serial.print("電圧:");
  Serial.print(vinput);
  Serial.print(",");
  Serial.print("明暗:");
  Serial.println(digitalRead(D_OUT));

  delay(100);
}