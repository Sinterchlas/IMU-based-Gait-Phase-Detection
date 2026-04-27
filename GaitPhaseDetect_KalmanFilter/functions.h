#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <Arduino.h>

void lpfsensors();
void readsensors();
void sensorRate();
void sensorCalc();
void printraw();
float cal_sensorconvert(float rawADC, float ADCoffset, float ADCsens);
void Gy_caliberation();
void kalmanfilter();

#endif FUNCTIONS_H