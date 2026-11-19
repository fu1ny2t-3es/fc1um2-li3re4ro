/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2009 CaH4e3
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * VRC-6 Sound
 *
 */

#include "mapinc.h"
#include "vrc6sound.h"

<<<<<<< HEAD
<<<<<<< HEAD
static uint8_t is26;
static uint8_t prg[2], chr[8], mirr;
static uint8_t IRQLatch, IRQa, IRQd;
static int32_t IRQCount, CycleCount;
static uint8_t *WRAM = NULL;
static uint32_t WRAMSIZE;

static SFORMAT StateRegs[] =
{
	{ prg, 2, "PRG" },
	{ chr, 8, "CHR" },
	{ &mirr, 1, "MIRR" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQd, 1, "IRQD" },
	{ &IRQLatch, 1, "IRQL" },
	{ &IRQCount, 4 | FCEUSTATE_RLSB, "IRQC" },
	{ &CycleCount, 4 | FCEUSTATE_RLSB, "CYCC" },
	{ 0 }
};

=======
>>>>>>> 6ebeb523 (Split up more mappers)
static void(*sfun[3]) (void);

static uint8_t vpsg1[8];
static uint8_t vpsg2[4];
static int32_t cvbc[3];
static int32_t vcount[3];
static int32_t dcount[3];
static int32_t phaseacc;
=======
enum { SQUARE1, SQUARE2, SAW };

typedef struct __VRC6SQUARE {
	/* regs */
	uint8 volume;
	uint8 duty;
	uint8 mode;
	uint8 enabled;
	uint16 freq;
>>>>>>> a1c8c17c (Update libretro_core_options.h)

<<<<<<< HEAD
/* These were excluded on Wii/GC (GEKKO) after 2018 reports of states
 * failing to load on big-endian hosts. The failures traced back to the
 * since-fixed FlipByteOrder over-iteration no-op and ReadStateChunk's
 * unchecked skip-seek, not to these entries: they are plain 4-byte
 * scalars with FCEUSTATE_RLSB, which the state layer byte-swaps
 * correctly on MSB_FIRST hosts. Register them everywhere so big-endian
 * builds save and restore the full expansion-audio state. */
	/* rw - 2018-11-28 Added */
	{ &cvbc[0], 4 | FCEUSTATE_RLSB, "BC01" },
	{ &cvbc[1], 4 | FCEUSTATE_RLSB, "BC02" },
	{ &cvbc[2], 4 | FCEUSTATE_RLSB, "BC03" },
	{ &dcount[0], 4 | FCEUSTATE_RLSB, "DCT0" },
	{ &dcount[1], 4 | FCEUSTATE_RLSB, "DCT1" },
	{ &dcount[2], 4 | FCEUSTATE_RLSB, "DCT2" },
	{ &vcount[0], 4 | FCEUSTATE_RLSB, "VCT0" },
	{ &vcount[1], 4 | FCEUSTATE_RLSB, "VCT1" },
	{ &vcount[2], 4 | FCEUSTATE_RLSB, "VCT2" },
	{ &phaseacc, 4 | FCEUSTATE_RLSB, "ACCU" },
	{ 0 }
};
=======
	/* timers */
	uint8 dcount;
	int32 vcount;
	int32 cvbc;
} VRC6SQUARE;
>>>>>>> 190d61ff (Update libretro_core_options.h)

