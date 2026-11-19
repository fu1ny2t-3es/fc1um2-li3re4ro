/**
 * emu2413 v1.5.9
 * https://github.com/digital-sound-antiques/emu2413
 * Copyright (C) 2020 Mitsutaka Okazaki
 *
 * This source refers to the following documents. The author would like to thank all the authors who have
 * contributed to the writing of them.
 * - [YM2413 notes](http://www.smspower.org/Development/YM2413) by andete
 * - ymf262.c by Jarek Burczynski
 * - [VRC7 presets](https://siliconpr0n.org/archive/doku.php?id=vendor:yamaha:opl2#opll_vrc7_patch_format) by Nuke.YKT
 * - YMF281B presets by Chabin
 */
#include "emu2413.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef INLINE
#if defined(_MSC_VER)
#define INLINE __inline
#elif defined(__GNUC__)
#define INLINE __inline__
#else
#define INLINE inline
#endif
#endif

#define _PI_ 3.14159265358979323846264338327950288

#define OPLL_TONE_NUM 3
/* clang-format off */
static uint8_t default_inst[OPLL_TONE_NUM][(16 + 3) * 8] = {{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /* 0: User */
0x71,0x61,0x1e,0x17,0xd0,0x78,0x00,0x17, /* 1: Violin */
0x13,0x41,0x1a,0x0d,0xd8,0xf7,0x23,0x13, /* 2: Guitar */
0x13,0x01,0x99,0x00,0xf2,0xc4,0x21,0x23, /* 3: Piano */
0x11,0x61,0x0e,0x07,0x8d,0x64,0x70,0x27, /* 4: Flute */
0x32,0x21,0x1e,0x06,0xe1,0x76,0x01,0x28, /* 5: Clarinet */
0x31,0x22,0x16,0x05,0xe0,0x71,0x00,0x18, /* 6: Oboe */
0x21,0x61,0x1d,0x07,0x82,0x81,0x11,0x07, /* 7: Trumpet */
0x33,0x21,0x2d,0x13,0xb0,0x70,0x00,0x07, /* 8: Organ */
0x61,0x61,0x1b,0x06,0x64,0x65,0x10,0x17, /* 9: Horn */
0x41,0x61,0x0b,0x18,0x85,0xf0,0x81,0x07, /* A: Synthesizer */
0x33,0x01,0x83,0x11,0xea,0xef,0x10,0x04, /* B: Harpsichord */
0x17,0xc1,0x24,0x07,0xf8,0xf8,0x22,0x12, /* C: Vibraphone */
0x61,0x50,0x0c,0x05,0xd2,0xf5,0x40,0x42, /* D: Synthsizer Bass */
0x01,0x01,0x55,0x03,0xe9,0x90,0x03,0x02, /* E: Acoustic Bass */
0x41,0x41,0x89,0x03,0xf1,0xe4,0xc0,0x13, /* F: Electric Guitar */
0x01,0x01,0x18,0x0f,0xdf,0xf8,0x6a,0x6d, /* R: Bass Drum (from VRC7) */
0x01,0x01,0x00,0x00,0xc8,0xd8,0xa7,0x68, /* R: High-Hat(M) / Snare Drum(C) (from VRC7) */
0x05,0x01,0x00,0x00,0xf8,0xaa,0x59,0x55, /* R: Tom-tom(M) / Top Cymbal(C) (from VRC7) */
},{
/* VRC7 presets from Nuke.YKT */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x03,0x21,0x05,0x06,0xe8,0x81,0x42,0x27,
0x13,0x41,0x14,0x0d,0xd8,0xf6,0x23,0x12,
0x11,0x11,0x08,0x08,0xfa,0xb2,0x20,0x12,
0x31,0x61,0x0c,0x07,0xa8,0x64,0x61,0x27,
0x32,0x21,0x1e,0x06,0xe1,0x76,0x01,0x28,
0x02,0x01,0x06,0x00,0xa3,0xe2,0xf4,0xf4,
0x21,0x61,0x1d,0x07,0x82,0x81,0x11,0x07,
0x23,0x21,0x22,0x17,0xa2,0x72,0x01,0x17,
0x35,0x11,0x25,0x00,0x40,0x73,0x72,0x01,
0xb5,0x01,0x0f,0x0F,0xa8,0xa5,0x51,0x02,
0x17,0xc1,0x24,0x07,0xf8,0xf8,0x22,0x12,
0x71,0x23,0x11,0x06,0x65,0x74,0x18,0x16,
0x01,0x02,0xd3,0x05,0xc9,0x95,0x03,0x02,
0x61,0x63,0x0c,0x00,0x94,0xC0,0x33,0xf6,
0x21,0x72,0x0d,0x00,0xc1,0xd5,0x56,0x06,
0x01,0x01,0x18,0x0f,0xdf,0xf8,0x6a,0x6d,
0x01,0x01,0x00,0x00,0xc8,0xd8,0xa7,0x68,
0x05,0x01,0x00,0x00,0xf8,0xaa,0x59,0x55,
},{
/* YMF281B presets */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /* 0: User */
0x62,0x21,0x1a,0x07,0xf0,0x6f,0x00,0x16, /* 1: Electric Strings (form Chabin's patch) */
0x40,0x10,0x45,0x00,0xf6,0x83,0x73,0x63, /* 2: Bow Wow (based on plgDavid's patch, KSL fixed) */
0x13,0x01,0x99,0x00,0xf2,0xc3,0x21,0x23, /* 3: Electric Guitar (similar to YM2413 but different DR(C)) */
0x01,0x61,0x0b,0x0f,0xf9,0x64,0x70,0x17, /* 4: Organ (based on Chabin, TL/DR fixed) */
0x32,0x21,0x1e,0x06,0xe1,0x76,0x01,0x28, /* 5: Clarinet (identical to YM2413) */
0x60,0x01,0x82,0x0e,0xf9,0x61,0x20,0x27, /* 6: Saxophone (based on plgDavid, PM/EG fixed) */
0x21,0x61,0x1c,0x07,0x84,0x81,0x11,0x07, /* 7: Trumpet (similar to YM2413 but different TL/DR(M)) */
0x37,0x32,0xc9,0x01,0x66,0x64,0x40,0x28, /* 8: Street Organ (from Chabin) */
0x01,0x21,0x07,0x03,0xa5,0x71,0x51,0x07, /* 9: Synth Brass (based on Chabin, TL fixed) */
0x06,0x01,0x5e,0x07,0xf3,0xf3,0xf6,0x13, /* A: Electric Piano (based on Chabin, DR/RR/KR fixed) */
0x00,0x00,0x18,0x06,0xf5,0xf3,0x20,0x23, /* B: Bass (based on Chabin, EG fixed) */
0x17,0xc1,0x24,0x07,0xf8,0xf8,0x22,0x12, /* C: Vibraphone (identical to YM2413) */
0x35,0x64,0x00,0x00,0xff,0xf3,0x77,0xf5, /* D: Chimes (from plgDavid) */
0x11,0x31,0x00,0x07,0xdd,0xf3,0xff,0xfb, /* E: Tom Tom II (from plgDavid) */
0x3a,0x21,0x00,0x07,0x80,0x84,0x0f,0xf5, /* F: Noise (based on plgDavid, AR fixed) */
0x01,0x01,0x18,0x0f,0xdf,0xf8,0x6a,0x6d, /* R: Bass Drum (identical to YM2413) */
0x01,0x01,0x00,0x00,0xc8,0xd8,0xa7,0x68, /* R: High-Hat(M) / Snare Drum(C) (identical to YM2413) */
0x05,0x01,0x00,0x00,0xf8,0xaa,0x59,0x55, /* R: Tom-tom(M) / Top Cymbal(C) (identical to YM2413) */
}};
/* clang-format on */

/* phase increment counter */
#define DP_BITS 19
#define DP_WIDTH (1 << DP_BITS)
#define DP_BASE_BITS (DP_BITS - PG_BITS)

/* dynamic range of envelope output */
#define EG_STEP 0.375
#define EG_BITS 7
#define EG_MUTE ((1 << EG_BITS) - 1)
#define EG_MAX (EG_MUTE - 4)

/* dynamic range of total level */
#define TL_STEP 0.75
#define TL_BITS 6

/* dynamic range of sustine level */
#define SL_STEP 3.0
#define SL_BITS 4

<<<<<<< HEAD
#define EG2DB(d) ((d) * (int32_t)(EG_STEP / DB_STEP))
#define TL2EG(d) ((d) * (int32_t)(TL_STEP / EG_STEP))
#define SL2EG(d) ((d) * (int32_t)(SL_STEP / EG_STEP))

#define DB_POS(x) (uint32_t)((x) / DB_STEP)
#define DB_NEG(x) (uint32_t)(DB_MUTE + DB_MUTE + (x) / DB_STEP)
=======
/* damper speed before key-on. key-scale affects. */
#define DAMPER_RATE 12

#define TL2EG(d) ((d) << 1)
>>>>>>> f6dccad9 (Update libretro_core_options.h)

/* sine table */
#define PG_BITS 10 /* 2^10 = 1024 length sine table */
#define PG_WIDTH (1 << PG_BITS)

