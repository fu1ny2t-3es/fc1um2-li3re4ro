/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "mapinc.h"
#include "mmc2.h"

<<<<<<< HEAD
<<<<<<< HEAD:src/mappers/mmc2and4.c
static uint8_t is10, isPC10;
static uint8_t creg[4], latch0, latch1, preg, mirr;
static uint8_t *WRAM = NULL;
static uint32_t WRAMSIZE;
=======
static uint8 isPC10;
static uint8 creg[4], latch0, latch1, preg, mirr;
static uint8 *WRAM = NULL;
<<<<<<< HEAD
static uint32 WRAMSIZE;
>>>>>>> 33b26dd0 (Turn mmc2and4.c into mapper009.c and mapper010.c and remove isPC10):src/mappers/mapper009.c
=======

#ifndef WRAM_SIZE
#define WRAM_SIZE 8192
#endif
>>>>>>> 0daf5c03 (Update libretro_core_options.h)

static SFORMAT StateRegs[] =
{
	{ creg, 4, "CREG" },
	{ &preg, 1, "PREG" },
	{ &mirr, 1, "MIRR" },
	{ &latch0, 1, "LAT0" },
	{ &latch1, 1, "LAT1" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, preg);
	setprg8(0xA000, ~2);
	setprg8(0xC000, ~1);
	setprg8(0xE000, ~0);
	if (isPC10)
		setprg8r(0x10, 0x6000, 0);
	setchr4(0x0000, creg[latch0]);
	setchr4(0x1000, creg[latch1 + 2]);
	setmirror(mirr);
}

<<<<<<< HEAD
static DECLFW(MMC2and4Write) {
=======
static void MMC2and4Write(uint32 A, uint8 V) {
>>>>>>> 3cd7a223 (Update libretro_core_options.h)
	switch (A & 0xF000) {
	case 0xA000: preg = V & 0xF; Sync(); break;
	case 0xB000: creg[0] = V & 0x1F; Sync(); break;
	case 0xC000: creg[1] = V & 0x1F; Sync(); break;
	case 0xD000: creg[2] = V & 0x1F; Sync(); break;
	case 0xE000: creg[3] = V & 0x1F; Sync(); break;
	case 0xF000: mirr = (V & 1) ^ 1; Sync(); break;
	}
}

<<<<<<< HEAD
static void FP_FASTAPASS(1) MMC2and4PPUHook(uint32_t A) {
	uint8_t l, h = A >> 8;
=======
static void MMC2and4PPUHook(uint32 A) {
	uint8 l, h = A >> 8;
>>>>>>> 3cd7a223 (Update libretro_core_options.h)
	if (h >= 0x20 || ((h & 0xF) != 0xF))
		return;
	l = A & 0xF0;
	if (h < 0x10) {
		if (l == 0xD0) {
			latch0 = 0;
			setchr4(0x0000, creg[0]);
		} else if (l == 0xE0) {
			latch0 = 1;
			setchr4(0x0000, creg[1]);
		}
	} else {
		if (l == 0xD0) {
			latch1 = 0;
			setchr4(0x1000, creg[2]);
		} else if (l == 0xE0) {
			latch1 = 1;
			setchr4(0x1000, creg[3]);
		}
	}
}

static void MMC2and4Power(void) {
	preg = 0;
	latch0 = latch1 = 1;
	Sync();
	if (isPC10) {
		SetReadHandler(0x6000, 0x7FFF, CartBR);
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
		FCEU_CheatAddRAM(WRAM_SIZE >> 10, 0x6000, WRAM);
	}
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0xA000, 0xFFFF, MMC2and4Write);
}

static void StateRestore(int version) {
	/* latch0/latch1 are written as boolean (0 or 1) and used as
	 * indices into creg[4] via creg[latch0] and creg[latch1 + 2].
	 * Clamp savestate-loaded values to keep the indices in range. */
	latch0 &= 1;
	latch1 &= 1;
	Sync();
}

static void MMC2and4Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

void Mapper9_Init(CartInfo *info) {
	isPC10 = 0;
	info->Power = MMC2and4Power;
	info->Close = MMC2and4Close;
	PPU_hook = MMC2and4PPUHook;
	if (info->battery) { /* Mike Tyson's Punch-Out!! (PC10) supports save ram */
		isPC10 = 1;
<<<<<<< HEAD
		WRAMSIZE = 8192;
		WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
=======
		WRAM = (uint8*)FCEU_gmalloc(WRAM_SIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAM_SIZE, 1);
		AddExState(WRAM, WRAM_SIZE, 0, "WRAM");
>>>>>>> 0daf5c03 (Update libretro_core_options.h)
		if (info->battery) {
			info->SaveGame[0] = WRAM;
			info->SaveGameLen[0] = WRAM_SIZE;
		}
	}
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
=======
static void M009PW(uint16 A, uint16 V) {
    setprg8(A, V & 0x0F);
}

static void M009CW(uint16 A, uint16 V) {
    setchr4(A, V & 0x1F);
}

void Mapper009_Init(CartInfo *info) {
	int ws = info->iNES2 ? ((info->PRGRamSize + info->PRGRamSaveSize) / 1024) : (info->battery ? 8 : 0);
	MMC2_Init(info, ws, info->battery);
    MMC2_pwrap = M009PW;
    MMC2_cwrap = M009CW;
>>>>>>> f6dccad9 (Update libretro_core_options.h)
}
<<<<<<< HEAD:src/mappers/mmc2and4.c

void Mapper10_Init(CartInfo *info) {
	is10 = 1;
	isPC10 = 0;
	info->Power = MMC2and4Power;
	info->Close = MMC2and4Close;
	PPU_hook = MMC2and4PPUHook;
	WRAMSIZE = 8192;
	WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
=======
>>>>>>> 33b26dd0 (Turn mmc2and4.c into mapper009.c and mapper010.c and remove isPC10):src/mappers/mapper009.c
