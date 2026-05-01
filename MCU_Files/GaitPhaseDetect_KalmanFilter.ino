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
  dt = (currentTime - previousTime) / 1000000.0;
  previousTime = currentTime;

  readsensors();
  //printraw();
  sensorRate();
  sensorCalc();
  kalmanfilter();
  peakdetection();
  fsrdetection();

  /*// bad output
  Serial.print(GyL_angle); // heavily drifted integrated angval
  Serial.print("\t");  
  Serial.print(Tilt_angleL); // heavily noisy tilt from accelero
  Serial.print("\t");      
  Serial.print(GyR_angle); // heavily drifted integrated angval
  Serial.print("\t");  
  Serial.println(Tilt_angleR); // heavily noisy tilt from accelero
  */

  
  // good output
  //Serial.print(GyL_rate); // Gyro with bias
  Serial.print(GyL_stable);
  Serial.print("\t");        
  Serial.print(-TiltKalL);
  Serial.print("\t");  
  //Serial.print(GyR_rate);// Gyro with bias
  Serial.print(GyR_stable);
  Serial.print("\t");         
  Serial.print(-TiltKalR); 
  Serial.print("\t");
  Serial.print(gaitState);
  Serial.print("\t");
  Serial.println(gaitFlag);
  delay(10);
}


