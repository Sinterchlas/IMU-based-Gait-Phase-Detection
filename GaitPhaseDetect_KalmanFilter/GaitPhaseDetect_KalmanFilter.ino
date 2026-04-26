#include <arduino.h>
#include <math.h>
#include "functions.h"
#include "config.h"
unsigned long previousTime;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  Gy_caliberation();
  previousTime = micros(); //mungkin bisa juga pake millis()
}

void loop() {
  unsigned long currentTime = micros();
  dt = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;

  readsensors();
  //printraw();
// KODE YANG BENAR
  Gy_rate = cal_sensorconvert(Ylpf[2], Gyoffset, Gysens);
  Acz_rate = cal_sensorconvert(Ylpf[1], Accoffset, Accsens);
  Acx_rate = cal_sensorconvert(Ylpf[0], Accoffset, Accsens);

  Tilt_angle = (atan2f(Acx_rate, Acz_rate) * Rad2Deg)-90;
  Gy_angle = Gy_angle + Gy_rate * dt;

  kalmanfilter();

  //Serial.print(Gy_angle);
  //Serial.print("\t");          // Pemisah kolom 1 dan 2
  //Serial.print(Tilt_angle);
  //Serial.print("\t");          // Pemisah kolom 2 dan 3
  Serial.print(Gy_rate);
  Serial.print("\t");          // Pemisah kolom 2 dan 3
  Serial.println(TiltKal);
  delay(10);
}


