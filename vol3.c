#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vol.h"

// The algorithm below uses ARM SIMD assembly instructions accesed through inline assembly (specific to AArch64) 
int main() {
#ifndef __aarch64__
    printf("Wrong architecture - written for aarch64 only.\n");
#else
    int16_t* in_cursor;    // input cursor
    int16_t* out_cursor;   // output cursor
    int16_t* limit;        // end of input array

    int16_t* in  	 = (int16_t*)calloc(SAMPLES, sizeof(int16_t));
    int16_t* out 	 = (int16_t*)calloc(SAMPLES, sizeof(int16_t));
    int16_t  vol_int     = (int16_t)(VOLUME / 100.0 * 32767.0);
    int      ttl 	 = 0;

    vol_createsample(in, SAMPLES);

    in_cursor  = in;
    out_cursor = out;
    limit      = in + SAMPLES;

    double time_spent = 0.0;
    clock_t begin = clock(); // Time in

    __asm__("dup v1.8h,%w0" ::"r"(vol_int));  // duplicate vol_int into v1.8h

    while (in_cursor < limit) {
        __asm__(
            "ldr q0, [%[in_cursor]], #16 	\n\t"
            "sqrdmulh v0.8h, v0.8h, v1.8h	\n\t"
            "str q0, [%[out_cursor]],#16	\n\t"

            : [in_cursor] "+r"(in_cursor), [out_cursor] "+r"(out_cursor)
            : "r"(in_cursor), "r"(out_cursor)
            : "memory");
    }
    clock_t end = clock(); // Time out

    for (size_t i = 0; i < SAMPLES; i++) {
        ttl = (ttl + out[i]) % 1000;
    }

    printf("Result: %d\n", ttl); // Check sum to prevent compiler optimizations

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
#endif

    return 0;
}
