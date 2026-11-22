/* Stand-in for the compiler's <emmintrin.h>, placed ahead of it on the include
 * path by tests/ntsc_restrict_order.c. nes_ntsc_impl.h defines restrict;
 * MSVC's malloc.h, which its intrinsics headers include, spells
 * __declspec(restrict), so restrict must not be a macro here. */
#ifdef restrict
#error restrict is a macro when the intrinsics header is included
#endif
#include_next <emmintrin.h>
