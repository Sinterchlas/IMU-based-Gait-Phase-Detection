#include <arduino.h>
#include <math.h>
#include "functions.h"
#include "config.h"
unsigned long previousTime;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  Gy_caliberation();
  previousTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  dt = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;

  readsensors();
  //printraw();
  Gy_rate = cal_sensorconvert(sinyal[2], Gyoffset, Gysens);
  Acz_rate = cal_sensorconvert(sinyal[1], Accoffset, Accsens);
  Acx_rate = cal_sensorconvert(sinyal[0], Accoffset, Accsens);

  Tilt_angle = atan2f(Acx_rate, Acz_rate) * Rad2Deg;
  Gy_angle = Gy_angle + Gy_rate * dt;
  Angle = alpha * (Angle + Gy_rate * dt) + (1 - alpha) * Tilt_angle;

  //Serial.print(Gy_angle);
  //Serial.print("\t");          // Pemisah kolom 1 dan 2
  //Serial.print(Tilt_angle);
  //Serial.print("\t");          // Pemisah kolom 2 dan 3
  Serial.print(Gy_rate);
  Serial.print("\t");          // Pemisah kolom 2 dan 3
  Serial.println(Angle);
  delay(10);
}


