#ifndef _X6502STRUCTH
#define _X6502STRUCTH

typedef struct __X6502 {
	int32_t tcount;		/* Temporary cycle counter */
	uint16_t PC;			/* I'll change this to uint32_t later... */
						/* I'll need to AND PC after increments to 0xFFFF */
						/* when I do, though.  Perhaps an IPC() macro? */
	uint8_t A, X, Y, S, P, mooPI;
	uint8_t jammed;

	int32_t count;
	uint32_t IRQlow;		/* Simulated IRQ pin held low(or is it high?).
						And other junk hooked on for speed reasons.*/
<<<<<<< HEAD
<<<<<<< HEAD
	uint8_t DB;			/* Data bus "cache" for reads from certain areas */
=======
	uint8 DB;			/* Data bus "cache" for reads from certain areas */
=======
	uint8 openbus;		/* Data bus "cache" for reads from certain areas */
>>>>>>> 0105d2d1 (Update libretro_core_options.h)

	int preexec;		/* Pre-exec'ing for debug breakpoints. */
>>>>>>> d5085b8d (Update libretro_core_options.h)
} X6502;

#endif
