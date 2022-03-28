# SPO-sound-sampling

These are the six programs that are implement different sound volume scaling algorithms:
1. vol0.c -  is the basic or naive algorithm. This approach multiplies each sound sample by the volume scaling factor, casting from signed 16-bit integer to floating point and back again. Casting between an integer and a floating point can be an expensive operation.
2. vol1.c does the math using fixed-point calculations. This avoids the overhead of casting between an integer and a floating point and back again.
3. vol2.c pre-calculates all 65536 different results and then looks up the answer for each input value.
4. vol4.c uses Single Instruction, Multiple Data (SIMD) instructions accessed through inline assembly (assembly language code inserted into a C program). This program is specific to the AArch64 architecture and will not build for x86_64.
5. vol5.c uses SIMD instructions accessed through Compiler Intrinsics. This program is specific to AArch64.
6. vol6.c uses SVE2 SIMD instructions accessed through Compiler Intrinsics. This program is specific to AArch64.
