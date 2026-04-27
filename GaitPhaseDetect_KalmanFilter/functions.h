#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <Arduino.h>

void lpfsensors();
void readsensors();
void printraw();
float cal_sensorconvert(float rawADC, float ADCoffset, float ADCsens);
void Gy_caliberation();

#endif FUNCTIONS_H