typedef struct __VRC6SAW {
	/* regs */
	uint8 accumrate;
	uint8 enabled;
	uint16 freq;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static void Sync(void) {
	uint8_t i;
=======
static void VRC6Sync(void) {
	uint8 i;
>>>>>>> d5085b8d (Update libretro_core_options.h)
	if (is26)
		setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, prg[0]);
	setprg8(0xc000, prg[1]);
	setprg8(0xe000, ~0);
	for (i = 0; i < 8; i++)
		setchr1(i << 10, chr[i]);
	switch (mirr & 3) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

=======
>>>>>>> 6ebeb523 (Split up more mappers)
static void VRC6SW(uint32 A, uint8 V) {
	A &= 0xF003;
	if (A >= 0x9000 && A <= 0x9002) {
		vpsg1[A & 3] = V;
		if (sfun[0]) sfun[0]();
	} else if (A >= 0xA000 && A <= 0xA002) {
		vpsg1[4 | (A & 3)] = V;
		if (sfun[1]) sfun[1]();
	} else if (A >= 0xB000 && A <= 0xB002) {
		vpsg2[A & 3] = V;
		if (sfun[2]) sfun[2]();
=======
	uint8 phaseacc;

	/* timers */
	uint8 dcount;
	int32 vcount;
	int32 cvbc;
} VRC6SAW;

typedef struct __VRC6SOUND {
	VRC6SQUARE square[2];
	VRC6SAW saw;
	uint8 haltsound;
	uint8 freqshift;
} VR6SOUND;

static VR6SOUND VRC6Sound;

static void (*sfun[3])(void);

static INLINE void DoSQV(VRC6SQUARE *channel) {
	int32 start, end, V;

	start = channel->cvbc;
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start) {
		return;
>>>>>>> a1c8c17c (Update libretro_core_options.h)
	}
	channel->cvbc = end;

	if (!VRC6Sound.haltsound && channel->enabled) {
		int32 amp = ((channel->volume << 8) * 6 / 8) >> 4;
		int32 out = GetOutput(SND_VRC6, amp);

<<<<<<< HEAD
static void DoSQV1(void);
static void DoSQV2(void);
static void DoSawV(void);

static INLINE void DoSQV(int x) {
<<<<<<< HEAD
	int32_t V;
	int32_t amp = GetExpOutput(SND_VRC6,
		(((vpsg1[x << 2] & 15) << 8) * 6 / 8) >> 4);
	int32_t start, end;

	start = cvbc[x];
	end = (SOUNDTS << 16) / soundtsinc;
=======
	int32 V;
	int32 amp = (((vpsg1[x << 2] & 15) << 8) * 6 / 8) >> 4;
	int32 start = cvbc[x];
	int32 end = (SOUNDTS << 16) / soundtsinc;
>>>>>>> 6ebeb523 (Split up more mappers)
	if (end <= start) return;
	cvbc[x] = end;

	if (vpsg1[(x << 2) | 0x2] & 0x80) {
		if (vpsg1[x << 2] & 0x80) {
			for (V = start; V < end; V++)
				Wave[V >> 4] += amp;
		} else {
			int32_t thresh = (vpsg1[x << 2] >> 4) & 7;
			int32_t freq = ((vpsg1[(x << 2) | 0x1] | ((vpsg1[(x << 2) | 0x2] & 15) << 8)) + 1) << 17;
			int32_t dc = dcount[x];
			int32_t vc = vcount[x];

			for (V = start; V < end; V++) {
				if (dc > thresh)
					Wave[V >> 4] += amp;
				vc -= nesincsize;
				while (vc <= 0) {
					vc += freq;
					dc = (dc + 1) & 15;
				}
=======
		for (V = start; V < end; V++) {
			channel->vcount -= nesincsize;
			while (channel->vcount <= 0) {
				channel->dcount = (channel->dcount + 1) & 0x0F;
				channel->vcount += ((channel->freq >> VRC6Sound.freqshift) + 1) << 17;
			}
			if (channel->mode || (channel->dcount <= channel->duty)) {
				Wave[V >> 4] += out;
>>>>>>> a1c8c17c (Update libretro_core_options.h)
			}
		}
	}
}

static void DoSQV1(void) {
	DoSQV(&VRC6Sound.square[0]);
}

static void DoSQV2(void) {
	DoSQV(&VRC6Sound.square[1]);
}

static void DoSawV(void) {
<<<<<<< HEAD
	int V;
<<<<<<< HEAD
	int32_t start, end;

	start = cvbc[2];
	end = (SOUNDTS << 16) / soundtsinc;
=======
	int32 start = cvbc[2];
	int32 end = (SOUNDTS << 16) / soundtsinc;
>>>>>>> 6ebeb523 (Split up more mappers)
	if (end <= start) return;
	cvbc[2] = end;

	if (vpsg2[2] & 0x80) {
		uint32_t freq3;
		static uint32_t duff = 0;
=======
	int32 start, end, V;

	start = VRC6Sound.saw.cvbc;
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start) {
		return;
	}
	VRC6Sound.saw.cvbc = end;
>>>>>>> a1c8c17c (Update libretro_core_options.h)

	if (VRC6Sound.saw.enabled) {
		int32 out;

		for (V = start; V < end; V++) {
<<<<<<< HEAD
			vcount[2] -= nesincsize;
			if (vcount[2] <= 0) {
				int32_t t;
 rea:
				t = freq3;
				t <<= 18;
				vcount[2] += t;
				phaseacc += vpsg2[0] & 0x3f;
				dcount[2]++;
				if (dcount[2] == 7) {
					dcount[2] = 0;
					phaseacc = 0;
=======
			VRC6Sound.saw.vcount -= nesincsize;
			while (VRC6Sound.saw.vcount <= 0) {
				VRC6Sound.saw.dcount++;
				VRC6Sound.saw.dcount %= 14;
				if (VRC6Sound.saw.dcount == 0) {
					VRC6Sound.saw.phaseacc = 0;
				} else if (!(VRC6Sound.saw.dcount & 0x01)) {
					VRC6Sound.saw.phaseacc += VRC6Sound.saw.accumrate;
>>>>>>> a1c8c17c (Update libretro_core_options.h)
				}
<<<<<<< HEAD
				if (vcount[2] <= 0)
					goto rea;
				duff = GetExpOutput(SND_VRC6,
					(((phaseacc >> 3) & 0x1f) << 4) * 6 / 8);
=======
				VRC6Sound.saw.vcount += ((VRC6Sound.saw.freq >> VRC6Sound.freqshift) + 1) << 17;
>>>>>>> 515b323a (Update libretro_core_options.h)
			}
			out = (((VRC6Sound.saw.phaseacc >> 3) & 0x1F) << 3) * 6 / 8;
			Wave[V >> 4] += GetOutput(SND_VRC6, out);
		}
	}
}

<<<<<<< HEAD
static INLINE void DoSQVHQ(int x) {
	int32_t V;
	int32_t amp = GetExpOutput(SND_VRC6,
		((vpsg1[x << 2] & 15) << 8) * 6 / 8);

	if (vpsg1[(x << 2) | 0x2] & 0x80) {
		if (vpsg1[x << 2] & 0x80) {
			for (V = cvbc[x]; V < (int)SOUNDTS; V++)
				WaveHi[V] += amp;
		} else {
			int32_t thresh = (vpsg1[x << 2] >> 4) & 7;
			int32_t dc = dcount[x];
			int32_t vc = vcount[x];
=======
static INLINE void DoSQVHQ(VRC6SQUARE *channel) {
	int32 V;

	if (channel->enabled) {
		int32 amp = (channel->volume << 8) * 6 / 8;
		int32 out = GetOutput(SND_VRC6, amp);
>>>>>>> a1c8c17c (Update libretro_core_options.h)

		for (V = channel->cvbc; V < (int32)SOUNDTS; V++) {
			channel->vcount--;
			if (channel->vcount <= 0) {
				channel->dcount++;
				channel->dcount &= 0x0F;
				channel->vcount = (channel->freq >> VRC6Sound.freqshift) + 1;
			}
			if ((channel->mode || (channel->dcount > channel->duty))) {
				WaveHi[V] += out;
			}
		}
	}
	channel->cvbc = SOUNDTS;
}

static void DoSQV1HQ(void) {
	DoSQVHQ(&VRC6Sound.square[0]);
}

static void DoSQV2HQ(void) {
	DoSQVHQ(&VRC6Sound.square[1]);
}

static void DoSawVHQ(void) {
	int32_t V;

<<<<<<< HEAD
	if (vpsg2[2] & 0x80) {
		for (V = cvbc[2]; V < (int)SOUNDTS; V++) {
			WaveHi[V] += GetExpOutput(SND_VRC6,
				(((phaseacc >> 3) & 0x1f) << 8) * 6 / 8);
			vcount[2]--;
			if (vcount[2] <= 0) {
				vcount[2] = (vpsg2[1] + ((vpsg2[2] & 15) << 8) + 1) << 1;
				phaseacc += vpsg2[0] & 0x3f;
				dcount[2]++;
				if (dcount[2] == 7) {
					dcount[2] = 0;
					phaseacc = 0;
=======
	if (VRC6Sound.saw.enabled) {
		int32 out = 0;

		for (V = VRC6Sound.saw.cvbc; V < (int32)SOUNDTS; V++) {
			VRC6Sound.saw.vcount--;
			while (VRC6Sound.saw.vcount <= 0) {
				VRC6Sound.saw.dcount++;
				VRC6Sound.saw.dcount %= 14;
				if (VRC6Sound.saw.dcount == 0) {
					VRC6Sound.saw.phaseacc = 0;
				} else if (!(VRC6Sound.saw.dcount & 0x01)) {
					VRC6Sound.saw.phaseacc += VRC6Sound.saw.accumrate;
>>>>>>> 515b323a (Update libretro_core_options.h)
				}
				VRC6Sound.saw.vcount += ((VRC6Sound.saw.freq >> VRC6Sound.freqshift) + 1) << 0;
			}
			out = (((VRC6Sound.saw.phaseacc >> 3) & 0x1F) << 7) * 6 / 8;
			WaveHi[V] += GetOutput(SND_VRC6, out);
		}
	}
	VRC6Sound.saw.cvbc = SOUNDTS;
}

<<<<<<< HEAD
static void VRC6Sound(int Count) {
	int x;

=======
static void VRC6RunSound(int32 Count) {
>>>>>>> a1c8c17c (Update libretro_core_options.h)
	DoSQV1();
	DoSQV2();
	DoSawV();
	VRC6Sound.square[0].cvbc = Count;
	VRC6Sound.square[1].cvbc = Count;
	VRC6Sound.saw.cvbc = Count;

}

<<<<<<< HEAD
static void VRC6SoundHQ(void) {
=======
static void VRC6RunSoundHQ(void) {
>>>>>>> a1c8c17c (Update libretro_core_options.h)
	DoSQV1HQ();
	DoSQV2HQ();
	DoSawVHQ();
}

<<<<<<< HEAD
static void VRC6SyncHQ(int32_t ts) {
	int x;
	for (x = 0; x < 3; x++) cvbc[x] = ts;
=======
static void VRC6SyncHQ(int32 ts) {
	VRC6Sound.square[0].cvbc = ts;
	VRC6Sound.square[1].cvbc = ts;
	VRC6Sound.saw.cvbc = ts;
>>>>>>> a1c8c17c (Update libretro_core_options.h)
}

static void VRC6Square_Write(VRC6SQUARE *channel, uint8 reg, uint8 V) {
	switch (reg & 0x03) {
	case 0:
		channel->volume = V & 0x0F;
		channel->duty = (V & 0x70) >> 4;
		channel->mode = (V & 0x80) == 0x80;
		break;

	case 1:
		channel->freq = (channel->freq & 0xF00) | V;
		break;

	case 2:
		channel->freq = (channel->freq & 0xFF) | ((V & 0x0F) << 8);
		channel->enabled = (V & 0x80) == 0x80;
		if (!channel->enabled) {
			channel->dcount = 0;
		}
		break;
	}
}

DECLFW(VRC6Sound_Write) {
	switch (A & 0xF003) {
	case 0x9000:
	case 0x9001:
	case 0x9002:
		VRC6Square_Write(&VRC6Sound.square[0], A & 0x03, V);
		if (sfun[SQUARE1]) {
			sfun[SQUARE1]();
		}
		break;

	case 0x9003:
		VRC6Sound.haltsound = (V & 0x01) == 0x01;
		if (V & 0x04) {
			VRC6Sound.freqshift = 8;
		} else if (V & 0x02) {
			VRC6Sound.freqshift = 4;
		} else {
			VRC6Sound.freqshift = 0;
		}
		break;

	case 0xA000:
	case 0xA001:
	case 0xA002:
		VRC6Square_Write(&VRC6Sound.square[1], A & 0x03, V);
		if (sfun[SQUARE2]) {
			sfun[SQUARE2]();
		}
		break;
	
	case 0xB000:
	case 0xB001:
	case 0xB002:
		switch (A & 0x03) {
		case 0:
			VRC6Sound.saw.accumrate = V & 0x3F;
			break;

		case 1:
			VRC6Sound.saw.freq = (VRC6Sound.saw.freq & 0xF00) | V;
			break;

		case 2:
			VRC6Sound.saw.freq = (VRC6Sound.saw.freq & 0xFF) | ((V & 0x0F) << 8);
			VRC6Sound.saw.enabled = (V & 0x80) == 0x80;
			if (!VRC6Sound.saw.enabled) {
				VRC6Sound.saw.dcount = 0;
			}
			break;
		}
		if (sfun[SAW]) {
			sfun[SAW]();
		}
		break;
	}
}

static void VRC6Sound_SC(void) {
	GameExpSound[SND_VRC6 - 6].Fill   = VRC6RunSound;
	GameExpSound[SND_VRC6 - 6].HiFill = VRC6RunSoundHQ;
	GameExpSound[SND_VRC6 - 6].HiSync = VRC6SyncHQ;

	VRC6Sound.square[0].vcount = 1;
	VRC6Sound.square[1].vcount = 1;
	VRC6Sound.saw.vcount       = 1;

	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1) {
			sfun[SQUARE1] = DoSQV1HQ;
			sfun[SQUARE2] = DoSQV2HQ;
			sfun[SAW]     = DoSawVHQ;
		} else {
			sfun[SQUARE1] = DoSQV1;
			sfun[SQUARE2] = DoSQV2;
			sfun[SAW]     = DoSawV;
		}
	} else {
		memset(sfun, 0, sizeof(sfun));
	}
}

<<<<<<< HEAD
/* VRC6 Sound */

<<<<<<< HEAD
void Mapper24_Init(CartInfo *info) {
	is26 = 0;
	info->Power = VRC6Power;
	MapIRQHook = VRC6IRQHook;
	VRC6_ESI();
	GameStateRestore = VRC6StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
	AddExState(&SStateRegs, ~0, 0, 0);
}

void Mapper26_Init(CartInfo *info) {
	is26 = 1;
	info->Power = VRC6Power;
	info->Close = VRC6Close;
	MapIRQHook = VRC6IRQHook;
	VRC6_ESI();
	GameStateRestore = VRC6StateRestore;

	WRAMSIZE = 8192;
	WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
	AddExState(&StateRegs, ~0, 0, 0);
	AddExState(&SStateRegs, ~0, 0, 0);
}

=======
>>>>>>> 6ebeb523 (Split up more mappers)
void NSFVRC6_Init(void) {
	VRC6_ESI();
	SetWriteHandler(0x8000, 0xbfff, VRC6SW);
	AddExState(&SStateRegs, ~0, 0, 0);
=======
void VRC6Sound_ESI(void) {
	memset(&VRC6Sound, 0, sizeof(VRC6Sound));
	GameExpSound[SND_VRC6 - 6].RChange = VRC6Sound_SC;
	VRC6Sound_SC();
>>>>>>> a1c8c17c (Update libretro_core_options.h)
}

void VRC6Sound_AddStateInfo(void) {
	AddExState(&VRC6Sound.haltsound,         1, 0, "HLTS");
	AddExState(&VRC6Sound.freqshift,         1, 0, "FRSH");

	AddExState(&VRC6Sound.square[0].enabled, 1, 0, "S0EN");
	AddExState(&VRC6Sound.square[0].volume,  1, 0, "S0VL");
	AddExState(&VRC6Sound.square[0].freq,    2, 0, "S0FQ");
	AddExState(&VRC6Sound.square[0].duty,    1, 0, "S0DT");
	AddExState(&VRC6Sound.square[0].mode,    1, 0, "S0MD");
	AddExState(&VRC6Sound.square[0].dcount,  1, 0, "S0DC");
	AddExState(&VRC6Sound.square[0].vcount,  4, 0, "S0VC");
	AddExState(&VRC6Sound.square[0].cvbc,    4, 0, "S0BC");

	AddExState(&VRC6Sound.square[1].enabled, 1, 0, "S1EN");
	AddExState(&VRC6Sound.square[1].volume,  1, 0, "S1VL");
	AddExState(&VRC6Sound.square[1].freq,    2, 0, "S1FQ");
	AddExState(&VRC6Sound.square[1].duty,    1, 0, "S1DT");
	AddExState(&VRC6Sound.square[1].mode,    1, 0, "S1MD");
	AddExState(&VRC6Sound.square[1].dcount,  1, 0, "S1DC");
	AddExState(&VRC6Sound.square[1].vcount,  4, 0, "S1VC");
	AddExState(&VRC6Sound.square[1].cvbc,    4, 0, "S1BC");

	AddExState(&VRC6Sound.saw.enabled,       1, 0, "SWEN");
	AddExState(&VRC6Sound.saw.accumrate,     1, 0, "SWAR");
	AddExState(&VRC6Sound.saw.phaseacc,      1, 0, "SWAC");
	AddExState(&VRC6Sound.saw.freq,          2, 0, "SWFR");
	AddExState(&VRC6Sound.saw.dcount,        1, 0, "SWDC");
	AddExState(&VRC6Sound.saw.vcount,        4, 0, "SWVC");
	AddExState(&VRC6Sound.saw.cvbc,          4, 0, "SWBC");
}