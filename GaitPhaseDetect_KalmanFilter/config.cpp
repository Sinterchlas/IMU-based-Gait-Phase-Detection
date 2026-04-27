#include "config.h"

float Ylpf[6] = {0}, Ylpf_1[6]={0}, Ylpf_2[6]={0};
float sinyal[6] = {0}, sinyal_1[6]={0}, sinyal_2[6]={0};

float GyL_rate = 0.0, AcxL_rate = 0.0, AczL_rate = 0.0; 
float GyR_rate = 0.0, AcxR_rate = 0.0, AczR_rate = 0.0; 
float GyL_angle = 0.0, GyR_angle = 0.0;
float dt =0.0;
float Accoffset = 2048, GyLoffset = 0.0, GyRoffset = 0.0;
float Accsens = 409.6, Gysens = 0.8136;
float Tilt_angleL = 0.0, Tilt_angleR = 0.0;


float TiltKalL = 0.0, TiltKalR = 0.0;     // Hasil akhir sudut yang difilter
float BiasL = 0.0, BiasR = 0.0;        // Estimasi bias giroskop

// Matriks error covariance (P)
float P1L = 0.1, P2L = 0.0, P3L = 0.0, P4L = 0.1;
float P1R = 0.1, P2R = 0.0, P3R = 0.0, P4R = 0.1;

// Parameter tuning Kalman Filter
// Anda bisa mengubah nilai Q dan R ini untuk menyesuaikan sensitivitas
float QangleL = 0.001;    // Variance process noise dari accelerometer
float QgyroL = 0.003;     // Variance process noise dari gyroscope
float RangleL = 10;     // Variance measurement noise
float ErrEstmtL = 0, K0L = 0, K1L = 0, ErrTiltL = 0;

float QangleR = 0.001;    // Variance process noise dari accelerometer
float QgyroR = 0.003;     // Variance process noise dari gyroscope
float RangleR = 10;     // Variance measurement noise
float ErrEstmtR = 0, K0R = 0, K1R = 0, ErrTiltR = 0;
