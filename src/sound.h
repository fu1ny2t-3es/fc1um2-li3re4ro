/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _FCEU_SOUND_H
#define _FCEU_SOUND_H

<<<<<<< HEAD
#include "fceu-types.h"

/* Identifiers for the expansion-audio channels exposed to the
 * per-channel volume API (#512).  Indices into FSettings.ExpVolume[].
 * Numbering is internal-only - the savestate format and core options
 * key names do not depend on these values - but the count must match
 * FSettings.ExpVolume[]'s declared length in fceu.h. */
enum {
	SND_FDS  = 0,	/* Famicom Disk System 2C33 wavetable */
	SND_S5B  = 1,	/* Sunsoft 5B / YM2149F (mapper 69) */
	SND_N163 = 2,	/* Namco 163 (mapper 19 / "n106") */
	SND_VRC6 = 3,	/* Konami VRC6 (mappers 24, 26) */
	SND_VRC7 = 4,	/* Konami VRC7 / YM2413-clone (mapper 85) */
	SND_MMC5 = 5,	/* Nintendo MMC5 PCM + square (mapper 5) */
	SND_EXP_LAST = 6
};

/* Apply the per-channel volume modifier from FSettings.ExpVolume[].
 * Scale is 0..256: 256 (default) returns the sample unchanged so the
 * hot path stays free of multiplies on builds where the user has not
 * touched the new options; 0 returns silence; intermediate values
 * scale the sample by (in * vol) / 256.  Used by the six expansion-
 * audio modules (see fds_apu.c, boards/vrc6.c, boards/vrc7.c,
 * boards/n106.c, boards/mmc5.c, boards/69.c). */
int32_t GetExpOutput(int channel, int32_t in);
=======
enum AUDIO_CHANNEL {
	/* NES APU */
	SND_MASTER  = 0,
	SND_SQUARE1  = 1,
	SND_SQUARE2  = 2,
	SND_TRIANGLE = 3,
	SND_NOISE    = 4,
	SND_DMC      = 5,

	/* EXPANSION AUDIO */
	/* index beyond this line also affects GameExpSound struct index */

	SND_VRC6     = 6,
	SND_VRC7     = 7,
	SND_FDS      = 8,
	SND_N163     = 9,
	SND_S5B      = 10,
	SND_MMC5     = 11,
	
	SND_LAST = 12
};

typedef struct {
	uint8 Speed;
	uint8 Mode;
	uint8 DecCountTo1;
	uint8 decvolume;
	int reloaddec;
} ENVUNIT;
>>>>>>> 515b323a (Update libretro_core_options.h)

typedef struct {
	void (*Fill)(int Count);	/* Low quality ext sound. */

	/* NeoFill is for sound devices that are emulated in a more
		high-level manner(VRC7) in HQ mode.  Interestingly,
		this device has slightly better sound quality(updated more
		often) in lq mode than in high-quality mode.  Maybe that
		should be fixed. :)
	*/
	void (*NeoFill)(int32_t *WaveBuf, int Count);
	void (*HiFill)(void);
	void (*HiSync)(int32_t ts);

	void (*RChange)(void);
	void (*Kill)(void);
} EXPSOUND;

#define GAMEEXPSOUND_COUNT 6
extern EXPSOUND GameExpSound[GAMEEXPSOUND_COUNT];

extern int32_t nesincsize;

void SetSoundVariables(void);

<<<<<<< HEAD
<<<<<<< HEAD
int GetSoundBuffer(int32_t **W);
=======
int GetSoundBuffer(void);
>>>>>>> f31fc48c (Update libretro_core_options.h)
int FlushEmulateSound(void);
extern int32_t Wave[2048 + 512];
extern int32_t WaveFinal[2048 + 512];
extern int32_t WaveHi[];
extern uint32_t soundtsinc;
=======
int GetSoundBuffer(int32 **W);
int FlushEmulateSound(void);
extern int32 Wave[8192 + 512];
extern int32 WaveFinal[8192 + 512];
extern int32 WaveHi[];
extern uint32 soundtsinc;
>>>>>>> a1c8c17c (Update libretro_core_options.h)

extern uint32_t soundtsoffs;
#define SOUNDTS (sound_timestamp + soundtsoffs)

void FCEUSND_Power(void);
void FCEUSND_Reset(void);
void FCEUSND_SaveState(void);
void FCEUSND_LoadState(int version);

void FCEU_SoundCPUHook(int);

/* Modify channel wave volume based on volume modifiers
 * Note: the formulat x = x * y /256 does not yield exact results,
 * but is "close enough" and avoids the need for using double values
 * or implicit cohersion which are slower (we need speed here) */
/* TODO: Optimize this. */
int32 GetOutput(int channel, int32 in);

#endif
