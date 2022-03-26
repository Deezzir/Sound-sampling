#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vol.h"

// The algorithm below precalculates all 65536 different results 
// and then looks up the answer for each input value
int main() {
    int16_t* in  = (int16_t*)calloc(SAMPLES, sizeof(int16_t));
    int16_t* out = (int16_t*)calloc(SAMPLES, sizeof(int16_t));
    int      ttl = 0;

    static int16_t* precalc;

    vol_createsample(in, SAMPLES);

    double time_spent = 0.0;
    clock_t begin = clock(); // Time in

    precalc = (int16_t*) calloc(65536, 2);
    if (precalc == NULL) {
        printf("malloc failed!\n");
        return 1;
    }

    for (size_t i = -32768; i <= 32767; i++) {
        precalc[(uint16_t)i] = (int16_t)((float)i * VOLUME / 100.0);
    }

    for (size_t i = 0; i < SAMPLES; i++) {
        out[i] = precalc[(uint16_t)in[i]];
    }

    clock_t end = clock(); //Time out

    for (size_t i = 0; i < SAMPLES; i++) {
        ttl = (ttl + out[i]) % 1000;
    }

    printf("Result: %d\n", ttl); // Check sum to prevent compiler optimizations

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);

    return 0;
}
