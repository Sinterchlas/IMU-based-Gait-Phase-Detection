#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

const int pthighAx = PA6; 
const int pthighAz = PA5;
const int pthighGy = PA7;
const float Rad2Deg = 57.2957795;

// Filter Coefficients
const float a1 = 1.64746;
const float a2 = 0.70090;    
const float b0 = 0.01336;   
const float b1 = 0.02672;    
const float b2 = 0.01336;
extern float Ylpf[3], Ylpf_1[3],Ylpf_2[3];
extern float sinyal[3], sinyal_1[3], sinyal_2[3];
// sinyal[0] = Ax, sinyal[1] = Az, sinyal[2] = Gy

extern float Accoffset, Accsens, Gysens;
//extern float rawthighAx, rawthighAz, rawthighGy;
extern float Gy_rate, Acz_rate, Acx_rate; 
extern float Gy_angle;
extern float dt;
extern long sumGy; extern float Gyoffset;
extern float Tilt_angle, alpha, Angle;

#endif CONFIG_H