/* clang-format off */
/* exp_table[x] = round((exp2((double)x / 256.0) - 1) * 1024) */
static uint16_t exp_table[256] = {
0,    3,    6,    8,    11,   14,   17,   20,   22,   25,   28,   31,   34,   37,   40,   42,
45,   48,   51,   54,   57,   60,   63,   66,   69,   72,   75,   78,   81,   84,   87,   90,
93,   96,   99,   102,  105,  108,  111,  114,  117,  120,  123,  126,  130,  133,  136,  139,
142,  145,  148,  152,  155,  158,  161,  164,  168,  171,  174,  177,  181,  184,  187,  190,
194,  197,  200,  204,  207,  210,  214,  217,  220,  224,  227,  231,  234,  237,  241,  244,
248,  251,  255,  258,  262,  265,  268,  272,  276,  279,  283,  286,  290,  293,  297,  300,
304,  308,  311,  315,  318,  322,  326,  329,  333,  337,  340,  344,  348,  352,  355,  359,
363,  367,  370,  374,  378,  382,  385,  389,  393,  397,  401,  405,  409,  412,  416,  420,
424,  428,  432,  436,  440,  444,  448,  452,  456,  460,  464,  468,  472,  476,  480,  484,
488,  492,  496,  501,  505,  509,  513,  517,  521,  526,  530,  534,  538,  542,  547,  551,
555,  560,  564,  568,  572,  577,  581,  585,  590,  594,  599,  603,  607,  612,  616,  621,
625,  630,  634,  639,  643,  648,  652,  657,  661,  666,  670,  675,  680,  684,  689,  693,
698,  703,  708,  712,  717,  722,  726,  731,  736,  741,  745,  750,  755,  760,  765,  770,
774,  779,  784,  789,  794,  799,  804,  809,  814,  819,  824,  829,  834,  839,  844,  849,
854,  859,  864,  869,  874,  880,  885,  890,  895,  900,  906,  911,  916,  921,  927,  932,
937,  942,  948,  953,  959,  964,  969,  975,  980,  986,  991,  996, 1002, 1007, 1013, 1018
};
/* fullsin_table[x] = round(-log2(sin((x + 0.5) * PI / (PG_WIDTH / 4) / 2)) * 256) */
static uint16_t fullsin_table[PG_WIDTH] = {
2137, 1731, 1543, 1419, 1326, 1252, 1190, 1137, 1091, 1050, 1013, 979,  949,  920,  894,  869, 
846,  825,  804,  785,  767,  749,  732,  717,  701,  687,  672,  659,  646,  633,  621,  609, 
598,  587,  576,  566,  556,  546,  536,  527,  518,  509,  501,  492,  484,  476,  468,  461,
453,  446,  439,  432,  425,  418,  411,  405,  399,  392,  386,  380,  375,  369,  363,  358,  
352,  347,  341,  336,  331,  326,  321,  316,  311,  307,  302,  297,  293,  289,  284,  280,
276,  271,  267,  263,  259,  255,  251,  248,  244,  240,  236,  233,  229,  226,  222,  219, 
215,  212,  209,  205,  202,  199,  196,  193,  190,  187,  184,  181,  178,  175,  172,  169, 
167,  164,  161,  159,  156,  153,  151,  148,  146,  143,  141,  138,  136,  134,  131,  129,  
127,  125,  122,  120,  118,  116,  114,  112,  110,  108,  106,  104,  102,  100,  98,   96,   
94,   92,   91,   89,   87,   85,   83,   82,   80,   78,   77,   75,   74,   72,   70,   69,
67,   66,   64,   63,   62,   60,   59,   57,   56,   55,   53,   52,   51,   49,   48,   47,  
46,   45,   43,   42,   41,   40,   39,   38,   37,   36,   35,   34,   33,   32,   31,   30,  
29,   28,   27,   26,   25,   24,   23,   23,   22,   21,   20,   20,   19,   18,   17,   17,   
16,   15,   15,   14,   13,   13,   12,   12,   11,   10,   10,   9,    9,    8,    8,    7,    
7,    7,    6,    6,    5,    5,    5,    4,    4,    4,    3,    3,    3,    2,    2,    2,
2,    1,    1,    1,    1,    1,    1,    1,    0,    0,    0,    0,    0,    0,    0,    0,
};
/* clang-format on */

static uint16_t halfsin_table[PG_WIDTH];
static uint16_t *wave_table_map[2] = {fullsin_table, halfsin_table};

/* pitch modulator */
/* offset to fnum, rough approximation of 14 cents depth. */
static int8_t pm_table[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},    /* fnum = 000xxxxxx */
    {0, 0, 1, 0, 0, 0, -1, 0},   /* fnum = 001xxxxxx */
    {0, 1, 2, 1, 0, -1, -2, -1}, /* fnum = 010xxxxxx */
    {0, 1, 3, 1, 0, -1, -3, -1}, /* fnum = 011xxxxxx */
    {0, 2, 4, 2, 0, -2, -4, -2}, /* fnum = 100xxxxxx */
    {0, 2, 5, 2, 0, -2, -5, -2}, /* fnum = 101xxxxxx */
    {0, 3, 6, 3, 0, -3, -6, -3}, /* fnum = 110xxxxxx */
    {0, 3, 7, 3, 0, -3, -7, -3}, /* fnum = 111xxxxxx */
};

/* amplitude lfo table */
/* The following envelop pattern is verified on real YM2413. */
/* each element repeates 64 cycles */
static uint8_t am_table[210] = {
	0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  /* */
	2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  /* */
	4,  4,  4,  4,  4,  4,  4,  4,  5,  5,  5,  5,  5,  5,  5,  5,  /* */
	6,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,  7,  7,  7,  7,  /* */
	8,  8,  8,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,  9,  /* */
	10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, /* */
	12, 12, 12, 12, 12, 12, 12, 12,                                 /* */
	13, 13, 13,                                                     /* */
	12, 12, 12, 12, 12, 12, 12, 12,                                 /* */
	11, 11, 11, 11, 11, 11, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, /* */
	9,  9,  9,  9,  9,  9,  9,  9,  8,  8,  8,  8,  8,  8,  8,  8,  /* */
	7,  7,  7,  7,  7,  7,  7,  7,  6,  6,  6,  6,  6,  6,  6,  6,  /* */
	5,  5,  5,  5,  5,  5,  5,  5,  4,  4,  4,  4,  4,  4,  4,  4,  /* */
	3,  3,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2,  2,  2,  2,  2,  /* */
	1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0
};

/* envelope decay increment step table */
/* based on andete's research */
static uint8_t eg_step_tables[4][8] = {
    {0, 1, 0, 1, 0, 1, 0, 1},
    {0, 1, 0, 1, 1, 1, 0, 1},
    {0, 1, 1, 1, 0, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1},
};

enum __OPLL_EG_STATE { ATTACK, DECAY, SUSTAIN, RELEASE, DAMP, UNKNOWN };

static uint32_t ml_table[16] = {1,     1 * 2, 2 * 2,  3 * 2,  4 * 2,  5 * 2,  6 * 2,  7 * 2,
                                8 * 2, 9 * 2, 10 * 2, 10 * 2, 12 * 2, 12 * 2, 15 * 2, 15 * 2};

#define dB2(x) ((x)*2)
static double kl_table[16] = {dB2(0.000),  dB2(9.000),  dB2(12.000), dB2(13.875), dB2(15.000), dB2(16.125),
                              dB2(16.875), dB2(17.625), dB2(18.000), dB2(18.750), dB2(19.125), dB2(19.500),
                              dB2(19.875), dB2(20.250), dB2(20.625), dB2(21.000)};

<<<<<<< HEAD
/* Adjust envelope speed which depends on sampling rate.
 *
 * Old form: (uint32_t)((double)(x) * clk / 72 / rate + 0.5). Every call site
 * for the phase-increment tables passes a non-negative integer x, so the
 * result is simply round(x*clk/(72*rate)). We compute that in exact 64-bit
 * integer math with round-half-up - no floating point. Verified bit-identical
 * to the old double form for clk=3579545 at every rate the core can select
 * (32000/44100/48000/96000 and the 49716 native passthrough).
 *
 * NOTE: only valid for integer x. The pm/am LFO deltas, whose arguments were
 * fractional doubles, are computed separately in internal_refresh(). */
#define rate_adjust(x) ((rate == 49716) ? (uint32_t)(x) : (uint32_t)(((uint64_t)(x) * clk + 36ULL * rate) / (72ULL * rate)))
=======
static uint32_t tll_table[8 * 16][1 << TL_BITS][4];
static int32_t rks_table[8 * 2][2];

<<<<<<< HEAD
/* Adjust envelope speed which depends on sampling rate. */
#define rate_adjust(x) (rate == 49716 ? (uint32_t)(x) : (uint32_t)((double)(x) * clk / 72 / rate + 0.5))			/* added 0.5 to round the value*/
=======
static OPLL_PATCH null_patch = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static OPLL_PATCH default_patch[OPLL_TONE_NUM][(16 + 3) * 2];
>>>>>>> f6dccad9 (Update libretro_core_options.h)
>>>>>>> 52db67b5 (Update libretro_core_options.h)

/* don't forget min/max is defined as a macro in stdlib.h of Visual C. */
#ifndef min
static INLINE int min(int i, int j) { return (i < j) ? i : j; }
#endif
#ifndef max
static INLINE int max(int i, int j) { return (i > j) ? i : j; }
#endif

/***************************************************

<<<<<<< HEAD
/* Input clock */
static uint32_t clk = 844451141;
/* Sampling rate */
static uint32_t rate = 3354932;

/* Constant (rate/clk-independent) tables that used to be generated at init
 * with pow/log/sin/log10. They are now baked as deterministic const arrays
 * in emu2413_tables.h, so this TU pulls in no libm and produces byte-identical
 * tables on every platform. Provides: fullsintable, halfsintable, pmtable,
 * amtable, DB2LIN_TABLE, AR_ADJUST_TABLE. */
#include "emu2413_tables.h"

static const uint16_t *waveform[2] = { fullsintable, halfsintable };

/* Phase delta for LFO */
static uint32_t pm_dphase;
static uint32_t am_dphase;

<<<<<<< HEAD
/* Definition of envelope mode */
enum
{ SETTLE, ATTACK, DECAY, SUSHOLD, SUSTINE, RELEASE, FINISH };
=======
/* dB to Liner table */
static int16_t DB2LIN_TABLE[(DB_MUTE + DB_MUTE) * 2];

/* Liner to Log curve conversion table (for Attack rate). */
static uint16_t AR_ADJUST_TABLE[1 << EG_BITS];
=======
           Internal Sample Rate Converter

****************************************************/
/* Note: to disable internal rate converter, set clock/72 to output sampling rate. */
>>>>>>> 52db67b5 (Update libretro_core_options.h)

/*
 * LW is truncate length of sinc(x) calculation.
 * Lower LW is faster, higher LW results better quality.
 * LW must be a non-zero positive even number, no upper limit.
 * LW=16 or greater is recommended when upsampling.
 * LW=8 is practically okay for downsampling.
 */
#define LW 16

/* resolution of sinc(x) table. sinc(x) where 0.0<=x<1.0 corresponds to sinc_table[0...SINC_RESO-1] */
#define SINC_RESO 256
#define SINC_AMP_BITS 12

/* double hamming(double x) { return 0.54 - 0.46 * cos(2 * PI * x); } */
static double blackman(double x) { return 0.42 - 0.5 * cos(2 * _PI_ * x) + 0.08 * cos(4 * _PI_ * x); }
static double sinc(double x) { return (x == 0.0 ? 1.0 : sin(_PI_ * x) / (_PI_ * x)); }
static double windowed_sinc(double x) { return blackman(0.5 + 0.5 * x / (LW / 2)) * sinc(x); }

/* f_inp: input frequency. f_out: output frequencey, ch: number of channels */
OPLL_RateConv *OPLL_RateConv_new(double f_inp, double f_out, int ch) {
  OPLL_RateConv *conv = malloc(sizeof(OPLL_RateConv));
  int i;

  conv->ch = ch;
  conv->f_ratio = f_inp / f_out;
  conv->buf = malloc(sizeof(void *) * ch);
  for (i = 0; i < ch; i++) {
    conv->buf[i] = malloc(sizeof(conv->buf[0][0]) * LW);
  }
>>>>>>> f6dccad9 (Update libretro_core_options.h)

  /* create sinc_table for positive 0 <= x < LW/2 */
  conv->sinc_table = malloc(sizeof(conv->sinc_table[0]) * SINC_RESO * LW / 2);
  for (i = 0; i < SINC_RESO * LW / 2; i++) {
    const double x = (double)i / SINC_RESO;
    if (f_out < f_inp) {
      /* for downsampling */
      conv->sinc_table[i] = (int16_t)((1 << SINC_AMP_BITS) * windowed_sinc(x / conv->f_ratio) / conv->f_ratio);
    } else {
      /* for upsampling */
      conv->sinc_table[i] = (int16_t)((1 << SINC_AMP_BITS) * windowed_sinc(x));
    }
  }

<<<<<<< HEAD
/* Phase incr table for Attack */
static uint32_t dphaseARTable[16][16];
/* Phase incr table for Decay and Release */
static uint32_t dphaseDRTable[16][16];

/* KSL + TL Table */
static uint32_t tllTable[16][8][1 << TL_BITS][4];
static int32_t rksTable[2][8][2];

/* Phase incr table for PG */
static uint32_t dphaseTable[512][8][16];
=======
  return conv;
}

