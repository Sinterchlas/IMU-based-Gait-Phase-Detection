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

// --- Variabel Kalman Filter ---
float TiltKal = 0.0;     // Hasil akhir sudut yang difilter
float Bias = 0.0;        // Estimasi bias giroskop

// Matriks error covariance (P)
float P1 = 0.1, P2 = 0.0, P3 = 0.0, P4 = 0.1;

// Parameter tuning Kalman Filter
// Anda bisa mengubah nilai Q dan R ini untuk menyesuaikan sensitivitas
float Qangle = 0.001;    // Variance process noise dari accelerometer
float Qgyro = 0.003;     // Variance process noise dari gyroscope
float Rangle = 10;     // Variance measurement noise
float ErrEstmt = 0, K0 = 0, K1 = 0, ErrTilt = 0;
