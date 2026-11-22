/* The NTSC filter's intrinsics header must be included before
 * nes_ntsc_impl.h defines restrict: MSVC's malloc.h, pulled in by
 * emmintrin.h, uses __declspec(restrict) and fails with C2485 when
 * restrict expands to __restrict. The stand-in headers in
 * tests/restrict_order stop the build with #error if restrict is a
 * macro when the intrinsics header is reached, so this holds on gcc
 * and clang too. Builds and passes trivially without a vector arm.
 *
 *   cc -std=c89 -O2 -Itests/restrict_order -Isrc/ntsc \
 *      tests/ntsc_restrict_order.c -o ntsc_restrict_order -lm && ./ntsc_restrict_order
 * MSYS2 MINGW64: the same command, running ./ntsc_restrict_order.exe. */
#include "../src/ntsc/nes_ntsc.c"

#include <stdio.h>

int main(void)
{
#ifdef NES_NTSC_SIMD
   printf("ntsc_restrict_order: intrinsics header included before restrict\n");
#else
   printf("ntsc_restrict_order: no vector arm for this target\n");
#endif
   return 0;
}