static INLINE int16_t lookup_sinc_table(int16_t *table, double x) {
  int16_t index = (int16_t)(x * SINC_RESO);
  if (index < 0)
    index = -index;
  return table[min(SINC_RESO * LW / 2 - 1, index)];
}

void OPLL_RateConv_reset(OPLL_RateConv *conv) {
  int i;
  conv->timer = 0;
  for (i = 0; i < conv->ch; i++) {
    memset(conv->buf[i], 0, sizeof(conv->buf[i][0]) * LW);
  }
}

/* put original data to this converter at f_inp. */
void OPLL_RateConv_putData(OPLL_RateConv *conv, int ch, int16_t data) {
  int16_t *buf = conv->buf[ch];
  int i;
  for (i = 0; i < LW - 1; i++) {
    buf[i] = buf[i + 1];
  }
  buf[LW - 1] = data;
}

/* get resampled data from this converter at f_out. */
/* this function must be called f_out / f_inp times per one putData call. */
int16_t OPLL_RateConv_getData(OPLL_RateConv *conv, int ch) {
  int16_t *buf = conv->buf[ch];
  int32_t sum = 0;
  int k;
  double dn;
  conv->timer += conv->f_ratio;
  dn = conv->timer - floor(conv->timer);
  conv->timer = dn;

  for (k = 0; k < LW; k++) {
    double x = ((double)k - (LW / 2 - 1)) - dn;
    sum += buf[k] * lookup_sinc_table(conv->sinc_table, x);
  }
  return sum >> SINC_AMP_BITS;
}

void OPLL_RateConv_delete(OPLL_RateConv *conv) {
  int i;
  for (i = 0; i < conv->ch; i++) {
    free(conv->buf[i]);
  }
  free(conv->buf);
  free(conv->sinc_table);
  free(conv);
}
>>>>>>> f6dccad9 (Update libretro_core_options.h)

/***************************************************

                  Create tables

****************************************************/
<<<<<<< HEAD
/* AR_ADJUST_TABLE, DB2LIN_TABLE, fullsintable/halfsintable, pmtable and
 * amtable are now baked constants (emu2413_tables.h); the pow/log/sin/log10
 * generators that built them - makeAdjustTable, makeDB2LinTable, lin2db,
 * makeSinTable, makePmTable, makeAmTable and the Min() helper - have been
 * removed. */
=======
<<<<<<< HEAD
INLINE static int32_t Min(int32_t i, int32_t j) {
	if (i < j)
		return i;
	else
		return j;
}

/* Table for AR to LogCurve. */
static void makeAdjustTable(void) {
	int32_t i;

	AR_ADJUST_TABLE[0] = (1 << EG_BITS);
	for (i = 1; i < 128; i++)
		AR_ADJUST_TABLE[i] = (uint16_t)((double)(1 << EG_BITS) - 1 - (1 << EG_BITS) * log(i) / log(128));
}


/* Table for dB(0 -- (1<<DB_BITS)-1) to Liner(0 -- DB2LIN_AMP_WIDTH) */
static void makeDB2LinTable(void) {
	int32_t i;

	for (i = 0; i < DB_MUTE + DB_MUTE; i++) {
		DB2LIN_TABLE[i] = (int16_t)((double)((1 << DB2LIN_AMP_BITS) - 1) * pow(10, -(double)i * DB_STEP / 20));
		if (i >= DB_MUTE) DB2LIN_TABLE[i] = 0;
		DB2LIN_TABLE[i + DB_MUTE + DB_MUTE] = (int16_t)(-DB2LIN_TABLE[i]);
	}
}

/* Liner(+0.0 - +1.0) to dB((1<<DB_BITS) - 1 -- 0) */
static int32_t lin2db(double d) {
	if (d == 0)
		return(DB_MUTE - 1);
	else
		return Min(-(int32_t)(20.0 * log10(d) / DB_STEP), DB_MUTE - 1);	/* 0 -- 127 */
}


