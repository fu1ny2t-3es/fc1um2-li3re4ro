/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2025 NewRisingSun
=======
 *  Copyright (C) 2012 CaH4e3
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> 4096c502 (Update libretro.c)
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

#include "mapinc.h"

<<<<<<< HEAD
static uint8_t submapper, initialMirroring;
static uint8_t reg[3];
static uint8_t regIndex, cycleCount, fetchCount;

<<<<<<< HEAD
static void sync () {
	int prg, chr;
	if (reg[0] &0x40) { /* NINA-03 */
		prg = reg[0] &0x0E | reg[2] &0x01;
		chr = reg[0] <<2 &0x38 | reg[2] >>4 &0x07;
	} else { /* CNROM */
		prg = reg[0] &0x0F;
		chr = reg[0] <<2 &0x3C | reg[2] >>4 &0x03;
	}
	if (~reg[0] &0x10) {
		prg |= reg[0] >>1 &0x10;
		chr |= reg[0] <<1 &0x40;
	}
	prg ^= 0x10;
	chr ^= 0x40;
	if (prg &0x10) prg &= (ROM_size -1) >>1;
	if (chr &0x40) chr &= VROM_size -1;
	setprg32(0x8000, prg);
	setchr8(chr);
	setmirror(reg[0] &0x80? MI_H: MI_V);
}

static DECLFR (readRegF_submapper0) {
	uint8_t result = CartBR(A);
	if (A >= 0xFFE8 && A < 0xFFF8) {
		reg[2] = result;
		sync();
	} else
	if (A >= 0xFFC0 && A < 0xFFE0 && !(reg[0] &0x3F)) {
		reg[1] = result;
		sync();
	} else
	if (A >= 0xFF80 && A < 0xFFA0 && !(reg[0] &0x3F)) {
		reg[0] = result;
		sync();
	}
	return result;
}

static DECLFR (readReg4_submapper1) {
	/* Reading from 41xx selects a register for the subsequent read within five CPU cycles.
	   However, because FCEU only executes the MapIRQHook after each instruction, this doesn't work, so it is necessary to count both cycle and fetches
	   to account for switching code that executes from RAM and from ROM. */
	regIndex = A &0xFF;
	cycleCount = 5;
	fetchCount = 4;
	return X.DB;
}

static DECLFR (readReg8_submapper1) {
	uint8_t result = CartBR(A);
	if (cycleCount && fetchCount--) switch(regIndex &0xC0) {
		case 0x00:
			reg[2] = result;
			sync();
			break;
		case 0x40:
			reg[1] = result;
			sync();
			break;
		case 0x80:
			reg[0] = result;
			regIndex = 0xFF;
			sync();
			break;			
	}
	return result;
}

static void FP_FASTAPASS(1) cpuCycle_submapper1 (int a) {
	while (a--) {
		if (cycleCount) cycleCount--;
	}
}

static void reset () {
	reg[0] = reg[1] = reg[2] = cycleCount = fetchCount = 0;
	regIndex = 0xFF;
	if (ROM_size <= 4) { /* Extracts don't set up the mirroring and mode registers by themselves, so fill up these registers based on iNES header data */
		reg[0] = 0x3F;
		if (~initialMirroring &1) reg[0] |= 0x80;
		if (ROM_size == 4 || VROM_size >= 8) reg[0] |= 0x40;
	}
	sync();
}

static void power () {
	if (submapper == 1) {
		SetReadHandler(0x4100, 0x41FF, readReg4_submapper1);
		SetReadHandler(0x8000, 0xFFFF, readReg8_submapper1);
=======
static void M234Sync(void) {
	if (bank & 0x40) {
		setprg32(0x8000, (bank & 0xE) | (preg & 1));
		setchr8(((bank & 0xE) << 2) | ((preg >> 4) & 7));
>>>>>>> 563724c2 (Update libretro.c)
	} else {
		SetReadHandler(0x8000, 0xEFFF, CartBR);
		SetReadHandler(0xF000, 0xFFFF, readRegF_submapper0);
	}
	reset();
}

<<<<<<< HEAD
static void stateRestore (int version) {
	sync();
}

void Mapper234_Init (CartInfo *info) {
	submapper = info->submapper;
	initialMirroring = info->mirror;
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = stateRestore;
	AddExState(reg, 3, 0, "REGS");
	AddExState(&regIndex, 1, 0, "RIDX");
	AddExState(&cycleCount, 1, 0, "CYCC");
	AddExState(&fetchCount, 1, 0, "FETC");
	if (submapper == 1) MapIRQHook = cpuCycle_submapper1;
=======
=======
<<<<<<< HEAD
static uint8_t bank, preg;
static SFORMAT StateRegs[] =
{
	{ &bank, 1, "BANK" },
	{ &preg, 1, "PREG" },
=======
static uint8 reg[3];

static SFORMAT StateRegs[] = {
	{ reg, 3, "REGS" },
>>>>>>> 7b06ddc4 (Update libretro.c)
	{ 0 }
};

static void Sync(void) {
	if (reg[0] & 0x40) {
		setprg32(0x8000, (reg[0] & 0x0E) | (reg[1] & 0x01));
		setchr8(((reg[0] & 0x0E) << 2) | ((reg[1] >> 4) & 0x07));
	} else {
		setprg32(0x8000, reg[0] & 0x0F);
		setchr8(((reg[0] & 0x0F) << 2) | ((reg[1] >> 4) & 0x03));
	}
	setmirror((reg[0] >> 7) ^ 0x01);
}

<<<<<<< HEAD
>>>>>>> 4096c502 (Update libretro.c)
<<<<<<< HEAD
static DECLFR(M234ReadBank) {
	uint8_t r = CartBR(A);
=======
uint8 M234ReadBank(uint32 A) {
	uint8 r = CartBR(A);
>>>>>>> 74114ad0 (Update libretro.c)
	if (!bank) {
		bank = r;
		M234Sync();
	}
	return r;
}

<<<<<<< HEAD
static DECLFR(M234ReadPreg) {
	uint8_t r = CartBR(A);
=======
uint8 M234ReadPreg(uint32 A) {
	uint8 r = CartBR(A);
>>>>>>> 74114ad0 (Update libretro.c)
	preg = r;
	M234Sync();
	return r;
=======
static DECLFR(M234Read) {
	uint8 ret = CartBR(A);

	switch (A & 0xFFF8) {
	case 0xFF80:
	case 0xFF88:
	case 0xFF90:
	case 0xFF98:
		if (!reg[0]) {
			reg[0] = ret;
			Sync();
		}
		break;
	case 0xFFC0:
	case 0xFFC8:
	case 0xFFD0:
	case 0xFFD8:
		if (!reg[0]) {
			reg[2] = ret;
			Sync();
		}
		break;
	case 0xFFE8:
	case 0xFFF0:
		reg[1] = ret;
		Sync();
		break;
	}

	return ret;
>>>>>>> 7b06ddc4 (Update libretro.c)
}

static void M234Reset(void) {
	reg[0] = reg[1] = reg[2] = 0;
	Sync();
}

static void M234Power(void) {
	reg[0] = reg[1] = reg[2] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0xFF80, 0xFFFF, M234Read);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper234_Init(CartInfo *info) {
	info->Power = M234Power;
	info->Reset = M234Reset;
<<<<<<< HEAD
	AddExState(&StateRegs, ~0, 0, 0);
	GameStateRestore = M234StateRestore;
>>>>>>> 563724c2 (Update libretro.c)
=======
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
>>>>>>> 4096c502 (Update libretro.c)
}
