#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// to generate more cycle, increase cycle number
// then duplicate the input based on cycle number
// for example : in_a[] = {Va[0], .... , Va[DATA_LENGTH - 1],
// Va[0], .... , Va[DATA_LENGTH - 1],
// . . .};
#define CYCLE 1
#define DATA_LENGTH 20

float Va[] = {
    156.63, 246.59, 294.72, 305.51, 300.66,
    268.03, 204.18, 125.41, 42.954, -48.322,
    -154.08, -243.95, -293.12, -303.09, -297.98,
    -264.13, -202.1, -122.25, -39.893, 51.818
};
float Vb[] = {
    -308.4, -280.19, -240.66, -186.6, -99.744,
    -0.54547, 92.853, 181.46, 262.05, 312.39,
    311.44, 283.76, 245.04, 188.62, 102.16,
    2.9662, -89.395, -176.17, -259.16, -309.96
};
float Vc[] = {
    156.11, 82.694, -21.783, -128.37, -213.06,
    -269.49, -309.58, -313.4, -273.73, -214.81,
    -154.29, -79.64, 24.679, 132.16, 216.63,
    274.14, 311.11, 315.76, 276.27, 216.22
};

typedef struct _DDATA{
float *in_a;
float *in_b;
float *in_c;
float *F_est;
float *Theta_est;
float *Harmonics;
float Ts;  // sampling time
float Kc1; // Kc are controller gains
float Kc2; // choose your controller and
float Kc3; // gains accordingly to get satisfied result
}DDATA;

DDATA ddata = {
.in_a = &Va,
.in_b = &Vb,
.in_c = &Vc,
.Ts = 0.001,
};

void estimateFrequencyAndTheta(DDATA *d, int dataSize){
    /*
     * Method:
     * - to find a sinusoidal frequency, we can find when 0 is crossed
     * - 1 periodic happens when 0 is crossed 2 times
     */
    float zeroCrossingCount = 0;
    float sumTheta = 0;
    int i;

    /* find out how many times we cross 0 border */
    for(i = 1; i < dataSize; i++){
        if((d->in_a[i-1] * d->in_a[i]) < 0){
            // the data has crossed from positive to negative; vice versa
            zeroCrossingCount++;
        }
    }

    /*
     * frequency formula:
     * number_of_cycle = zeroCrossingCount / 2
     * f = number_of_cycle / total time 
     */ 
    float totalTime = dataSize * d->Ts;
    float frequencyEstimate = (zeroCrossingCount / 2) / (totalTime);

    /*
     * setimating phase angle
     * found a way in internet. I will develop it further
     */
    float thetaEstimate = atan(d->in_a[0] / d->in_b[0]);

    // Store the estimates
    *(d->F_est) = frequencyEstimate;
    *(d->Theta_est) = thetaEstimate;

    return;
}

void getHarmonicAmplitudes(DDATA *d, int dataSize){
    // I have no idea
    return;
}

int main()
{
    int i = 0;

    ddata.F_est = malloc(sizeof(float));
    ddata.Theta_est = malloc(sizeof(float));
    ddata.Harmonics = malloc(sizeof(float));

    estimateFrequencyAndTheta(&ddata, DATA_LENGTH * CYCLE);
    getHarmonicAmplitudes(&ddata, DATA_LENGTH * CYCLE);

    printf("estimated frequency : %.2f\n", *(ddata.F_est));
    printf("estimated Theta     : %.2f\n", *(ddata.Theta_est));

    printf("done\n");

    free(ddata.F_est);
    free(ddata.Theta_est);
    free(ddata.Harmonics);

    return 0;
}