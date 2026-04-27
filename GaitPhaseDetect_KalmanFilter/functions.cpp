#include "functions.h"
#include "config.h"

void readsensors(){
  sinyal[0] = analogRead(pthighAx); //rawthighAx
  sinyal[1] = analogRead(pthighAz); //rawthighAz 
  sinyal[2] = analogRead(pthighGy); //rawthighGy
  lpfsensors();
}

void printraw(){
  Serial.print(sinyal[0]);
  Serial.print("\t");
  Serial.print(sinyal[1]);
  Serial.print("\t");
  Serial.print(sinyal[2]);
}

float cal_sensorconvert(float rawADC, float ADCoffset, float ADCsens){
  float sensorRate = (rawADC-ADCoffset)/ADCsens;
  return sensorRate;
}

void Gy_caliberation(){
  long sumGy = 0.0;
  for (int i = 0; i < 500; i++){
    sumGy += analogRead(pthighGy);
    delay(2);
  }
   Gyoffset = sumGy / 500.0;

}

void lpfsensors(){
  for(int i=0; i<3; i++){
    Ylpf[i] = (a1 * Ylpf_1[i] - a2 * Ylpf_2[i] + b0 * sinyal[i] + 
              b1 * sinyal_1[i] + b2 * sinyal_2[i]);
  if (Ylpf[i] < 0) Ylpf[i] =0;
  if (Ylpf[i] >4096) Ylpf[i] =4096;

  Ylpf_2[i] = Ylpf_1[i];
  Ylpf_1[i] = Ylpf[i];
  sinyal_2[i] = sinyal_1[i];
  sinyal_1[i] = sinyal[i];

  }
}