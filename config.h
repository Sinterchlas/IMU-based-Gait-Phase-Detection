#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

// PINOUT
const int PinLeftGy = PB1, PinLeftAx = PB0, PinLeftAz = PA7; //VCC = PB2, GND = PA6
const int PinRightGy = PA4, PinRightAx = PA3, PinRightAz = PA2; //VCC = PA5, GND = PA1


// MATH CALC
const float Rad2Deg = 57.2957795;
extern float dt;


// LPF COEFF
const float a1 = 1.64746;
const float a2 = 0.70090;    
const float b0 = 0.01336;   
const float b1 = 0.02672;    
const float b2 = 0.01336;


// SIGNAL PROCESSING
extern float Ylpf[6], Ylpf_1[6],Ylpf_2[6];
extern float sinyal[6], sinyal_1[6], sinyal_2[6]; // sinyal[0-6] =  GyL, AxL, AzL, GyR, AxR, AzR 
extern float Accoffset, Accsens, GyLoffset, GyRoffset, Gysens;
extern float GyL_rate, AczL_rate, AcxL_rate, GyL_angle;// digunakan untuk integrasi sudut
extern float GyR_rate, AczR_rate, AcxR_rate, GyR_angle;// digunakan untuk integrasi sudut
extern long sumGyL, sumGyR; 
extern float Tilt_angleL, Tilt_angleR;


//  KALMAN FILTER
extern float TiltKalL, TiltKalR; 
extern float BiasL, BiasR;
extern float GyL_stable, GyR_stable;
// ERROR COVARIANCE MATRIX (P)
extern float P1L, P2L, P3L, P4L;
extern float P1R, P2R, P3R, P4R;
// TUNING PARAMETER
extern float QangleL, QgyroL, RangleL;    
extern float QangleR, QgyroR, RangleR;    
extern float ErrEstmtL, K0L, K1L, ErrTiltL;
extern float ErrEstmtR, K0R, K1R, ErrTiltR;


// PEAK DETECTION
//  BUFFER FOR GYRO
extern float buffGyL[3], buffGyR[3];
extern float buffAngleL[3], buffAngleR[3];
// STATE MACHINE
extern int gaitState;
// STATIC THRESHOLD
extern float thresMS, thresIC, thresEC;

#endif CONFIG_H