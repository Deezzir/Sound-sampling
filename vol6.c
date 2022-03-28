#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#ifdef __aarch64__
#include <arm_sve.h>
#endif

#include "vol.h"

// The algorithm below uses ARM SVE2 assembly instructions accesed through GCC compiler intrinsics (specific to AArch64) 
int main(void) {
#ifndef __aarch64__
	printd("Wrong architecture - written for aarch64 only.\n");
#else
	int16_t* in	 = (int16_t*) calloc(SAMPLES, sizeof(int16_t));
	int16_t* out     = (int16_t*) calloc(SAMPLES, sizeof(int16_t));
	int16_t  vol_int = (int16_t)  (VOLUME/100.0 * 32767.0);
	int      ttl     = 0;

	vol_createsample(in, SAMPLES);

	int32_t ix  = 0;
	svbool_t pd = svwhilelt_b16(ix, SAMPLES);
	do {
		svint16_t in_vec = svld1_s16(pd, &in[ix]);
		svst1_s16(pd, &out[ix], svqrdmulh_n_s16(in_vec, vol_int));

		ix += svcntd();
		pd = svwhilelt_b16(ix, SAMPLES);
	} while(svptest_any(svptrue_b16(), pd));

	for (size_t i = 0; i < SAMPLES; i++) {
        	ttl = (ttl + out[i]) % 1000;
    	}

    	printf("Result: %d\n", ttl);  // Check sum to prevent compiler optimizations
#endif
    	return 0;
}
