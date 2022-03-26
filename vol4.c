// vol_intrinsics.c :: volume scaling in C using AArch64 Intrinsics
// Chris Tyler 2017.11.29-2021.11.16 - Licensed under GPLv3.
// For the Seneca College SPO600 Course

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __aarch64__
#include <arm_neon.h>
#endif

#include "vol.h"

// The algorithm below uses ARM SIMD assembly instructions accesed through GCC compiler intrinsics (specific to AArch64) 
int main() {
#ifndef __aarch64__
    printf("Wrong architecture - written for aarch64 only.\n");
#else
    register int16_t* in_cursor  asm("r20");  // input cursor (pointer)
    register int16_t* out_cursor asm("r21");  // output cursor (pointer)
    register int16_t vol_int     asm("r22");  // volume as int16_t
    int16_t* limit;  

    int16_t* in  = (int16_t*)calloc(SAMPLES, sizeof(int16_t));
    int16_t* out = (int16_t*)calloc(SAMPLES, sizeof(int16_t));
	int      ttl = 0;

    vol_int  = (int16_t)(VOLUME / 100.0 * 32767.0);

    vol_createsample(in, SAMPLES);

    in_cursor  = in;
    out_cursor = out;
    limit      = in + SAMPLES;

    double time_spent = 0.0;
    clock_t begin = clock(); //Time in

    while (in_cursor < limit) {
        vst1q_s16(out_cursor, vqrdmulhq_s16(vld1q_s16(in_cursor), vdupq_n_s16(vol_int)));
        in_cursor += 8;
        out_cursor += 8;
    }
    
    clock_t end = clock(); // Time out

    for (size_t i = 0; i < SAMPLES; i++) {
        ttl = (ttl + out[i]) % 1000;
    }

    printf("Result: %d\n", ttl);  // Check sum to prevent compiler optimizations

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
#endif

    return 0;
}
