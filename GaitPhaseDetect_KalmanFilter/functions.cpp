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

void kalmanfilter(){
  // Menggunakan kecepatan sudut dari Gyro Y
  TiltKal += (Gy_rate - Bias) * dt;
  
  // Update matriks error covariance (P)
  P1 += (Qangle + P4 * dt - P3 - P2) * dt;
  P2 -= P4 * dt;
  P3 -= P4 * dt;
  P4 += Qgyro * dt;

  // Step 2: Pembaruan (Update)
  // Hitung Kalman Gain (K)
  ErrEstmt = P1 + Rangle;
  K0 = P1 / ErrEstmt;
  K1 = P3 / ErrEstmt;
  
  // Hitung selisih antara sudut akselerometer (X dan Z) dan estimasi Kalman
  ErrTilt = Tilt_angle - TiltKal;

  // Koreksi estimasi sudut dan bias giroskop
  TiltKal += K0 * ErrTilt;
  Bias += K1 * ErrTilt;

  // Update matriks error covariance (P)
  P1 -= K0 * P1;
  P2 -= K0 * P2;
  P3 -= K1 * P1;
  P4 -= K1 * P2;
}