/* Sin Table */
static void makeSinTable(void) {
	int32_t i;

	for (i = 0; i < PG_WIDTH / 4; i++) {
		fullsintable[i] = (uint32_t)lin2db(sin(2.0 * PI * i / PG_WIDTH));
	}
=======

static void makeSinTable(void) {
  int x;

  for (x = 0; x < PG_WIDTH / 4; x++) {
    fullsin_table[PG_WIDTH / 4 + x] = fullsin_table[PG_WIDTH / 4 - x - 1];
  }
>>>>>>> f6dccad9 (Update libretro_core_options.h)

  for (x = 0; x < PG_WIDTH / 2; x++) {
    fullsin_table[PG_WIDTH / 2 + x] = 0x8000 | fullsin_table[x];
  }

<<<<<<< HEAD
	for (i = 0; i < PG_WIDTH / 2; i++) {
		fullsintable[PG_WIDTH / 2 + i] = (uint32_t)(DB_MUTE + DB_MUTE + fullsintable[i]);
	}

	for (i = 0; i < PG_WIDTH / 2; i++)
		halfsintable[i] = fullsintable[i];
	for (i = PG_WIDTH / 2; i < PG_WIDTH; i++)
		halfsintable[i] = fullsintable[0];
}

/* Table for Pitch Modulator */
static void makePmTable(void) {
	int32_t i;

	for (i = 0; i < PM_PG_WIDTH; i++)
		pmtable[i] = (int32_t)((double)PM_AMP * pow(2, (double)PM_DEPTH * sin(2.0 * PI * i / PM_PG_WIDTH) / 1200));
}

/* Table for Amp Modulator */
static void makeAmTable(void) {
	int32_t i;

	for (i = 0; i < AM_PG_WIDTH; i++)
		amtable[i] = (int32_t)((double)AM_DEPTH / 2 / DB_STEP * (1.0 + sin(2.0 * PI * i / PM_PG_WIDTH)));
}
>>>>>>> 52db67b5 (Update libretro_core_options.h)

/* Phase increment counter table */
static void makeDphaseTable(void) {
	uint32_t fnum, block, ML;
	uint32_t mltable[16] =
	{ 1, 1 * 2, 2 * 2, 3 * 2, 4 * 2, 5 * 2, 6 * 2, 7 * 2, 8 * 2, 9 * 2, 10 * 2, 10 * 2, 12 * 2, 12 * 2, 15 * 2, 15 * 2 };

	for (fnum = 0; fnum < 512; fnum++)
		for (block = 0; block < 8; block++)
			for (ML = 0; ML < 16; ML++)
				dphaseTable[fnum][block][ML] = rate_adjust(((fnum * mltable[ML]) << block) >> (20 - DP_BITS));
=======
  for (x = 0; x < PG_WIDTH / 2; x++)
    halfsin_table[x] = fullsin_table[x];

  for (x = PG_WIDTH / 2; x < PG_WIDTH; x++)
    halfsin_table[x] = 0xfff;
>>>>>>> f6dccad9 (Update libretro_core_options.h)
}

static void makeTllTable(void) {
<<<<<<< HEAD
	/* kltable[] values are all multiples of 0.25 dB, so kltable4[] = kltable*4
	 * is exact integer. EG_STEP is exactly 3/8, so the "/EG_STEP" divide is a
	 * "*8/3". The (int) truncation of (kltable[fnum] - 6*(7-block)) is
	 * reproduced by C integer division (also truncates toward zero). Verified
	 * bit-identical to the original double version across all 32768 entries. */
	static const int32_t kltable4[16] = {
		0, 72, 96, 111, 120, 129, 135, 141, 144, 150, 153, 156, 159, 162, 165, 168
	};
=======

  int32_t tmp;
  int32_t fnum, block, TL, KL;
>>>>>>> 52db67b5 (Update libretro_core_options.h)

<<<<<<< HEAD
	int32_t tmp;
	int32_t fnum, block, TL, KL;

	for (fnum = 0; fnum < 16; fnum++)
		for (block = 0; block < 8; block++)
			for (TL = 0; TL < 64; TL++)
				for (KL = 0; KL < 4; KL++) {
					if (KL == 0) {
						tllTable[fnum][block][TL][KL] = TL2EG(TL);
					} else {
						tmp = (kltable4[fnum] - 24 * (7 - block)) / 4;
						if (tmp <= 0)
							tllTable[fnum][block][TL][KL] = TL2EG(TL);
						else
							tllTable[fnum][block][TL][KL] = (uint32_t)(((tmp >> (3 - KL)) * 8) / 3) + TL2EG(TL);
					}
				}
}

/* Rate Table for Attack */
static void makeDphaseARTable(void) {
	int32_t AR, Rks, RM, RL;
	for (AR = 0; AR < 16; AR++)
		for (Rks = 0; Rks < 16; Rks++) {
			RM = AR + (Rks >> 2);
			RL = Rks & 3;
			if (RM > 15)
				RM = 15;
			switch (AR) {
			case 0:
				dphaseARTable[AR][Rks] = 0;
				break;
			case 15:
				dphaseARTable[AR][Rks] = 0;	/*EG_DP_WIDTH;*/
				break;
			default:
				dphaseARTable[AR][Rks] = rate_adjust((3 * (RL + 4) << (RM + 1)));
				break;
			}
		}
}

/* Rate Table for Decay and Release */
static void makeDphaseDRTable(void) {
	int32_t DR, Rks, RM, RL;

	for (DR = 0; DR < 16; DR++)
		for (Rks = 0; Rks < 16; Rks++) {
			RM = DR + (Rks >> 2);
			RL = Rks & 3;
			if (RM > 15)
				RM = 15;
			switch (DR) {
			case 0:
				dphaseDRTable[DR][Rks] = 0;
				break;
			default:
				dphaseDRTable[DR][Rks] = rate_adjust((RL + 4) << (RM - 1));
				break;
			}
		}
}

static void makeRksTable(void) {
	int32_t fnum8, block, KR;

	for (fnum8 = 0; fnum8 < 2; fnum8++)
		for (block = 0; block < 8; block++)
			for (KR = 0; KR < 2; KR++) {
				if (KR != 0)
					rksTable[fnum8][block][KR] = (block << 1) + fnum8;
				else
					rksTable[fnum8][block][KR] = block >> 1;
			}
}

/************************************************************

                      Calc Parameters

************************************************************/

INLINE static uint32_t calc_eg_dphase(OPLL_SLOT * slot) {
	switch (slot->eg_mode) {
	case ATTACK:
		return dphaseARTable[slot->patch.AR][slot->rks];
	case DECAY:
		return dphaseDRTable[slot->patch.DR][slot->rks];
	case SUSTINE:
		return dphaseDRTable[slot->patch.RR][slot->rks];
	case RELEASE:
		if (slot->sustine)
			return dphaseDRTable[5][slot->rks];
		else if (slot->patch.EG)
			return dphaseDRTable[slot->patch.RR][slot->rks];
		return dphaseDRTable[7][slot->rks];
	case SUSHOLD:
	case FINISH:
	default:
		break;
	}
	return 0;
=======
  for (fnum = 0; fnum < 16; fnum++) {
    for (block = 0; block < 8; block++) {
      for (TL = 0; TL < 64; TL++) {
        for (KL = 0; KL < 4; KL++) {
          if (KL == 0) {
            tll_table[(block << 4) | fnum][TL][KL] = TL2EG(TL);
          } else {
            tmp = (int32_t)(kl_table[fnum] - dB2(3.000) * (7 - block));
            if (tmp <= 0)
              tll_table[(block << 4) | fnum][TL][KL] = TL2EG(TL);
            else
              tll_table[(block << 4) | fnum][TL][KL] = (uint32_t)((tmp >> (3 - KL)) / EG_STEP) + TL2EG(TL);
          }
        }
      }
    }
  }
}

static void makeRksTable(void) {
  int fnum8, block;
  for (fnum8 = 0; fnum8 < 2; fnum8++)
    for (block = 0; block < 8; block++) {
      rks_table[(block << 1) | fnum8][1] = (block << 1) + fnum8;
      rks_table[(block << 1) | fnum8][0] = block >> 1;
    }
}

static void makeDefaultPatch(void) {
  int i, j;
  for (i = 0; i < OPLL_TONE_NUM; i++)
    for (j = 0; j < 19; j++)
      OPLL_getDefaultPatch(i, j, &default_patch[i][j * 2]);
>>>>>>> f6dccad9 (Update libretro_core_options.h)
}

static uint8_t table_initialized = 0;

<<<<<<< HEAD
                    OPLL internal interfaces

*************************************************************/

#define UPDATE_PG(S)  (S)->dphase = dphaseTable[(S)->fnum][(S)->block][(S)->patch.ML]
#define UPDATE_TLL(S) \
	(((S)->type == 0) ?	\
	 ((S)->tll = tllTable[((S)->fnum) >> 5][(S)->block][(S)->patch.TL][(S)->patch.KL]) : \
	 ((S)->tll = tllTable[((S)->fnum) >> 5][(S)->block][(S)->volume][(S)->patch.KL]))
#define UPDATE_RKS(S) (S)->rks = rksTable[((S)->fnum) >> 8][(S)->block][(S)->patch.KR]
#define UPDATE_WF(S)  (S)->sintbl = waveform[(S)->patch.WF]
#define UPDATE_EG(S)  (S)->eg_dphase = calc_eg_dphase(S)
#define UPDATE_ALL(S) \
	UPDATE_PG(S); \
	UPDATE_TLL(S); \
	UPDATE_RKS(S); \
	UPDATE_WF(S); \
	UPDATE_EG(S)				/* EG should be updated last. */


/* Slot key on  */
INLINE static void slotOn(OPLL_SLOT * slot) {
	slot->eg_mode = ATTACK;
	slot->eg_phase = 0;
	slot->phase = 0;
}

/* Slot key on without reseting the phase */
INLINE static void slotOn2(OPLL_SLOT * slot) {
	slot->eg_mode = ATTACK;
	slot->eg_phase = 0;
}

/* Slot key off */
INLINE static void slotOff(OPLL_SLOT * slot) {
	if (slot->eg_mode == ATTACK)
		slot->eg_phase = EXPAND_BITS(AR_ADJUST_TABLE[HIGHBITS(slot->eg_phase, EG_DP_BITS - EG_BITS)], EG_BITS, EG_DP_BITS);
	slot->eg_mode = RELEASE;
}

/* Channel key on */
INLINE static void keyOn(OPLL * opll, int32_t i) {
	if (!opll->slot_on_flag[i * 2])
		slotOn(MOD(opll, i));
	if (!opll->slot_on_flag[i * 2 + 1])
		slotOn(CAR(opll, i));
	opll->key_status[i] = 1;
}

/* Channel key off */
INLINE static void keyOff(OPLL * opll, int32_t i) {
	if (opll->slot_on_flag[i * 2 + 1])
		slotOff(CAR(opll, i));
	opll->key_status[i] = 0;
}

/* Set sustine parameter */
INLINE static void setSustine(OPLL * opll, int32_t c, int32_t sustine) {
	CAR(opll, c)->sustine = sustine;
	if (MOD(opll, c)->type)
		MOD(opll, c)->sustine = sustine;
}

/* Volume : 6bit ( Volume register << 2 ) */
INLINE static void setVolume(OPLL * opll, int32_t c, int32_t volume) {
	CAR(opll, c)->volume = volume;
}

INLINE static void setSlotVolume(OPLL_SLOT * slot, int32_t volume) {
	slot->volume = volume;
}

/* Set F-Number ( fnum : 9bit ) */
INLINE static void setFnumber(OPLL * opll, int32_t c, int32_t fnum) {
	CAR(opll, c)->fnum = fnum;
	MOD(opll, c)->fnum = fnum;
}

/* Set Block data (block : 3bit ) */
INLINE static void setBlock(OPLL * opll, int32_t c, int32_t block) {
	CAR(opll, c)->block = block;
	MOD(opll, c)->block = block;
}

INLINE static void update_key_status(OPLL * opll) {
	int ch;

	for (ch = 0; ch < 6; ch++)
		opll->slot_on_flag[ch * 2] = opll->slot_on_flag[ch * 2 + 1] = (opll->HiFreq[ch]) & 0x10;
}

/***********************************************************

                      Initializing

***********************************************************/

static void OPLL_SLOT_reset(OPLL_SLOT * slot, int type) {
	slot->type = type;
	slot->sintbl = waveform[0];
	slot->phase = 0;
	slot->dphase = 0;
	slot->output[0] = 0;
	slot->output[1] = 0;
	slot->feedback = 0;
	slot->eg_mode = SETTLE;
	slot->eg_phase = EG_DP_WIDTH;
	slot->eg_dphase = 0;
	slot->rks = 0;
	slot->tll = 0;
	slot->sustine = 0;
	slot->fnum = 0;
	slot->block = 0;
	slot->volume = 0;
	slot->pgout = 0;
	slot->egout = 0;
}

static void internal_refresh(void) {
	makeDphaseTable();
	makeDphaseARTable();
	makeDphaseDRTable();
	/* pm/am LFO phase deltas. The old code fed a fractional double argument
	 * (PM_SPEED=6.4, AM_SPEED=3.7) through rate_adjust. As exact rationals
	 * (6.4=32/5, 3.7=37/10) the whole thing is integer:
	 *   round( SPEEDnum * PG_WIDTH * clk / (SPEEDden * (clk/72) * 72 * rate) ).
	 * Bit-identical to the old double result at every supported rate; the
	 * 49716 native rate keeps rate_adjust's passthrough of the truncated arg. */
	if (rate == 49716) {
		pm_dphase = (uint32_t)(32ULL * PM_DP_WIDTH / (5ULL * (clk / 72)));
		am_dphase = (uint32_t)(37ULL * AM_DP_WIDTH / (10ULL * (clk / 72)));
	} else {
		uint64_t pden = 5ULL * (clk / 72) * 72ULL * rate;
		uint64_t aden = 10ULL * (clk / 72) * 72ULL * rate;
		pm_dphase = (uint32_t)((32ULL * PM_DP_WIDTH * clk + pden / 2) / pden);
		am_dphase = (uint32_t)((37ULL * AM_DP_WIDTH * clk + aden / 2) / aden);
	}
}

static void maketables(uint32_t c, uint32_t r) {
	if (c != clk) {
		clk = c;
		/* Only the rate/clk-independent integer tables remain to build here;
		 * the transcendental ones are baked constants (emu2413_tables.h). */
		makeTllTable();
		makeRksTable();
<<<<<<< HEAD
=======
		makeSinTable();
>>>>>>> eecb0c29 (Update libretro_core_options.h)
	}

	if (r != rate) {
		rate = r;
		internal_refresh();
	}
}

OPLL *OPLL_new(uint32_t _clk, uint32_t _rate) {
	OPLL *opll;

	maketables(_clk, _rate);

	opll = (OPLL*)calloc(sizeof(OPLL), 1);
	if (!opll)
		return NULL;

	opll->mask = 0;

	OPLL_reset(opll);

	return opll;
}


void OPLL_delete(OPLL * opll) {
	free(opll);
}

/* Reset whole of OPLL except patch datas. */
void OPLL_reset(OPLL * opll) {
	int32_t i;

	if (!opll)
		return;

	opll->adr = 0;
	opll->out = 0;

	opll->pm_phase = 0;
	opll->am_phase = 0;

	opll->mask = 0;

	for (i = 0; i < 12; i++)
		OPLL_SLOT_reset(&opll->slot[i], i % 2);

	for (i = 0; i < 6; i++) {
		opll->key_status[i] = 0;
		/* setPatch (opll, i, 0); */
	}

	for (i = 0; i < 0x40; i++)
		OPLL_writeReg(opll, i, 0);

	opll->realstep = (uint32_t)((1 << 31) / rate);
	opll->opllstep = (uint32_t)((1 << 31) / (clk / 72));
	opll->oplltime = 0;
}

/* Force Refresh (When external program changes some parameters). */
void OPLL_forceRefresh(OPLL * opll) {
	int32_t i;

	if (opll == NULL)
		return;

	for (i = 0; i < 12; i++) {
		UPDATE_PG(&opll->slot[i]);
		UPDATE_RKS(&opll->slot[i]);
		UPDATE_TLL(&opll->slot[i]);
		UPDATE_WF(&opll->slot[i]);
		UPDATE_EG(&opll->slot[i]);
	}
}

void OPLL_set_rate(OPLL * opll, uint32_t r) {
	if (opll->quality)
		rate = 49716;
	else
		rate = r;
	internal_refresh();
	rate = r;
}

void OPLL_set_quality(OPLL * opll, uint32_t q) {
	opll->quality = q;
	OPLL_set_rate(opll, rate);
=======
static void initializeTables(void) {
  makeTllTable();
  makeRksTable();
  makeSinTable();
  makeDefaultPatch();
  table_initialized = 1;
>>>>>>> f6dccad9 (Update libretro_core_options.h)
}

/*********************************************************

                      Synthesizing

*********************************************************/
#define SLOT_BD1 12
#define SLOT_BD2 13
#define SLOT_HH 14
#define SLOT_SD 15
#define SLOT_TOM 16
#define SLOT_CYM 17

/* utility macros */
#define MOD(o, x) (&(o)->slot[(x) << 1])
#define CAR(o, x) (&(o)->slot[((x) << 1) | 1])
#define BIT(s, b) (((s) >> (b)) & 1)

#if OPLL_DEBUG
static void _debug_print_patch(OPLL_SLOT *slot) {
  OPLL_PATCH *p = slot->patch;
  printf("[slot#%d am:%d pm:%d eg:%d kr:%d ml:%d kl:%d tl:%d ws:%d fb:%d A:%d D:%d S:%d R:%d]\n", slot->number, //
         p->AM, p->PM, p->EG, p->KR, p->ML,                                                                     //
         p->KL, p->TL, p->WS, p->FB,                                                                            //
         p->AR, p->DR, p->SL, p->RR);
}

static char *_debug_eg_state_name(OPLL_SLOT *slot) {
  switch (slot->eg_state) {
  case ATTACK:
    return "attack";
  case DECAY:
    return "decay";
  case SUSTAIN:
    return "sustain";
  case RELEASE:
    return "release";
  case DAMP:
    return "damp";
  default:
    return "unknown";
  }
}

static INLINE void _debug_print_slot_info(OPLL_SLOT *slot) {
  char *name = _debug_eg_state_name(slot);
  printf("[slot#%d state:%s fnum:%03x rate:%d-%d]\n", slot->number, name, slot->blk_fnum, slot->eg_rate_h,
         slot->eg_rate_l);
  _debug_print_patch(slot);
  fflush(stdout);
}
#endif

static INLINE int get_parameter_rate(OPLL_SLOT *slot) {

  if ((slot->type & 1) == 0 && slot->key_flag == 0) {
    return 0;
  }

  switch (slot->eg_state) {
  case ATTACK:
    return slot->patch->AR;
  case DECAY:
    return slot->patch->DR;
  case SUSTAIN:
    return slot->patch->EG ? 0 : slot->patch->RR;
  case RELEASE:
    if (slot->sus_flag) {
      return 5;
    } else if (slot->patch->EG) {
      return slot->patch->RR;
    } else {
      return 7;
    }
  case DAMP:
    return DAMPER_RATE;
  default:
    return 0;
  }
}

enum SLOT_UPDATE_FLAG {
  UPDATE_WS = 1,
  UPDATE_TLL = 2,
  UPDATE_RKS = 4,
  UPDATE_EG = 8,
  UPDATE_ALL = 255
};

static INLINE void request_update(OPLL_SLOT *slot, int flag) { slot->update_requests |= flag; }

static void commit_slot_update(OPLL_SLOT *slot) {

#if OPLL_DEBUG
  if (slot->last_eg_state != slot->eg_state) {
    _debug_print_slot_info(slot);
    slot->last_eg_state = slot->eg_state;
  }
#endif

  if (slot->update_requests & UPDATE_WS) {
    slot->wave_table = wave_table_map[slot->patch->WS];
  }

  if (slot->update_requests & UPDATE_TLL) {
    if ((slot->type & 1) == 0) {
      slot->tll = tll_table[slot->blk_fnum >> 5][slot->patch->TL][slot->patch->KL];
    } else {
      slot->tll = tll_table[slot->blk_fnum >> 5][slot->volume][slot->patch->KL];
    }
  }

  if (slot->update_requests & UPDATE_RKS) {
    slot->rks = rks_table[slot->blk_fnum >> 8][slot->patch->KR];
  }

  if (slot->update_requests & (UPDATE_RKS | UPDATE_EG)) {
    int p_rate = get_parameter_rate(slot);

    if (p_rate == 0) {
      slot->eg_shift = 0;
      slot->eg_rate_h = 0;
      slot->eg_rate_l = 0;
      return;
    }

    slot->eg_rate_h = min(15, p_rate + (slot->rks >> 2));
    slot->eg_rate_l = slot->rks & 3;
    if (slot->eg_state == ATTACK) {
      slot->eg_shift = (0 < slot->eg_rate_h && slot->eg_rate_h < 12) ? (13 - slot->eg_rate_h) : 0;
    } else {
      slot->eg_shift = (slot->eg_rate_h < 13) ? (13 - slot->eg_rate_h) : 0;
    }
  }

  slot->update_requests = 0;
}

<<<<<<< HEAD
/* PG */
INLINE static void calc_phase(OPLL_SLOT * slot, int32_t lfo) {
	if (slot->patch.PM)
		slot->phase += (slot->dphase * lfo) >> PM_AMP_BITS;
	else
		slot->phase += slot->dphase;

	slot->phase &= (DP_WIDTH - 1);

	slot->pgout = HIGHBITS(slot->phase, DP_BASE_BITS);
}

/* EG */
static void calc_envelope(OPLL_SLOT * slot, int32_t lfo) {
#define S2E(x) (SL2EG((int32_t)(x / SL_STEP)) << (EG_DP_BITS - EG_BITS))

	static const uint32_t SL[16] = {
		S2E(0.0), S2E(3.0), S2E(6.0), S2E(9.0), S2E(12.0), S2E(15.0), S2E(18.0), S2E(21.0),
		S2E(24.0), S2E(27.0), S2E(30.0), S2E(33.0), S2E(36.0), S2E(39.0), S2E(42.0), S2E(48.0)
	};

	uint32_t egout;

	switch (slot->eg_mode) {
	case ATTACK:
		egout = AR_ADJUST_TABLE[HIGHBITS(slot->eg_phase, EG_DP_BITS - EG_BITS)];
		slot->eg_phase += slot->eg_dphase;
		if ((EG_DP_WIDTH & slot->eg_phase) || (slot->patch.AR == 15)) {
			egout = 0;
			slot->eg_phase = 0;
			slot->eg_mode = DECAY;
			UPDATE_EG(slot);
		}
		break;

	case DECAY:
		egout = HIGHBITS(slot->eg_phase, EG_DP_BITS - EG_BITS);
		slot->eg_phase += slot->eg_dphase;
		if (slot->eg_phase >= SL[slot->patch.SL]) {
			if (slot->patch.EG) {
				slot->eg_phase = SL[slot->patch.SL];
				slot->eg_mode = SUSHOLD;
				UPDATE_EG(slot);
			} else {
				slot->eg_phase = SL[slot->patch.SL];
				slot->eg_mode = SUSTINE;
				UPDATE_EG(slot);
			}
		}
		break;

	case SUSHOLD:
		egout = HIGHBITS(slot->eg_phase, EG_DP_BITS - EG_BITS);
		if (slot->patch.EG == 0) {
			slot->eg_mode = SUSTINE;
			UPDATE_EG(slot);
		}
		break;

	case SUSTINE:
	case RELEASE:
		egout = HIGHBITS(slot->eg_phase, EG_DP_BITS - EG_BITS);
		slot->eg_phase += slot->eg_dphase;
		if (egout >= (1 << EG_BITS)) {
			slot->eg_mode = FINISH;
			egout = (1 << EG_BITS) - 1;
		}
		break;

	case FINISH:
		egout = (1 << EG_BITS) - 1;
		break;

	default:
		egout = (1 << EG_BITS) - 1;
		break;
	}

	if (slot->patch.AM)
		egout = EG2DB(egout + slot->tll) + lfo;
	else
		egout = EG2DB(egout + slot->tll);

	if (egout >= DB_MUTE)
		egout = DB_MUTE - 1;

	slot->egout = egout;
}

/* CARRIOR */
INLINE static int32_t calc_slot_car(OPLL_SLOT * slot, int32_t fm) {
	slot->output[1] = slot->output[0];

	if (slot->egout >= (DB_MUTE - 1))
		slot->output[0] = 0;
	else
		slot->output[0] = DB2LIN_TABLE[slot->sintbl[(slot->pgout + wave2_8pi(fm)) & (PG_WIDTH - 1)] + slot->egout];

	return (slot->output[1] + slot->output[0]) >> 1;
}

/* MODULATOR */
INLINE static int32_t calc_slot_mod(OPLL_SLOT * slot) {
	int32_t fm;
=======
static void reset_slot(OPLL_SLOT *slot, int number) {
  slot->number = number;
  slot->type = number % 2;
  slot->pg_keep = 0;
  slot->wave_table = wave_table_map[0];
  slot->pg_phase = 0;
  slot->output[0] = 0;
  slot->output[1] = 0;
  slot->eg_state = RELEASE;
  slot->eg_shift = 0;
  slot->rks = 0;
  slot->tll = 0;
  slot->key_flag = 0;
  slot->sus_flag = 0;
  slot->blk_fnum = 0;
  slot->blk = 0;
  slot->fnum = 0;
  slot->volume = 0;
  slot->pg_out = 0;
  slot->eg_out = EG_MUTE;
  slot->patch = &null_patch;
}

static INLINE void slotOn(OPLL *opll, int i) {
  OPLL_SLOT *slot = &opll->slot[i];
  slot->key_flag = 1;
  slot->eg_state = DAMP;
  request_update(slot, UPDATE_EG);
}

static INLINE void slotOff(OPLL *opll, int i) {
  OPLL_SLOT *slot = &opll->slot[i];
  slot->key_flag = 0;
  if (slot->type & 1) {
    slot->eg_state = RELEASE;
    request_update(slot, UPDATE_EG);
  }
}

static INLINE void update_key_status(OPLL *opll) {
  const uint8_t r14 = opll->reg[0x0e];
  const uint8_t rhythm_mode = BIT(r14, 5);
  uint32_t new_slot_key_status = 0;
  uint32_t updated_status;
  int ch;
>>>>>>> f6dccad9 (Update libretro_core_options.h)

  for (ch = 0; ch < 9; ch++)
    if (opll->reg[0x20 + ch] & 0x10)
      new_slot_key_status |= 3 << (ch * 2);

  if (rhythm_mode) {
    if (r14 & 0x10)
      new_slot_key_status |= 3 << SLOT_BD1;

    if (r14 & 0x01)
      new_slot_key_status |= 1 << SLOT_HH;

    if (r14 & 0x08)
      new_slot_key_status |= 1 << SLOT_SD;

    if (r14 & 0x04)
      new_slot_key_status |= 1 << SLOT_TOM;

    if (r14 & 0x02)
      new_slot_key_status |= 1 << SLOT_CYM;
  }

  updated_status = opll->slot_key_status ^ new_slot_key_status;

  if (updated_status) {
    int i;
    for (i = 0; i < 18; i++)
      if (BIT(updated_status, i)) {
        if (BIT(new_slot_key_status, i)) {
          slotOn(opll, i);
        } else {
          slotOff(opll, i);
        }
      }
  }

  opll->slot_key_status = new_slot_key_status;
}

<<<<<<< HEAD
static INLINE int16_t calc(OPLL * opll) {
	int32_t inst = 0, out = 0;
	int32_t i;

	update_ampm(opll);

	for (i = 0; i < 12; i++) {
		calc_phase(&opll->slot[i], opll->lfo_pm);
		calc_envelope(&opll->slot[i], opll->lfo_am);
	}

	for (i = 0; i < 6; i++)
		if (!(opll->mask & OPLL_MASK_CH(i)) && (CAR(opll, i)->eg_mode != FINISH))
			inst += calc_slot_car(CAR(opll, i), calc_slot_mod(MOD(opll, i)));

	out = inst;
	return (int16_t)out;
}

void OPLL_fillbuf(OPLL* opll, int32_t *buf, int32_t len, int shift) {
	while (len > 0) {
		*buf += (calc(opll) + 32768) << shift;
		buf++;
		len--;
	}
}

/* fceumm drives the OPLL exclusively through the integer-accurate calc()
 * path: opll->quality is calloc-zeroed in OPLL_new and OPLL_set_quality is
 * never called anywhere in the core, so the old sub-sample interpolator
 * below was unreachable. It was also the *only* floating-point arithmetic
 * left in any function that returns an audio sample, so it is removed here
 * to guarantee a deterministic, FP-free int16 output end-to-end.
 *
 * The realstep/opllstep/oplltime/prev/next/out fields are deliberately kept
 * (see emu2413.h): the VRC7 mapper serialises them into its savestate block,
 * and they have always stayed zero on this path, so leaving them in place
 * keeps the on-disk state layout byte-identical. */
int16_t OPLL_calc(OPLL * opll) {
	return calc(opll);
}

uint32_t OPLL_setMask(OPLL * opll, uint32_t mask) {
	uint32_t ret;

	if (opll) {
		ret = opll->mask;
		opll->mask = mask;
		return ret;
	}
	return 0;
}

uint32_t OPLL_toggleMask(OPLL * opll, uint32_t mask) {
	uint32_t ret;

	if (opll) {
		ret = opll->mask;
		opll->mask ^= mask;
		return ret;
	}
	return 0;
}

/****************************************************

                       I/O Ctrl

*****************************************************/

static void setInstrument(OPLL * opll, uint32_t i, uint32_t inst) {
	const uint8_t *src;
	OPLL_PATCH *modp, *carp;

	opll->patch_number[i] = inst;

	if (inst)
		src = default_inst[inst - 1];
	else
		src = opll->CustInst;

	modp = &MOD(opll, i)->patch;
	carp = &CAR(opll, i)->patch;

	modp->AM = (src[0] >> 7) & 1;
	modp->PM = (src[0] >> 6) & 1;
	modp->EG = (src[0] >> 5) & 1;
	modp->KR = (src[0] >> 4) & 1;
	modp->ML = (src[0] & 0xF);

	carp->AM = (src[1] >> 7) & 1;
	carp->PM = (src[1] >> 6) & 1;
	carp->EG = (src[1] >> 5) & 1;
	carp->KR = (src[1] >> 4) & 1;
	carp->ML = (src[1] & 0xF);

	modp->KL = (src[2] >> 6) & 3;
	modp->TL = (src[2] & 0x3F);

	carp->KL = (src[3] >> 6) & 3;
	carp->WF = (src[3] >> 4) & 1;

	modp->WF = (src[3] >> 3) & 1;

	modp->FB = (src[3]) & 7;

	modp->AR = (src[4] >> 4) & 0xF;
	modp->DR = (src[4] & 0xF);

	carp->AR = (src[5] >> 4) & 0xF;
	carp->DR = (src[5] & 0xF);

	modp->SL = (src[6] >> 4) & 0xF;
	modp->RR = (src[6] & 0xF);

	carp->SL = (src[7] >> 4) & 0xF;
	carp->RR = (src[7] & 0xF);
=======
static INLINE void set_patch(OPLL *opll, int32_t ch, int32_t num) {
  opll->patch_number[ch] = num;
  MOD(opll, ch)->patch = &opll->patch[num * 2 + 0];
  CAR(opll, ch)->patch = &opll->patch[num * 2 + 1];
  request_update(MOD(opll, ch), UPDATE_ALL);
  request_update(CAR(opll, ch), UPDATE_ALL);
}

static INLINE void set_sus_flag(OPLL *opll, int ch, int flag) {
  CAR(opll, ch)->sus_flag = flag;
  request_update(CAR(opll, ch), UPDATE_EG);
  if (MOD(opll, ch)->type & 1) {
    MOD(opll, ch)->sus_flag = flag;
    request_update(MOD(opll, ch), UPDATE_EG);
  }
}

/* set volume ( volume : 6bit, register value << 2 ) */
static INLINE void set_volume(OPLL *opll, int ch, int volume) {
  CAR(opll, ch)->volume = volume;
  request_update(CAR(opll, ch), UPDATE_TLL);
}

static INLINE void set_slot_volume(OPLL_SLOT *slot, int volume) {
  slot->volume = volume;
  request_update(slot, UPDATE_TLL);
}

/* set f-Nnmber ( fnum : 9bit ) */
static INLINE void set_fnumber(OPLL *opll, int ch, int fnum) {
  OPLL_SLOT *car = CAR(opll, ch);
  OPLL_SLOT *mod = MOD(opll, ch);
  car->fnum = fnum;
  car->blk_fnum = (car->blk_fnum & 0xe00) | (fnum & 0x1ff);
  mod->fnum = fnum;
  mod->blk_fnum = (mod->blk_fnum & 0xe00) | (fnum & 0x1ff);
  request_update(car, UPDATE_EG | UPDATE_RKS | UPDATE_TLL);
  request_update(mod, UPDATE_EG | UPDATE_RKS | UPDATE_TLL);
}

/* set block data (blk : 3bit ) */
static INLINE void set_block(OPLL *opll, int ch, int blk) {
  OPLL_SLOT *car = CAR(opll, ch);
  OPLL_SLOT *mod = MOD(opll, ch);
  car->blk = blk;
  car->blk_fnum = ((blk & 7) << 9) | (car->blk_fnum & 0x1ff);
  mod->blk = blk;
  mod->blk_fnum = ((blk & 7) << 9) | (mod->blk_fnum & 0x1ff);
  request_update(car, UPDATE_EG | UPDATE_RKS | UPDATE_TLL);
  request_update(mod, UPDATE_EG | UPDATE_RKS | UPDATE_TLL);
>>>>>>> f6dccad9 (Update libretro_core_options.h)
}

static INLINE void update_rhythm_mode(OPLL *opll) {
  const uint8_t new_rhythm_mode = (opll->reg[0x0e] >> 5) & 1;

<<<<<<< HEAD
void OPLL_writeReg(OPLL * opll, uint32_t reg, uint32_t data) {
	int32_t i, v, ch;
=======
  if (opll->rhythm_mode != new_rhythm_mode) {
>>>>>>> f6dccad9 (Update libretro_core_options.h)

    if (new_rhythm_mode) {
      opll->slot[SLOT_HH].type = 3;
      opll->slot[SLOT_HH].pg_keep = 1;
      opll->slot[SLOT_SD].type = 3;
      opll->slot[SLOT_TOM].type = 3;
      opll->slot[SLOT_CYM].type = 3;
      opll->slot[SLOT_CYM].pg_keep = 1;
      set_patch(opll, 6, 16);
      set_patch(opll, 7, 17);
      set_patch(opll, 8, 18);
      set_slot_volume(&opll->slot[SLOT_HH], ((opll->reg[0x37] >> 4) & 15) << 2);
      set_slot_volume(&opll->slot[SLOT_TOM], ((opll->reg[0x38] >> 4) & 15) << 2);
    } else {
      opll->slot[SLOT_HH].type = 0;
      opll->slot[SLOT_HH].pg_keep = 0;
      opll->slot[SLOT_SD].type = 1;
      opll->slot[SLOT_TOM].type = 0;
      opll->slot[SLOT_CYM].type = 1;
      opll->slot[SLOT_CYM].pg_keep = 0;
      set_patch(opll, 6, opll->reg[0x36] >> 4);
      set_patch(opll, 7, opll->reg[0x37] >> 4);
      set_patch(opll, 8, opll->reg[0x38] >> 4);
    }
  }

  opll->rhythm_mode = new_rhythm_mode;
}

<<<<<<< HEAD
void OPLL_writeIO(OPLL * opll, uint32_t adr, uint32_t val) {
	if (adr & 1)
		OPLL_writeReg(opll, opll->adr, val);
	else
		opll->adr = val;
=======
static void update_ampm(OPLL *opll) {
  if (opll->test_flag & 2) {
    opll->pm_phase = 0;
    opll->am_phase = 0;
  } else {
    opll->pm_phase += (opll->test_flag & 8) ? 1024 : 1;
    opll->am_phase += (opll->test_flag & 8) ? 64 : 1;
  }
  opll->lfo_am = am_table[(opll->am_phase >> 6) % sizeof(am_table)];
}

static void update_noise(OPLL *opll, int cycle) {
  int i;
  for (i = 0; i < cycle; i++) {
    if (opll->noise & 1) {
      opll->noise ^= 0x800200;
    }
    opll->noise >>= 1;
  }
}

static void update_short_noise(OPLL *opll) {
  const uint32_t pg_hh = opll->slot[SLOT_HH].pg_out;
  const uint32_t pg_cym = opll->slot[SLOT_CYM].pg_out;

  const uint8_t h_bit2 = BIT(pg_hh, PG_BITS - 8);
  const uint8_t h_bit7 = BIT(pg_hh, PG_BITS - 3);
  const uint8_t h_bit3 = BIT(pg_hh, PG_BITS - 7);

  const uint8_t c_bit3 = BIT(pg_cym, PG_BITS - 7);
  const uint8_t c_bit5 = BIT(pg_cym, PG_BITS - 5);

  opll->short_noise = (h_bit2 ^ h_bit7) | (h_bit3 ^ c_bit5) | (c_bit3 ^ c_bit5);
}

static INLINE void calc_phase(OPLL_SLOT *slot, int32_t pm_phase, uint8_t reset) {
  const int8_t pm = slot->patch->PM ? pm_table[(slot->fnum >> 6) & 7][(pm_phase >> 10) & 7] : 0;
  if (reset) {
    slot->pg_phase = 0;
  }
  slot->pg_phase += (((slot->fnum & 0x1ff) * 2 + pm) * ml_table[slot->patch->ML]) << slot->blk >> 2;
  slot->pg_phase &= (DP_WIDTH - 1);
  slot->pg_out = slot->pg_phase >> DP_BASE_BITS;
}

static INLINE uint8_t lookup_attack_step(OPLL_SLOT *slot, uint32_t counter) {
  int index;

  switch (slot->eg_rate_h) {
  case 12:
    index = (counter & 0xc) >> 1;
    return 4 - eg_step_tables[slot->eg_rate_l][index];
  case 13:
    index = (counter & 0xc) >> 1;
    return 3 - eg_step_tables[slot->eg_rate_l][index];
  case 14:
    index = (counter & 0xc) >> 1;
    return 2 - eg_step_tables[slot->eg_rate_l][index];
  case 0:
  case 15:
    return 0;
  default:
    index = counter >> slot->eg_shift;
    return eg_step_tables[slot->eg_rate_l][index & 7] ? 4 : 0;
  }
}

static INLINE uint8_t lookup_decay_step(OPLL_SLOT *slot, uint32_t counter) {
  int index;

  switch (slot->eg_rate_h) {
  case 0:
    return 0;
  case 13:
    index = ((counter & 0xc) >> 1) | (counter & 1);
    return eg_step_tables[slot->eg_rate_l][index];
  case 14:
    index = ((counter & 0xc) >> 1);
    return eg_step_tables[slot->eg_rate_l][index] + 1;
  case 15:
    return 2;
  default:
    index = counter >> slot->eg_shift;
    return eg_step_tables[slot->eg_rate_l][index & 7];
  }
}

static INLINE void start_envelope(OPLL_SLOT *slot) {
  if (min(15, slot->patch->AR + (slot->rks >> 2)) == 15) {
    slot->eg_state = DECAY;
    slot->eg_out = 0;
  } else {
    slot->eg_state = ATTACK;
  }
  request_update(slot, UPDATE_EG);
}

static INLINE void calc_envelope(OPLL_SLOT *slot, OPLL_SLOT *buddy, uint16_t eg_counter, uint8_t test) {

  uint32_t mask = (1 << slot->eg_shift) - 1;
  uint8_t s;

  if (slot->eg_state == ATTACK) {
    if (0 < slot->eg_out && 0 < slot->eg_rate_h && (eg_counter & mask & ~3) == 0) {
      s = lookup_attack_step(slot, eg_counter);
      if (0 < s) {
        slot->eg_out = max(0, ((int)slot->eg_out - (slot->eg_out >> s) - 1));
      }
    }
  } else {
    if (slot->eg_rate_h > 0 && (eg_counter & mask) == 0) {
      slot->eg_out = min(EG_MUTE, slot->eg_out + lookup_decay_step(slot, eg_counter));
    }
  }

  switch (slot->eg_state) {
  case DAMP:
    /* DAMP to ATTACK transition is occured when the envelope reaches EG_MAX (max attenuation but it's not mute).
       Do not forget to check (eg_counter & mask) == 0 to synchronize it with the progress of the envelope. */
    if (slot->eg_out >= EG_MAX && (eg_counter & mask) == 0) {
      start_envelope(slot);
      if (slot->type & 1) {
        if (!slot->pg_keep) {
          slot->pg_phase = 0;
        }
        if (buddy && !buddy->pg_keep) {
          buddy->pg_phase = 0;
        }
      }
    }
    break;

  case ATTACK:
    if (slot->eg_out == 0) {
      slot->eg_state = DECAY;
      request_update(slot, UPDATE_EG);
    }
    break;

  case DECAY:
    /* DECAY to SUSTAIN transition must be checked at every cycle regardless of the conditions of the envelope rate and
       counter. i.e. the transition is not synchronized with the progress of the envelope. */
    if ((slot->eg_out >> 3) == slot->patch->SL) {
      slot->eg_state = SUSTAIN;
      request_update(slot, UPDATE_EG);
    }
    break;

  case SUSTAIN:
  case RELEASE:
  default:
    break;
  }

  if (test) {
    slot->eg_out = 0;
  }
}

static void update_slots(OPLL *opll) {
  int i;
  opll->eg_counter++;

  for (i = 0; i < 18; i++) {
    OPLL_SLOT *slot = &opll->slot[i];
    OPLL_SLOT *buddy = NULL;
    if (slot->type == 0) {
      buddy = &opll->slot[i + 1];
    }
    if (slot->type == 1) {
      buddy = &opll->slot[i - 1];
    }
    if (slot->update_requests) {
      commit_slot_update(slot);
    }
    calc_envelope(slot, buddy, opll->eg_counter, opll->test_flag & 1);
    calc_phase(slot, opll->pm_phase, opll->test_flag & 4);
  }
}

/* output: -4095...4095 */
static INLINE int16_t lookup_exp_table(uint16_t i) {
  /* from andete's expression */
  int16_t t = (exp_table[(i & 0xff) ^ 0xff] + 1024);
  int16_t res = t >> ((i & 0x7f00) >> 8);
  return ((i & 0x8000) ? ~res : res) << 1;
}

static INLINE int16_t to_linear(uint16_t h, OPLL_SLOT *slot, int16_t am) {
  uint16_t att;
  if (slot->eg_out > EG_MAX)
    return 0;

  att = min(EG_MUTE, (slot->eg_out + slot->tll + am)) << 4;
  return lookup_exp_table(h + att);
}

static INLINE int16_t calc_slot_car(OPLL *opll, int ch, int16_t fm) {
  OPLL_SLOT *slot = CAR(opll, ch);

  uint8_t am = slot->patch->AM ? opll->lfo_am : 0;

  slot->output[1] = slot->output[0];
  slot->output[0] = to_linear(slot->wave_table[(slot->pg_out + 2 * (fm >> 1)) & (PG_WIDTH - 1)], slot, am);

  return slot->output[0];
}

static INLINE int16_t calc_slot_mod(OPLL *opll, int ch) {
  OPLL_SLOT *slot = MOD(opll, ch);

  int16_t fm = slot->patch->FB > 0 ? (slot->output[1] + slot->output[0]) >> (9 - slot->patch->FB) : 0;
  uint8_t am = slot->patch->AM ? opll->lfo_am : 0;

  slot->output[1] = slot->output[0];
  slot->output[0] = to_linear(slot->wave_table[(slot->pg_out + fm) & (PG_WIDTH - 1)], slot, am);

  return slot->output[0];
}

static INLINE int16_t calc_slot_tom(OPLL *opll) {
  OPLL_SLOT *slot = MOD(opll, 8);

  return to_linear(slot->wave_table[slot->pg_out], slot, 0);
}

/* Specify phase offset directly based on 10-bit (1024-length) sine table */
#define _PD(phase) ((PG_BITS < 10) ? (phase >> (10 - PG_BITS)) : (phase << (PG_BITS - 10)))

static INLINE int16_t calc_slot_snare(OPLL *opll) {
  OPLL_SLOT *slot = CAR(opll, 7);

  uint32_t phase;

  if (BIT(slot->pg_out, PG_BITS - 2))
    phase = (opll->noise & 1) ? _PD(0x300) : _PD(0x200);
  else
    phase = (opll->noise & 1) ? _PD(0x0) : _PD(0x100);

  return to_linear(slot->wave_table[phase], slot, 0);
}

static INLINE int16_t calc_slot_cym(OPLL *opll) {
  OPLL_SLOT *slot = CAR(opll, 8);

  uint32_t phase = opll->short_noise ? _PD(0x300) : _PD(0x100);

  return to_linear(slot->wave_table[phase], slot, 0);
}

static INLINE int16_t calc_slot_hat(OPLL *opll) {
  OPLL_SLOT *slot = MOD(opll, 7);

  uint32_t phase;

  if (opll->short_noise)
    phase = (opll->noise & 1) ? _PD(0x2d0) : _PD(0x234);
  else
    phase = (opll->noise & 1) ? _PD(0x34) : _PD(0xd0);

  return to_linear(slot->wave_table[phase], slot, 0);
}

#define _MO(x) (-(x) >> 1)
#define _RO(x) (x)

static void update_output(OPLL *opll) {
  int16_t *out;
  int i;

  update_ampm(opll);
  update_short_noise(opll);
  update_slots(opll);

  out = opll->ch_out;

  /* CH1-6 */
  for (i = 0; i < 6; i++) {
    if (!(opll->mask & OPLL_MASK_CH(i))) {
      out[i] = _MO(calc_slot_car(opll, i, calc_slot_mod(opll, i)));
    }
  }

  /* CH7 */
  if (!opll->rhythm_mode) {
    if (!(opll->mask & OPLL_MASK_CH(6))) {
      out[6] = _MO(calc_slot_car(opll, 6, calc_slot_mod(opll, 6)));
    }
  } else {
    if (!(opll->mask & OPLL_MASK_BD)) {
      out[9] = _RO(calc_slot_car(opll, 6, calc_slot_mod(opll, 6)));
    }
  }
  update_noise(opll, 14);

  /* CH8 */
  if (!opll->rhythm_mode) {
    if (!(opll->mask & OPLL_MASK_CH(7))) {
      out[7] = _MO(calc_slot_car(opll, 7, calc_slot_mod(opll, 7)));
    }
  } else {
    if (!(opll->mask & OPLL_MASK_HH)) {
      out[10] = _RO(calc_slot_hat(opll));
    }
    if (!(opll->mask & OPLL_MASK_SD)) {
      out[11] = _RO(calc_slot_snare(opll));
    }
  }
  update_noise(opll, 2);

  /* CH9 */
  if (!opll->rhythm_mode) {
    if (!(opll->mask & OPLL_MASK_CH(8))) {
      out[8] = _MO(calc_slot_car(opll, 8, calc_slot_mod(opll, 8)));
    }
  } else {
    if (!(opll->mask & OPLL_MASK_TOM)) {
      out[12] = _RO(calc_slot_tom(opll));
    }
    if (!(opll->mask & OPLL_MASK_CYM)) {
      out[13] = _RO(calc_slot_cym(opll));
    }
  }
  update_noise(opll, 2);
}

INLINE static void mix_output(OPLL *opll) {
  int16_t out = 0;
  int i;
  for (i = 0; i < 14; i++) {
    out += opll->ch_out[i];
  }
  if (opll->conv) {
    OPLL_RateConv_putData(opll->conv, 0, out);
  } else {
    opll->mix_out[0] = out;
  }
}

INLINE static void mix_output_stereo(OPLL *opll) {
  int16_t *out = opll->mix_out;
  int i;
  out[0] = out[1] = 0;
  for (i = 0; i < 14; i++) {
    if (opll->pan[i] & 2)
      out[0] += (int16_t)(opll->ch_out[i] * opll->pan_fine[i][0]);
    if (opll->pan[i] & 1)
      out[1] += (int16_t)(opll->ch_out[i] * opll->pan_fine[i][1]);
  }
  if (opll->conv) {
    OPLL_RateConv_putData(opll->conv, 0, out[0]);
    OPLL_RateConv_putData(opll->conv, 1, out[1]);
  }
}

/***********************************************************

                   External Interfaces

***********************************************************/

OPLL *OPLL_new(uint32_t clk, uint32_t rate) {
  OPLL *opll;
  int i;

  if (!table_initialized) {
    initializeTables();
  }

  opll = (OPLL *)calloc(sizeof(OPLL), 1);
  if (opll == NULL)
    return NULL;

  for (i = 0; i < 19 * 2; i++)
    memcpy(&opll->patch[i], &null_patch, sizeof(OPLL_PATCH));

  opll->clk = clk;
  opll->rate = rate;
  opll->mask = 0;
  opll->conv = NULL;
  opll->mix_out[0] = 0;
  opll->mix_out[1] = 0;

  OPLL_reset(opll);
  OPLL_setChipType(opll, 0);
  OPLL_resetPatch(opll, 0);
  return opll;
}

void OPLL_delete(OPLL *opll) {
  if (opll->conv) {
    OPLL_RateConv_delete(opll->conv);
    opll->conv = NULL;
  }
  free(opll);
}

static void reset_rate_conversion_params(OPLL *opll) {
  const double f_out = opll->rate;
  const double f_inp = opll->clk / 72.0;

  opll->out_time = 0;
  opll->out_step = f_inp;
  opll->inp_step = f_out;

  if (opll->conv) {
    OPLL_RateConv_delete(opll->conv);
    opll->conv = NULL;
  }

  if (floor(f_inp) != f_out && floor(f_inp + 0.5) != f_out) {
    opll->conv = OPLL_RateConv_new(f_inp, f_out, 2);
  }

  if (opll->conv) {
    OPLL_RateConv_reset(opll->conv);
  }
}

void OPLL_reset(OPLL *opll) {
  int i;

  if (!opll)
    return;

  opll->adr = 0;

  opll->pm_phase = 0;
  opll->am_phase = 0;

  opll->noise = 0x1;
  opll->mask = 0;

  opll->rhythm_mode = 0;
  opll->slot_key_status = 0;
  opll->eg_counter = 0;

  reset_rate_conversion_params(opll);

  for (i = 0; i < 18; i++)
    reset_slot(&opll->slot[i], i);

  for (i = 0; i < 9; i++) {
    set_patch(opll, i, 0);
  }

  for (i = 0; i < 0x40; i++)
    OPLL_writeReg(opll, i, 0);

  for (i = 0; i < 15; i++) {
    opll->pan[i] = 3;
    opll->pan_fine[i][1] = opll->pan_fine[i][0] = 1.0f;
  }

  for (i = 0; i < 14; i++) {
    opll->ch_out[i] = 0;
  }
}

void OPLL_forceRefresh(OPLL *opll) {
  int i;

  if (opll == NULL)
    return;

  for (i = 0; i < 9; i++) {
    set_patch(opll, i, opll->patch_number[i]);
  }

  for (i = 0; i < 18; i++) {
    request_update(&opll->slot[i], UPDATE_ALL);
  }
}

void OPLL_setRate(OPLL *opll, uint32_t rate) {
  opll->rate = rate;
  reset_rate_conversion_params(opll);
}

void OPLL_setQuality(OPLL *opll, uint8_t q) {}

void OPLL_setChipType(OPLL *opll, uint8_t type) { opll->chip_type = type; }

void OPLL_writeReg(OPLL *opll, uint32_t reg, uint8_t data) {
  int ch, i;

  if (reg >= 0x40)
    return;

  /* mirror registers */
  if ((0x19 <= reg && reg <= 0x1f) || (0x29 <= reg && reg <= 0x2f) || (0x39 <= reg && reg <= 0x3f)) {
    reg -= 9;
  }

  opll->reg[reg] = (uint8_t)data;

  switch (reg) {
  case 0x00:
    opll->patch[0].AM = (data >> 7) & 1;
    opll->patch[0].PM = (data >> 6) & 1;
    opll->patch[0].EG = (data >> 5) & 1;
    opll->patch[0].KR = (data >> 4) & 1;
    opll->patch[0].ML = (data)&15;
    for (i = 0; i < 9; i++) {
      if (opll->patch_number[i] == 0) {
        request_update(MOD(opll, i), UPDATE_RKS | UPDATE_EG);
      }
    }
    break;

  case 0x01:
    opll->patch[1].AM = (data >> 7) & 1;
    opll->patch[1].PM = (data >> 6) & 1;
    opll->patch[1].EG = (data >> 5) & 1;
    opll->patch[1].KR = (data >> 4) & 1;
    opll->patch[1].ML = (data)&15;
    for (i = 0; i < 9; i++) {
      if (opll->patch_number[i] == 0) {
        request_update(CAR(opll, i), UPDATE_RKS | UPDATE_EG);
      }
    }
    break;

  case 0x02:
    opll->patch[0].KL = (data >> 6) & 3;
    opll->patch[0].TL = (data)&63;
    for (i = 0; i < 9; i++) {
      if (opll->patch_number[i] == 0) {
        request_update(MOD(opll, i), UPDATE_TLL);
      }
    }
    break;

  case 0x03:
    opll->patch[1].KL = (data >> 6) & 3;
    opll->patch[1].WS = (data >> 4) & 1;
    opll->patch[0].WS = (data >> 3) & 1;
    opll->patch[0].FB = (data)&7;
    for (i = 0; i < 9; i++) {
      if (opll->patch_number[i] == 0) {
        request_update(MOD(opll, i), UPDATE_WS);
        request_update(CAR(opll, i), UPDATE_WS | UPDATE_TLL);
      }
    }
    break;

  case 0x04:
    opll->patch[0].AR = (data >> 4) & 15;
    opll->patch[0].DR = (data)&15;
    for (i = 0; i < 9; i++) {
      if (opll->patch_number[i] == 0) {
        request_update(MOD(opll, i), UPDATE_EG);
      }
    }
    break;

  case 0x05:
    opll->patch[1].AR = (data >> 4) & 15;
    opll->patch[1].DR = (data)&15;
    for (i = 0; i < 9; i++) {
      if (opll->patch_number[i] == 0) {
        request_update(CAR(opll, i), UPDATE_EG);
      }
    }
    break;

  case 0x06:
    opll->patch[0].SL = (data >> 4) & 15;
    opll->patch[0].RR = (data)&15;
    for (i = 0; i < 9; i++) {
      if (opll->patch_number[i] == 0) {
        request_update(MOD(opll, i), UPDATE_EG);
      }
    }
    break;

  case 0x07:
    opll->patch[1].SL = (data >> 4) & 15;
    opll->patch[1].RR = (data)&15;
    for (i = 0; i < 9; i++) {
      if (opll->patch_number[i] == 0) {
        request_update(CAR(opll, i), UPDATE_EG);
      }
    }
    break;

  case 0x0e:
    if (opll->chip_type == 1)
      break;
    update_rhythm_mode(opll);
    update_key_status(opll);
    break;

  case 0x0f:
    opll->test_flag = data;
    break;

  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
    ch = reg - 0x10;
    set_fnumber(opll, ch, data + ((opll->reg[0x20 + ch] & 1) << 8));
    break;

  case 0x20:
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
  case 0x25:
  case 0x26:
  case 0x27:
  case 0x28:
    ch = reg - 0x20;
    set_fnumber(opll, ch, ((data & 1) << 8) + opll->reg[0x10 + ch]);
    set_block(opll, ch, (data >> 1) & 7);
    set_sus_flag(opll, ch, (data >> 5) & 1);
    update_key_status(opll);
    break;

  case 0x30:
  case 0x31:
  case 0x32:
  case 0x33:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x37:
  case 0x38:
    if ((opll->reg[0x0e] & 32) && (reg >= 0x36)) {
      switch (reg) {
      case 0x37:
        set_slot_volume(MOD(opll, 7), ((data >> 4) & 15) << 2);
        break;
      case 0x38:
        set_slot_volume(MOD(opll, 8), ((data >> 4) & 15) << 2);
        break;
      default:
        break;
      }
    } else {
      set_patch(opll, reg - 0x30, (data >> 4) & 15);
    }
    set_volume(opll, reg - 0x30, (data & 15) << 2);
    break;

  default:
    break;
  }
}

void OPLL_writeIO(OPLL *opll, uint32_t adr, uint8_t val) {
  if (adr & 1)
    OPLL_writeReg(opll, opll->adr, val);
  else
    opll->adr = val;
}

void OPLL_setPan(OPLL *opll, uint32_t ch, uint8_t pan) { opll->pan[ch & 15] = pan; }

void OPLL_setPanFine(OPLL *opll, uint32_t ch, float pan[2]) {
  opll->pan_fine[ch & 15][0] = pan[0];
  opll->pan_fine[ch & 15][1] = pan[1];
}

void OPLL_dumpToPatch(const uint8_t *dump, OPLL_PATCH *patch) {
  patch[0].AM = (dump[0] >> 7) & 1;
  patch[1].AM = (dump[1] >> 7) & 1;
  patch[0].PM = (dump[0] >> 6) & 1;
  patch[1].PM = (dump[1] >> 6) & 1;
  patch[0].EG = (dump[0] >> 5) & 1;
  patch[1].EG = (dump[1] >> 5) & 1;
  patch[0].KR = (dump[0] >> 4) & 1;
  patch[1].KR = (dump[1] >> 4) & 1;
  patch[0].ML = (dump[0]) & 15;
  patch[1].ML = (dump[1]) & 15;
  patch[0].KL = (dump[2] >> 6) & 3;
  patch[1].KL = (dump[3] >> 6) & 3;
  patch[0].TL = (dump[2]) & 63;
  patch[1].TL = 0;
  patch[0].FB = (dump[3]) & 7;
  patch[1].FB = 0;
  patch[0].WS = (dump[3] >> 3) & 1;
  patch[1].WS = (dump[3] >> 4) & 1;
  patch[0].AR = (dump[4] >> 4) & 15;
  patch[1].AR = (dump[5] >> 4) & 15;
  patch[0].DR = (dump[4]) & 15;
  patch[1].DR = (dump[5]) & 15;
  patch[0].SL = (dump[6] >> 4) & 15;
  patch[1].SL = (dump[7] >> 4) & 15;
  patch[0].RR = (dump[6]) & 15;
  patch[1].RR = (dump[7]) & 15;
}

void OPLL_getDefaultPatch(int32_t type, int32_t num, OPLL_PATCH *patch) {
  OPLL_dumpToPatch(default_inst[type] + num * 8, patch);
}

void OPLL_setPatch(OPLL *opll, const uint8_t *dump) {
  OPLL_PATCH patch[2];
  int i;
  for (i = 0; i < 19; i++) {
    OPLL_dumpToPatch(dump + i * 8, patch);
    memcpy(&opll->patch[i * 2 + 0], &patch[0], sizeof(OPLL_PATCH));
    memcpy(&opll->patch[i * 2 + 1], &patch[1], sizeof(OPLL_PATCH));
  }
}

void OPLL_patchToDump(const OPLL_PATCH *patch, uint8_t *dump) {
  dump[0] = (uint8_t)((patch[0].AM << 7) + (patch[0].PM << 6) + (patch[0].EG << 5) + (patch[0].KR << 4) + patch[0].ML);
  dump[1] = (uint8_t)((patch[1].AM << 7) + (patch[1].PM << 6) + (patch[1].EG << 5) + (patch[1].KR << 4) + patch[1].ML);
  dump[2] = (uint8_t)((patch[0].KL << 6) + patch[0].TL);
  dump[3] = (uint8_t)((patch[1].KL << 6) + (patch[1].WS << 4) + (patch[0].WS << 3) + patch[0].FB);
  dump[4] = (uint8_t)((patch[0].AR << 4) + patch[0].DR);
  dump[5] = (uint8_t)((patch[1].AR << 4) + patch[1].DR);
  dump[6] = (uint8_t)((patch[0].SL << 4) + patch[0].RR);
  dump[7] = (uint8_t)((patch[1].SL << 4) + patch[1].RR);
}

void OPLL_copyPatch(OPLL *opll, int32_t num, OPLL_PATCH *patch) {
  memcpy(&opll->patch[num], patch, sizeof(OPLL_PATCH));
}

void OPLL_resetPatch(OPLL *opll, uint8_t type) {
  int i;
  for (i = 0; i < 19 * 2; i++)
    OPLL_copyPatch(opll, i, &default_patch[type % OPLL_TONE_NUM][i]);
}

int16_t OPLL_calc(OPLL *opll) {
  while (opll->out_step > opll->out_time) {
    opll->out_time += opll->inp_step;
    update_output(opll);
    mix_output(opll);
  }
  opll->out_time -= opll->out_step;
  if (opll->conv) {
    opll->mix_out[0] = OPLL_RateConv_getData(opll->conv, 0);
  }
  return opll->mix_out[0];
}

void OPLL_calcStereo(OPLL *opll, int32_t out[2]) {
  while (opll->out_step > opll->out_time) {
    opll->out_time += opll->inp_step;
    update_output(opll);
    mix_output_stereo(opll);
  }
  opll->out_time -= opll->out_step;
  if (opll->conv) {
    out[0] = OPLL_RateConv_getData(opll->conv, 0);
    out[1] = OPLL_RateConv_getData(opll->conv, 1);
  } else {
    out[0] = opll->mix_out[0];
    out[1] = opll->mix_out[1];
  }
}

uint32_t OPLL_setMask(OPLL *opll, uint32_t mask) {
  uint32_t ret;

  if (opll) {
    ret = opll->mask;
    opll->mask = mask;
    return ret;
  } else
    return 0;
}

uint32_t OPLL_toggleMask(OPLL *opll, uint32_t mask) {
  uint32_t ret;

  if (opll) {
    ret = opll->mask;
    opll->mask ^= mask;
    return ret;
  } else
    return 0;
>>>>>>> f6dccad9 (Update libretro_core_options.h)
}
