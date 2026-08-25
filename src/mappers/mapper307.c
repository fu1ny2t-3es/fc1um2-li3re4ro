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
 * FDS Conversion - Metroid - Jin Ji Zhi Ling (Kaiser)(KS7037)[U][!]
 * NES 2.0 Mapper 307 - UNL-KS7037
 *
 */

#include "mapinc.h"
#include "n118.h"
#include "fdssound.h"

<<<<<<< HEAD
static uint8_t reg[8], cmd;
static uint8_t *WRAM = NULL;
static uint32_t WRAMSIZE;

static void (*WSync)(void);

static SFORMAT StateRegs[] =
{
	{ &cmd, 1, "CMD" },
	{ reg, 8, "REGS" },
	{ 0 }
};

static void SyncKS7037(void) {
=======
static void M307FixPRG(void) {
>>>>>>> e98261e5 (Update ppu.c)
	setprg4r(0x10, 0x6000, 0);
	setprg4(0x7000, 15);
	setprg8(0x8000, n118.reg[6]);
	setprg4(0xA000, ~3);
	setprg4r(0x10, 0xB000, 0x01);
	setprg8(0xC000, n118.reg[7]);
	setprg8(0xE000, ~0);
}

static void M307FixCHR(void) {
	setchr8(0);
	setmirrorw(n118.reg[2] & 0x01, n118.reg[4] & 0x01, n118.reg[3] & 0x01, n118.reg[5] & 0x01);
}

static void M307Power(void) {
	FDSSound_Power();
	N118_Power();
	SetWriteHandler(0xB000, 0xBFFF, CartBW);
}

<<<<<<< HEAD
static void UNLKS7037Write(uint32 A, uint8 V) {
	switch (A & 0xE001) {
	case 0x8000: cmd = V & 7; break;
	case 0x8001: reg[cmd] = V; WSync(); break;
	}
}

static void UNLKS7037Power(void) {
	FDSSoundPower();
	reg[0] = reg[1] = reg[2] = reg[3] = reg[4] = reg[5] = reg[6] = reg[7] = 0;
	WSync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0x9FFF, UNLKS7037Write);
	SetWriteHandler(0xA000, 0xBFFF, CartBW);
	SetWriteHandler(0xC000, 0xFFFF, UNLKS7037Write);
}

static void LH10Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = reg[4] = reg[5] = reg[6] = reg[7] = 0;
	WSync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xBFFF, UNLKS7037Write);
	SetWriteHandler(0xC000, 0xDFFF, CartBW);
	SetWriteHandler(0xE000, 0xFFFF, UNLKS7037Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void StateRestore(int version) {
	cmd &= 7;	/* reg[] has 8 entries; clamp savestate value */
	WSync();
}

void UNLKS7037_Init(CartInfo *info) {
	info->Power = UNLKS7037Power;
	info->Close = Close;

	WSync = SyncKS7037;

	WRAMSIZE = 8192;
	WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}

void LH10_Init(CartInfo *info) {
	info->Power = LH10Power;
	info->Close = Close;

	WSync = SyncLH10;

	WRAMSIZE = 8192;
	WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
=======
void Mapper307_Init(CartInfo *info) {
	N118_Init(info, 8, info->battery);
	info->Power = M307Power;
	N118_FixPRG = M307FixPRG;
	N118_FixCHR = M307FixCHR;
>>>>>>> e98261e5 (Update ppu.c)
}
