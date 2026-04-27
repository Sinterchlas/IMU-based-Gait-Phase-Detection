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
  sensorRate();
  sensorCalc();
  kalmanfilter();

  //Serial.print(Gy_angle);
  //Serial.print("\t");          // Pemisah kolom 1 dan 2
  //Serial.print(Tilt_angle);
  //Serial.print("\t");          // Pemisah kolom 2 dan 3
  Serial.print(GyL_rate);
  Serial.print("\t");          // Pemisah kolom 2 dan 3
  Serial.print(TiltKalL);
  Serial.print("\t");  
  Serial.print(GyR_rate);
  Serial.print("\t");          // Pemisah kolom 2 dan 3
  Serial.println(TiltKalR); 
  delay(10);
}


