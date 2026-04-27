#include "config.h"

/*float rawthighAx=0.0;
float rawthighGy=0.0;
float rawthighAz=0.0;*/
float Ylpf[3] = {0}, Ylpf_1[3]={0},Ylpf_2[3]={0};
float sinyal[3] = {0}, sinyal_1[3]={0}, sinyal_2[3]={0};

float Gy_rate = 0.0, Acx_rate = 0.0, Acz_rate = 0.0; 
float Gy_angle = 0.0;
float dt =0.0;
float Accoffset = 2048, Gyoffset = 0.0;
float Accsens = 409.6, Gysens = 0.8136;
 float Tilt_angle = 0.0;

float alpha = 0.98, Angle = 0.0;