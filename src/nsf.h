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

#ifndef _FCEU_NSF_H
#define _FCEU_NSF_H

<<<<<<< HEAD
#include "file.h"

typedef struct {
	char ID[5];				/* NESM^Z */
	uint8_t Version;
	uint8_t TotalSongs;
	uint8_t StartingSong;
	uint8_t LoadAddressLow;
	uint8_t LoadAddressHigh;
	uint8_t InitAddressLow;
	uint8_t InitAddressHigh;
	uint8_t PlayAddressLow;
	uint8_t PlayAddressHigh;
	uint8_t SongName[32];
	uint8_t Artist[32];
	uint8_t Copyright[32];
	uint8_t NTSCspeed[2];		/* Unused */
	uint8_t BankSwitch[8];
	uint8_t PALspeed[2];		/* Unused */
	uint8_t VideoSystem;
	uint8_t SoundChip;
	uint8_t Expansion[4];
	uint8_t reserve[8];
=======
#define	NSFSOUND_VRC6	0x01
#define	NSFSOUND_VRC7	0x02
#define	NSFSOUND_FDS	0x04
#define	NSFSOUND_MMC5	0x08
#define	NSFSOUND_N163	0x10
#define	NSFSOUND_S5B	0x20

typedef struct NSF_HEADER {
	char ID[5];				/* NESM^Z */
	uint8 Version;
	uint8 TotalSongs;
	uint8 StartingSong;
	uint8 LoadAddressLow;
	uint8 LoadAddressHigh;
	uint8 InitAddressLow;
	uint8 InitAddressHigh;
	uint8 PlayAddressLow;
	uint8 PlayAddressHigh;
	uint8 GameName[32];
	uint8 Artist[32];
	uint8 Copyright[32];
	uint8 NTSCspeed[2];		/* Unused */
	uint8 BankSwitch[8];
	uint8 PALspeed[2];		/* Unused */
	uint8 VideoSystem;
	uint8 SoundChip;
	uint8 Expansion[4];
	uint8 reserve[8];
>>>>>>> ae14339e (Update Makefile.libretro)
} NSF_HEADER;

typedef struct NSFINFO {
	char SongName[256];
	char Artist[256];
	char Copyright[256];
	char Dumper[256];
	char SongNames[100][256];

	uint8 TotalSongs;
	uint8 StartingSong;
	uint8 CurrentSong;
	uint8 VideoSystem;

	uint16 PlayAddr, InitAddr, LoadAddr;
	uint8 BankSwitch[8];
	uint8 SoundChip;

	uint8 *NSFDATA;
	size_t NSFMaxBank;
	size_t NSFSize;
} NSFINFO;

extern NSFINFO *NSFInfo;

void NSF_init(void);
<<<<<<< HEAD
void DrawNSF(uint8_t *XBuf);
void NSFDealloc(void);
void NSFDodo(void);
=======
void DrawNSF(uint8 *target);
>>>>>>> ae14339e (Update Makefile.libretro)
void DoNSFFrame(void);
int NSFLoad(FCEUFILE *fp);

/* NSF Expansion Chip Set Write Handler for mappers */
void NFSSetWriteHandler(int chip, int32 start, int32 end, writefunc func);

#endif
