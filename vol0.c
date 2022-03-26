#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vol.h"

// Naive approach
int16_t scale_sample(int16_t sample, int volume) {
    return (int16_t)((float)(volume / 100.0) * (float)sample);
}

int main() {
    int16_t* in  = (int16_t*)calloc(SAMPLES, sizeof(int16_t));
    int16_t* out = (int16_t*)calloc(SAMPLES, sizeof(int16_t));
    int      ttl = 0;

    vol_createsample(in, SAMPLES);

    double time_spent = 0.0;
    clock_t begin = clock();  // Time in

    for (size_t i = 0; i < SAMPLES; i++) {
        out[i] = scale_sample(in[i], VOLUME);
    }

    clock_t end = clock();  // Time out

    for (size_t i = 0; i < SAMPLES; i++) {
        ttl = (ttl + out[i]) % 1000;
    }

    printf("Result: %d\n", ttl);  // Check sum to prevent compiler optimizations

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);

    return 0;
}
