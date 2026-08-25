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

/* NES 2.0 Mapper 329 - UNL-EDU2000 */

#include "mapinc.h"
<<<<<<< HEAD

static uint8_t *WRAM = NULL;
static uint8_t reg;

static SFORMAT StateRegs[] =
{
	{ &reg, 1, "REG" },
	{ 0 }
};
=======
#include "latch.h"
>>>>>>> 7b06ddc4 (Update libretro.c)

static void Sync(void) {
	setprg8r(0x10, 0x6000, latch.data >> 6);
	setprg32(0x8000, latch.data & 0x1F);
	setchr8(0);
	setmirror(((latch.data >> 5) & 0x01) ^ 0x01);
}

<<<<<<< HEAD
static void UNLEDU2000HiWrite(uint32 A, uint8 V) { reg = V; Sync(); }

static void UNLEDU2000Power(void) {
	setmirror(MI_0);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0xFFFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, UNLEDU2000HiWrite);
	FCEU_CheatAddRAM(32, 0x6000, WRAM);
	reg = 0;
	Sync();
}

static void UNLEDU2000Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void UNLEDU2000Restore(int version) { Sync(); }

void UNLEDU2000_Init(CartInfo *info) {
	info->Power = UNLEDU2000Power;
	info->Close = UNLEDU2000Close;
	GameStateRestore = UNLEDU2000Restore;
	WRAM = (uint8_t*)FCEU_gmalloc(32768);
	SetupCartPRGMapping(0x10, WRAM, 32768, 1);
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = 32768;
	}
	AddExState(WRAM, 32768, 0, "WRAM");
	AddExState(StateRegs, ~0, 0, 0);
=======
void Mapper329_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
>>>>>>> 7b06ddc4 (Update libretro.c)
}
