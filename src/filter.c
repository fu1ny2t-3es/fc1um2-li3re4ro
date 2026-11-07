#include "fceu-types.h"

#include "sound.h"
#include "x6502.h"
#include "fceu.h"
#include "filter.h"

#include "fcoeffs.h"

<<<<<<< HEAD
/* Reference rate at which SexyFilter2's one-pole reproduces the legacy
 * hardcoded ">>3" coefficient (1/8 == 8192 in Q16).  The coefficient is
 * scaled by SEXYFILTER2_REF_RATE / SndRate so the corner frequency stays
 * fixed in Hz across every output rate the core selects, instead of
 * drifting with the rate as the old fixed shift did.  Changing this value
 * retunes the corner (it sits at ~0.0199 * SEXYFILTER2_REF_RATE Hz, i.e.
 * ~955 Hz at 48000). */
#define SEXYFILTER2_REF_RATE 48000

=======
<<<<<<< HEAD
>>>>>>> 52db67b5 (Update libretro_core_options.h)
static uint32_t mrindex;
static uint32_t mrratio;

<<<<<<< HEAD
<<<<<<< HEAD
int64_t sexyfilter_acc1 = 0, sexyfilter_acc2 = 0;

/* SexyFilter2's separate accumulator. Like sexyfilter_acc1/_acc2 it
 * was a function-local static, which meant FCEUSND_Power couldn't
 * reset it - cart B in a long-running process would inherit cart A's
 * lowpass state. Lifted to file scope alongside the others so
 * SexyFilter_Reset can zero it too. */
int64_t sexyfilter2_acc = 0;

void SexyFilter_Reset(void)
{
   sexyfilter_acc1 = 0;
   sexyfilter_acc2 = 0;
   sexyfilter2_acc = 0;
   /* mrindex is reset in MakeFilters, which FCEUI_Sound calls on
    * every cart load, so it doesn't need to be reset here. */
}
=======
=======
int64 sexyfilter_acc1 = 0, sexyfilter_acc2 = 0;
=======
static int32 sq2coeffs[SQ2NCOEFFS];
static int32 coeffs[NCOEFFS];

static uint32 mrindex;
static uint32 mrratio;

enum {
	OUTPUT_MAX = 32767,
	OUTPUT_MIN = - 32768
};

static INLINE int32 Clamp(int32 value) {
	return (value <= OUTPUT_MAX) ? ((value >= OUTPUT_MIN) ? value : OUTPUT_MIN) : OUTPUT_MAX;
}
>>>>>>> f6dccad9 (Update libretro_core_options.h)

