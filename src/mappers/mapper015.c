/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
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
#include "latch.h"

<<<<<<< HEAD
<<<<<<< HEAD
static uint16_t latchea;
static uint8_t latched;
static uint8_t *WRAM = NULL;
static uint32_t WRAMSIZE;
=======
static uint16 latchea;
static uint8 latched;
static uint8 *WRAM = NULL;

#ifndef WRAM_SIZE
#define WRAM_SIZE 8192
#endif

>>>>>>> 74114ad0 (Update libretro.c)
static SFORMAT StateRegs[] =
{
	{ &latchea, 2 | FCEUSTATE_RLSB, "AREG" },
	{ &latched, 1, "DREG" },
	{ 0 }
};

<<<<<<< HEAD
static void Sync(void) {
	uint32_t preg[4];
	uint32_t bank = (latched & 0x3F) << 1;
=======
static void M15Sync(void) {
	uint32 preg[4];
	uint32 bank = (latched & 0x3F) << 1;
>>>>>>> 74114ad0 (Update libretro.c)
	switch (latchea & 0x03) {
=======
static void Sync(void) {
	uint8 prg   = latch.data & 0x3F;

	setprg8r(0x10, 0x6000, 0);
	switch (latch.addr & 0x03) {
>>>>>>> 7b06ddc4 (Update libretro.c)
	case 0:
		setprg32(0x8000, prg >> 1);
		break;
	case 1:
		setprg16(0x8000, prg);
		setprg16(0xC000, prg | 0x07);
		break;
	case 2:
		setprg8(0x8000, (prg << 1) | (latch.data >> 7));
		setprg8(0xA000, (prg << 1) | (latch.data >> 7));
		setprg8(0xC000, (prg << 1) | (latch.data >> 7));
		setprg8(0xE000, (prg << 1) | (latch.data >> 7));
		break;
	case 3:
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
		break;
	}
	setchr8(0);
	setmirror(((latch.data >> 6) & 0x01) ^ 0x01);
}

void Mapper015_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, TRUE, FALSE);
	info->Reset = Latch_RegReset;
}
<<<<<<< HEAD

static void M15StateRestore(int version) {
	M15Sync();
}

static void M15Power(void) {
	latchea = 0x8000;
	latched = 0;
	setchr8(0);
	setprg8r(0x10, 0x6000, 0);
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, M15Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	FCEU_CheatAddRAM(WRAM_SIZE >> 10, 0x6000, WRAM);
	M15Sync();
}

static void M15Reset(void) {
	latchea = 0x8000;
	latched = 0;
	M15Sync();
}

static void M15Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

void Mapper15_Init(CartInfo *info) {
	info->Power = M15Power;
	info->Reset = M15Reset;
	info->Close = M15Close;
<<<<<<< HEAD
	GameStateRestore = StateRestore;
	WRAMSIZE = 8192;
	WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
=======
	GameStateRestore = M15StateRestore;
<<<<<<< HEAD
	WRAM = (uint8*)FCEU_gmalloc(M15_WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, M15_WRAMSIZE, 1);
>>>>>>> 74114ad0 (Update libretro.c)
=======
	WRAM = (uint8*)FCEU_gmalloc(WRAM_SIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAM_SIZE, 1);
>>>>>>> 99588a93 (Update libretro.c)
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAM_SIZE;
	}
	AddExState(WRAM, WRAM_SIZE, 0, "WRAM");
	AddExState(&StateRegs, ~0, 0, 0);
}

=======
>>>>>>> 7b06ddc4 (Update libretro.c)
