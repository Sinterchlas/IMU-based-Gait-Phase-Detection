#include "functions.h"
#include "config.h"

void readsensors(){
  sinyal[0] = analogRead(PinLeftGy);
  sinyal[1] = analogRead(PinLeftAx); 
  sinyal[2] = analogRead(PinLeftAz);  
   
  sinyal[3] = analogRead(PinRightGy); 
  sinyal[4] = analogRead(PinRightAx); 
  sinyal[5] = analogRead(PinRightAz); 

  lpfsensors();
}

void lpfsensors(){
  for(int i=0; i<6; i++){
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

void printraw(){
  Serial.print(sinyal[0]);
  Serial.print("\t");
  Serial.print(sinyal[1]);
  Serial.print("\t");
  Serial.print(sinyal[2]);
}

void sensorRate(){
  GyL_rate = cal_sensorconvert(Ylpf[0], GyLoffset, Gysens);
  AcxL_rate = cal_sensorconvert(Ylpf[1], Accoffset, Accsens);
  AczL_rate = cal_sensorconvert(Ylpf[2], Accoffset, Accsens);
  GyR_rate = cal_sensorconvert(Ylpf[3], GyRoffset, Gysens);
  AcxR_rate = cal_sensorconvert(Ylpf[4], Accoffset, Accsens);
  AczR_rate = cal_sensorconvert(Ylpf[5], Accoffset, Accsens);
}

float cal_sensorconvert(float rawADC, float ADCoffset, float ADCsens){
  float sensorRate = (rawADC-ADCoffset)/ADCsens;
  return sensorRate;
}

void sensorCalc(){
  Tilt_angleL = -((atan2f(AcxL_rate, AczL_rate) * Rad2Deg)-90);
  Tilt_angleR = -((atan2f(AcxR_rate, AczR_rate) * Rad2Deg)-90);
  
  // SUDUT DARI INTEGRASI W
 // GyL_angle = GyL_angle + GyL_rate * dt; 
 // GyR_angle = GyR_angle + GyR_rate * dt;
}

void Gy_caliberation(){
  long sumGyL = 0.0, sumGyR = 0.0;
  for (int i = 0; i < 500; i++){
    sumGyL += analogRead(PinLeftGy);
    sumGyR += analogRead(PinRightGy);
    delay(2);
  }
  GyLoffset = sumGyL / 500.0;
  GyRoffset = sumGyR / 500.0;

}

void kalmanfilter(){
  // Menggunakan kecepatan sudut dari Gyro Y
  GyL_stable = GyL_rate - BiasL;
  GyR_stable = GyR_rate - BiasR;
  TiltKalL += GyL_stable * dt;
  TiltKalR += GyR_stable * dt;
  
  // Update matriks error covariance (P)
  P1L += (QangleL + P4L * dt - P3L - P2L) * dt;
  P2L -= P4L * dt;
  P3L -= P4L * dt;
  P4L += QgyroL * dt;

  P1R += (QangleR + P4R * dt - P3R - P2R) * dt;
  P2R -= P4R * dt;
  P3R -= P4R * dt;
  P4R += QgyroR * dt;

  // Step 2: Pembaruan (Update)
  // Hitung Kalman Gain (K)
  ErrEstmtL = P1L + RangleL;
  K0L = P1L / ErrEstmtL;
  K1L = P3L / ErrEstmtL;

  ErrEstmtR = P1R + RangleR;
  K0R = P1R / ErrEstmtR;
  K1R = P3R / ErrEstmtR;
  
  // Hitung selisih antara sudut akselerometer (X dan Z) dan estimasi Kalman
  ErrTiltL = Tilt_angleL - TiltKalL;
  ErrTiltR = Tilt_angleR - TiltKalR;

  // Koreksi estimasi sudut dan bias giroskop
  TiltKalL += K0L * ErrTiltL;
  TiltKalR += K0R * ErrTiltR;
  BiasL += K1L * ErrTiltL;
  BiasR += K1R * ErrTiltR;

  // Update matriks error covariance (P)
  P1L -= K0L * P1L;
  P2L -= K0L * P2L;
  P3L -= K1L * P1L;
  P4L -= K1L * P2L;

  P1R -= K0R * P1R;
  P2R -= K0R * P2R;
  P3R -= K1R * P1R;
  P4R -= K1R * P2R;
}

void peakdetection(){
  buffGyL[0] = buffGyL[1];
  buffGyL[1] = buffGyL[2];
  buffGyL[2] = GyL_stable;
  buffGyR[0] = buffGyR[1];
  buffGyR[1] = buffGyR[2];
  buffGyR[2] = GyR_stable;

  buffAngleL[0] = buffAngleL[1];
  buffAngleL[1] = buffAngleL[2];
  buffAngleL[2] = -TiltKalL;
  buffAngleR[0] = buffAngleR[1];
  buffAngleR[1] = buffAngleR[2];
  buffAngleR[2] = -TiltKalR;

  // RIGHT IC DETECTION (MIN ANGLE)
  if (buffAngleR[1]<buffAngleR[0] && buffAngleR[1]<buffAngleR[2]){
    if (buffAngleR[1]<thresIC){
      if(gaitState == 6){
        gaitState = 1;}}}

  // LEFT EC DETECTION (MIN W)
  if (buffGyL[1]<buffGyL[0] && buffGyL[1]<buffGyL[2]){
    if (buffGyL[1]<thresEC){
      if(gaitState == 1){
        gaitState = 2;}}}

    // LEFT MS DETECTION (MAX W)
  if (buffGyL[1]>buffGyL[0] && buffGyL[1]>buffGyL[2]){
    if (buffGyL[1]>thresMS){
      if(gaitState == 2){
        gaitState = 3;}}}

    // LEFT IC DETECTION (MIN ANGLE)
  if (buffAngleL[1]<buffAngleL[0] && buffAngleL[1]<buffAngleL[2]){
    if (buffAngleL[1]<thresIC){
      if(gaitState == 3){
        gaitState = 4;}}}

  // RIGHT EC DETECTION (MIN W)
  if (buffGyR[1]<buffGyR[0] && buffGyR[1]<buffGyR[2]){
    if (buffGyR[1]<thresEC){
      if(gaitState == 4){
        gaitState = 5;}}}

    // RIGHT MS DETECTION (MAX W)
  if (buffGyR[1]>buffGyR[0] && buffGyR[1]>buffGyR[2]){
    if (buffGyR[1]>thresMS){
      if(gaitState == 5){
        gaitState = 6;}}}
        
}

void fsrdetection(){
  digitalWrite(controlPin, LOW);  // Seleksi Kanal 0
  delay(1);                       // Settling time kapasitor internal ADC
  rawToe = analogRead(PinFSR);
  
  // Membaca Heel FSR (Asumsi terhubung pada Channel 1)
  digitalWrite(controlPin, HIGH); // Seleksi Kanal 1
  delay(1);                       // Settling time kapasitor internal ADC
  rawHeel = analogRead(PinFSR);

  // 2. Pemrosesan Sinyal (Digital LPF)
  // Menghaluskan fluktuasi sinyal pembacaan
  heel_LPF = (alpha * rawHeel) + ((1.0 - alpha) * heel_LPF);
  toe_LPF  = (alpha * rawToe) + ((1.0 - alpha) * toe_LPF);

  // 3. Logika Deteksi Fase Gait
  // Fase Stance
  if (heel_LPF >= threshold || toe_LPF >= threshold) {
    if (gaitFlag == 0) {
      gaitFlag = 1;
    }
  }
  // Fase Swing
  else if (heel_LPF < threshold && toe_LPF < threshold) {
    if (gaitFlag == 1) {
      gaitFlag = 0;
    }
  }

  // Deteksi Transisi Heel-Off
  if (heel_LPF < threshold) {
    if (heelOff == 0) {
      heelOff = 1;
    }
  } else {
    heelOff = 0;
  }
}