>>>>>>> 3ef6de2c (Refactors)
void SexyFilter2(int32 *in, int32 count) {
#ifdef moo
	static int64 acc = 0;
	double x, p;
	int64 c;

	x = 2 * M_PI * 6000 / FSettings.SndRate;
	p = ((double)2 - cos(x)) - sqrt(pow((double)2 - cos(x), 2) - 1);

	c = p * 0x100000;
#endif
	static int64 acc = 0;
>>>>>>> 3cd7a223 (Update libretro_core_options.h)

void SexyFilter2(int32_t *in, int32_t count) {
	int64_t  coeff;
	uint32_t rate = FSettings.SndRate ? FSettings.SndRate
	                                  : SEXYFILTER2_REF_RATE;

	/* Derive the one-pole coefficient from the output rate so the corner
	 * frequency is rate-invariant.  coeff == 8192 (the legacy 1/8) exactly
	 * at rate == SEXYFILTER2_REF_RATE. */
	coeff = ((int64_t)8192 * SEXYFILTER2_REF_RATE + (rate >> 1)) / rate;

	/* A stable one-pole requires 0 < coeff <= 65536 (65536 == passthrough).
	 * The rates this core offers (>= 32 kHz) keep coeff in ~4096..12288;
	 * clamp defensively so an unusual rate can never push the pole onto or
	 * outside the unit circle. */
	if (coeff < 1)     coeff = 1;
	if (coeff > 65536) coeff = 65536;

	while (count--) {
<<<<<<< HEAD
		int64_t diff;
=======
<<<<<<< HEAD
>>>>>>> 52db67b5 (Update libretro_core_options.h)
		int64_t dropcurrent;

		diff        = ((int64_t)*in << 16) - sexyfilter2_acc;
		/* Round to nearest instead of truncating toward -inf, so the
		 * feedback path neither accumulates a sub-LSB downward bias nor
		 * settles to a stuck negative offset on silence. */
		dropcurrent = (diff * coeff + (1 << 15)) >> 16;

		sexyfilter2_acc += dropcurrent;
<<<<<<< HEAD
		*in = (int32_t)(sexyfilter2_acc >> 16);
=======
		*in = sexyfilter2_acc >> 16;
=======
		int64 dropcurrent = ((*in << 16) - acc) >> (FSettings.lowpass & 0x03);
		acc += dropcurrent;
		*in = acc >> 16;
>>>>>>> f6dccad9 (Update libretro_core_options.h)
>>>>>>> 52db67b5 (Update libretro_core_options.h)
		in++;
#if 0
		 acc=((int64)0x100000-c)* *in + ((c*acc)>>20);
		*in=acc>>20;
		in++;
#endif
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
void SexyFilter(int32_t *in, int32_t *out, int32_t count) {
	int32_t mul1, mul2, vmul;

	mul1 = (94 << 16) / FSettings.SndRate;
	mul2 = (24 << 16) / FSettings.SndRate;
	vmul = (FSettings.SoundVolume << 16) * 3 / 4 / 100;
=======
int64 sexyfilter_acc1 = 0, sexyfilter_acc2 = 0;

=======
>>>>>>> 3ef6de2c (Refactors)
void SexyFilter(int32 *in, int32 *out, int32 count) {
	int32 mul1 = (94 << 16) / FSettings.SndRate;
	int32 mul2 = (24 << 16) / FSettings.SndRate;
	int32 vmul = (FSettings.SoundVolume << 16) * 3 / 4 / 100;
>>>>>>> 3cd7a223 (Update libretro_core_options.h)
=======
int64 sexyfilter_acc1 = 0, sexyfilter_acc2 = 0;
>>>>>>> f6dccad9 (Update libretro_core_options.h)

void SexyFilter(int32 *in, int32 *out, int32 count) {
	int32 mul1, mul2, vmul;

	mul1 = (94 << 16) / FSettings.SndRate;
	mul2 = (24 << 16) / FSettings.SndRate;
	vmul = (FSettings.volume[SND_MASTER] << 16) * 3 / 4 / 100;

	if (FSettings.soundq) {
		vmul /= 4;
	} else {
		vmul *= 2; /* TODO:  Increase volume in low quality sound rendering code itself */
	}

	while (count) {
<<<<<<< HEAD
		int64_t ino = (int64_t) * in * vmul;
		/* Round to nearest in the recursive updates and in the output
		 * quantisation rather than truncating toward -inf.  This removes
		 * the systematic sub-LSB downward bias the floor shifts impart to
		 * the feedback state.  mul1/mul2 (the pole placement) are
		 * untouched, so the filter's frequency response/voicing is
		 * identical; only the quantisation noise floor is tightened. */
		sexyfilter_acc1 += ((ino - sexyfilter_acc1) * mul1 + (1 << 15)) >> 16;
		sexyfilter_acc2 += ((ino - sexyfilter_acc1 - sexyfilter_acc2) * mul2 + (1 << 15)) >> 16;
		/* Clearing the source sample is load-bearing only in the LQ call
		 * SexyFilter(Wave, WaveFinal, ...): it resets Wave for the next
		 * frame's accumulation.  In the HQ in-place call (out == in) it is
		 * a dead store immediately overwritten by *out below.  Do NOT
		 * remove it - the LQ accumulation path depends on it. */
		*in = 0;
		{
			int32_t t = (int32_t)((sexyfilter_acc1 - ino + sexyfilter_acc2
			            + (1 << 15)) >> 16);
			if (t > 32767) t = 32767;
			if (t < -32768) t = -32768;
			*out = t;
		}
=======
		int64 ino = (int64)*in * vmul;
		int32 t;

		sexyfilter_acc1 += ((ino - sexyfilter_acc1) * mul1) >> 16;
		sexyfilter_acc2 += ((ino - sexyfilter_acc1 - sexyfilter_acc2) * mul2) >> 16;
		*in = 0;
		t = (sexyfilter_acc1 - ino + sexyfilter_acc2) >> 16;
		*out = Clamp(t);
>>>>>>> f6dccad9 (Update libretro_core_options.h)
		in++;
		out++;
		count--;
	}
}

/* Returns number of samples written to out. */
/* leftover is set to the number of samples that need to be copied
    from the end of in to the beginning of in.
*/

/* static uint32_t mva=1000; */

/* This filtering code assumes that almost all input values stay below 32767.
    Do not adjust the volume in the wlookup tables and the expansion sound
    code to be higher, or you *might* overflow the FIR code.
*/

int32_t NeoFilterSound(int32_t *in, int32_t *out, uint32_t inlen, int32_t *leftover) {
	uint32_t x;
	int32_t *outsave = out;
	int32_t count = 0;
	uint32_t max = (inlen - 1) << 16;

	/* Both resample paths below round the final >> (16 + 11) to nearest
	 * (the + (1 << 26) term == half an output LSB) rather than flooring
	 * toward -inf.  Flooring here left a systematic ~-0.6 LSB downward
	 * bias on the output; rounding removes the bulk of it and roughly
	 * halves the quantisation error, matching the round-to-nearest the
	 * SexyFilter/SexyFilter2 stages already apply.  The tap loop and its
	 * per-tap >> 6 (which is int32/SIMD-friendly and overflow-bounded)
	 * are deliberately left unchanged: a full int64 accumulation removes
	 * the remaining ~0.1 LSB but regresses this hot loop >20%. */
	if (FSettings.soundq == 2) {
		for (x = mrindex; x < max; x += mrratio) {
			int32_t acc = 0, acc2 = 0;
			uint32_t c;
			int32_t *S, *D;

			for (c = SQ2NCOEFFS, S = &in [(x >> 16) - SQ2NCOEFFS], D = sq2coeffs; c; c--, D++) {
				acc += (S[c] * *D) >> 6;
				acc2 += (S[1 + c] * *D) >> 6;
			}

			acc = ((int64_t)acc * (65536 - (x & 65535)) + (int64_t)acc2 * (x & 65535)
			       + (1 << 26)) >> (16 + 11);
			*out = acc;
			out++;
			count++;
		}
	} else {
		for (x = mrindex; x < max; x += mrratio) {
			int32_t acc = 0, acc2 = 0;
			uint32_t c;
			int32_t *S, *D;

			for (c = NCOEFFS, S = &in [(x >> 16) - NCOEFFS], D = coeffs; c; c--, D++) {
				acc += (S[c] * *D) >> 6;
				acc2 += (S[1 + c] * *D) >> 6;
			}

			acc = ((int64_t)acc * (65536 - (x & 65535)) + (int64_t)acc2 * (x & 65535)
			       + (1 << 26)) >> (16 + 11);
			*out = acc;
			out++;
			count++;
		}
	}

	mrindex = x - max;

	if (FSettings.soundq == 2) {
		mrindex += SQ2NCOEFFS * 65536;
		*leftover = SQ2NCOEFFS + 1;
	} else {
		mrindex += NCOEFFS * 65536;
		*leftover = NCOEFFS + 1;
	}

	for (x = 0; x < GAMEEXPSOUND_COUNT; x++) {
		if (GameExpSound[x].NeoFill) {
			GameExpSound[x].NeoFill(outsave, count);
		}
	}

	SexyFilter(outsave, outsave, count);
	if (FSettings.lowpass) {
		SexyFilter2(outsave, count);
	}
	return (count);
}

<<<<<<< HEAD
void MakeFilters(int32_t rate) {
<<<<<<< HEAD
	int32_t *tabs[8] = { C44100NTSC, C44100PAL, C48000NTSC, C48000PAL, C96000NTSC,
					   C96000PAL, C32000NTSC, C32000PAL };
	int32_t *sq2tabs[8] = { SQ2C44100NTSC, SQ2C44100PAL, SQ2C48000NTSC, SQ2C48000PAL,
						  SQ2C96000NTSC, SQ2C96000PAL, SQ2C32000NTSC, SQ2C32000PAL };
=======
	int32_t *tabs[6] = { C44100NTSC, C44100PAL, C48000NTSC, C48000PAL, C96000NTSC,
					   C96000PAL };
	int32_t *sq2tabs[6] = { SQ2C44100NTSC, SQ2C44100PAL, SQ2C48000NTSC, SQ2C48000PAL,
						  SQ2C96000NTSC, SQ2C96000PAL };
<<<<<<< HEAD
>>>>>>> 3b1da35c (Update libretro_core_options.h)

	int32_t *tmp;
	int32_t x;
	uint32_t nco;
<<<<<<< HEAD
	uint32_t idx;
=======
=======
=======
void MakeFilters(int32 rate) {
	int32 *tabs[6] = { C44100NTSC, C44100PAL, C48000NTSC, C48000PAL, C96000NTSC, C96000PAL };
	int32 *sq2tabs[6] = { SQ2C44100NTSC, SQ2C44100PAL, SQ2C48000NTSC, SQ2C48000PAL, SQ2C96000NTSC, SQ2C96000PAL };

	int32 *tmp;
>>>>>>> f6dccad9 (Update libretro_core_options.h)
	int32 x;
	uint32 nco;
>>>>>>> 3cd7a223 (Update libretro_core_options.h)
>>>>>>> 3b1da35c (Update libretro_core_options.h)

	if (FSettings.soundq == 2) {
		nco = SQ2NCOEFFS;
	} else {
		nco = NCOEFFS;
	}

	mrindex = (nco + 1) << 16;
<<<<<<< HEAD
	mrratio = (PAL ? (int64_t)(PAL_CPU * 65536) : (int64_t)(NTSC_CPU * 65536)) / rate;
=======
	mrratio = (isPAL ? (int64)(PAL_CPU * 65536) : (int64)(NTSC_CPU * 65536)) / rate;
>>>>>>> c7dde5ce (Update libretro_core_options.h)

<<<<<<< HEAD
	/* Select the coefficient table matched to the output rate. Bit 0 is
	 * the PAL/NTSC region; the rate selects the base index. Rates without
	 * a dedicated table fall back to the 44100 design (index 0/1). */
	if (rate == 48000)
		idx = 2;
	else if (rate == 96000)
		idx = 4;
	else if (rate == 32000)
		idx = 6;
	else /* 44100 and any other rate */
		idx = 0;
	idx |= (PAL ? 1 : 0);

	if (FSettings.soundq == 2)
<<<<<<< HEAD
		tmp = sq2tabs[idx];
	else
		tmp = tabs[idx];

	if (FSettings.soundq == 2)
=======
	{
<<<<<<< HEAD
		int32 *tmp = sq2tabs[(PAL ? 1 : 0) | (rate == 48000 ? 2 : 0) | (rate == 96000 ? 4 : 0)];
>>>>>>> 3b1da35c (Update libretro_core_options.h)
=======
		int32 *tmp = sq2tabs[(isPAL ? 1 : 0) | (rate == 48000 ? 2 : 0) | (rate == 96000 ? 4 : 0)];
>>>>>>> 6a15e475 (Update libretro_core_options.h)
		for (x = 0; x < (SQ2NCOEFFS >> 1); x++)
			sq2coeffs[x] = sq2coeffs[SQ2NCOEFFS - 1 - x] = tmp[x];
=======
	if (FSettings.soundq == 2) {
		tmp = sq2tabs[(isPAL ? 1 : 0) | (rate == 48000 ? 2 : 0) | (rate >= 96000 ? 4 : 0)];
	} else {
		tmp = tabs[(isPAL ? 1 : 0) | (rate == 48000 ? 2 : 0) | (rate >= 96000 ? 4 : 0)];
>>>>>>> cd3c2d68 (Update libretro_core_options.h)
	}

	if (FSettings.soundq == 2) {
		for (x = 0; x < (SQ2NCOEFFS >> 1); x++) {
			sq2coeffs[x] = sq2coeffs[SQ2NCOEFFS - 1 - x] = tmp[x];
		}
	} else {
		for (x = 0; x < (NCOEFFS >> 1); x++) {
			coeffs[x] = coeffs[NCOEFFS - 1 - x] = tmp[x];
		}
	}
}
