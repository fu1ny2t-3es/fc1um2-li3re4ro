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
 */

<<<<<<< HEAD
/* 2020-2-3 - updated mapper 150/243 */

#include "mapinc.h"

static uint8_t cmd, dip;
static uint8_t latch[8];
static uint8_t mapperNum;

static int type;
static void S8259Synco(void) {
	int x;
	setprg32(0x8000, latch[5] & 7);

	if (!UNIFchrrama) {		/* No CHR RAM?  Then BS'ing is ok. */
		for (x = 0; x < 4; x++) {
			int bank;
			if (latch[7] & 1)
				bank = (latch[0] & 0x7) | ((latch[4] & 7) << 3);
			else
				bank = (latch[x] & 0x7) | ((latch[4] & 7) << 3);
			switch (type) {
			case 00: bank = (bank << 1) | (x & 1); setchr2(0x800 * x, bank); break;
			case 01: setchr2(0x800 * x, bank); break;
			case 02: bank = (bank << 2) | (x & 3); setchr2(0x800 * x, bank); break;
			case 03:
				bank = latch[x] & 7;
				switch (x & 3) {
				case 01: bank |= (latch[4] & 1) << 4; break;
				case 02: bank |= (latch[4] & 2) << 3; break;
				case 03: bank |= ((latch[4] & 4) << 2) | ((latch[6] & 1) << 3); break;
				}
				setchr1(0x400 * x, bank);
				setchr4(0x1000, ~0);
				break;
			}
		}
	}
	if (latch[7] &1)
		setmirror(MI_V);
	else
	switch(latch[7] >>1 &3) {
		case 0: setmirror(mapperNum == 137? MI_H: MI_V); break;
		case 1: setmirror(mapperNum == 137? MI_V: MI_H); break;
		case 2: setmirrorw(0, 1, 1, 1); break;
		case 3: setmirror(MI_0); break;
	}
}

static void S8259Write(uint32 A, uint8 V) {
	A &= 0x4101;
	if (A == 0x4100)
		cmd = V;
	else {
		latch[cmd & 7] = V;
		S8259Synco();
	}
}

static void S8259Reset(void) {
	int x;
	cmd = 0;

	for (x = 0; x < 8; x++) latch[x] = 0;
	setchr8(0);

	S8259Synco();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4100, 0x7FFF, S8259Write);
}

static void S8259Restore(int version) {
	cmd &= 7;	/* latch[] has 8 entries; clamp savestate value */
	S8259Synco();
}

void S8259A_Init(CartInfo *info) {	/* Kevin's Horton 141 mapper */
	mapperNum = info->mapper;
	info->Power = S8259Reset;
	GameStateRestore = S8259Restore;
	AddExState(latch, 8, 0, "LATC");
	AddExState(&cmd, 1, 0, "CMD");
	type = 0;
}

void S8259B_Init(CartInfo *info) {	/* Kevin's Horton 138 mapper */
	mapperNum = info->mapper;
	info->Power = S8259Reset;
	GameStateRestore = S8259Restore;
	AddExState(latch, 8, 0, "LATC");
	AddExState(&cmd, 1, 0, "CMD");
	type = 1;
}

void S8259C_Init(CartInfo *info) {	/* Kevin's Horton 139 mapper */
	mapperNum = info->mapper;
	info->Power = S8259Reset;
	GameStateRestore = S8259Restore;
	AddExState(latch, 8, 0, "LATC");
	AddExState(&cmd, 1, 0, "CMD");
	type = 2;
}

void S8259D_Init(CartInfo *info) {	/* Kevin's Horton 137 mapper */
	mapperNum = info->mapper;
	info->Power = S8259Reset;
	GameStateRestore = S8259Restore;
	AddExState(latch, 8, 0, "LATC");
	AddExState(&cmd, 1, 0, "CMD");
	type = 3;
}

static void (*WSync)(void);

static void SAWrite(uint32 A, uint8 V) {
	if (A & 0x100) {
		latch[0] = V;
		WSync();
	}
}

static void SAPower(void) {
	latch[0] = 0;
	WSync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4100, 0x5FFF, SAWrite);
}

static void SARestore(int version) {
	WSync();
}

static void SADWrite(uint32 A, uint8 V) {
	latch[0] = V;
	WSync();
}

static void SADPower(void) {
	latch[0] = 0;
	WSync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, SADWrite);
}

static void SA0161MSynco(void) {
	setprg32(0x8000, (latch[0] >> 3) & 1);
	setchr8(latch[0] & 7);
}

static void SA72007Synco(void) {
	setprg32(0x8000, 0);
	setchr8(latch[0] >> 7);
}

static void SA009Synco(void) {
	setprg32(0x8000, 0);
	setchr8(latch[0] & 1);
}

static void SA72008Synco(void) {
	setprg32(0x8000, (latch[0] >> 2) & 1);
	setchr8(latch[0] & 3);
}

void SA0161M_Init(CartInfo *info) {
	WSync = SA0161MSynco;
	GameStateRestore = SARestore;
	info->Power = SAPower;
	AddExState(&latch[0], 1, 0, "LATC");
}

void SA72007_Init(CartInfo *info) {
	WSync = SA72007Synco;
	GameStateRestore = SARestore;
	info->Power = SAPower;
	AddExState(&latch[0], 1, 0, "LATC");
}

void SA72008_Init(CartInfo *info) {
	WSync = SA72008Synco;
	GameStateRestore = SARestore;
	info->Power = SAPower;
	AddExState(&latch[0], 1, 0, "LATC");
}

void SA009_Init(CartInfo *info) {
	WSync = SA009Synco;
	GameStateRestore = SARestore;
	info->Power = SAPower;
	AddExState(&latch[0], 1, 0, "LATC");
}

void SA0036_Init(CartInfo *info) {
	WSync = SA72007Synco;
	GameStateRestore = SARestore;
	info->Power = SADPower;
	AddExState(&latch[0], 1, 0, "LATC");
}

void SA0037_Init(CartInfo *info) {
	WSync = SA0161MSynco;
	GameStateRestore = SARestore;
	info->Power = SADPower;
	AddExState(&latch[0], 1, 0, "LATC");
}

/* --------------------------------------------- */

<<<<<<< HEAD
static DECLFR(TCA01Read) {
	uint8_t ret;
=======
static uint8 TCA01Read(uint32 A) {
>>>>>>> b8aebecd (Update libretro.c)
	if ((A & 0x4100) == 0x4100)
		return (cpu.openbus & 0xC0) | ((~A) & 0x3F);
	return cpu.openbus;
}

static void TCA01Power(void) {
	setprg16(0x8000, 0);
	setprg16(0xC000, 1);
	setchr8(0);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4100, 0x5FFF, TCA01Read);
}

void TCA01_Init(CartInfo *info) {
	info->Power = TCA01Power;
}

/* ------------------ Mapper 150 --------------------- */
/* ------------------ Mapper 243 --------------------- */

=======
>>>>>>> dffcc139 (Update libretro.c)
/* Mapper 150 - SA-015 / SA-630 / Unif UNL-Sachen-74LS374N */
/* Mapper 243 - SA-020A */

<<<<<<< HEAD
static void S74LS374NSynco(void) {
	uint32_t chrBank;
	if (mapperNum == 150)
		chrBank = (latch[6] & 3) | ((latch[4] << 2) & 4) | (latch[2] << 3);
	else
		chrBank = (latch[2] & 1) | ((latch[4] << 1) & 2) | (latch[6] << 2);
=======
#include "mapinc.h"
>>>>>>> 5926d713 (Update libretro.c)

static uint8 dipsw;
static uint8 cmd;
static uint8 reg[8];

static SFORMAT StateRegs[] = {
	{ reg, 8, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ &cmd, 1, "CMD0" },
	{ 0 }
};

static void Sync(void) {
	setprg32(0x8000, (reg[2] & 0x01) | reg[5]);
	if (iNESCart.mapper == 243) {
        setchr8((reg[2] & 0x01) | ((reg[4] << 1) & 0x02) | (reg[6] << 2));
    } else {
        setchr8((reg[6] & 0x03) | ((reg[4] << 2) & 0x04) | (reg[2] << 3));
    }
	switch ((reg[7] >> 1) & 0x03) {
	case 0: setmirrorw(0, 1, 1, 1); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_V); break;
	case 3: setmirror(MI_0); break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static DECLFR(S74LS374NRead) {
	uint8_t ret;
=======
static uint8 S74LS374NRead(uint32 A) {
>>>>>>> b8aebecd (Update libretro.c)
	if ((A & 0xC101) == 0x4101) {
		if (dip & 1)
			return (latch[cmd] & 3) | (cpu.openbus & 0xFC);
		return (latch[cmd] & 7) | (cpu.openbus & 0xF8);
=======
static DECLFR(M150Read) {
	if ((A & 0x101) == 0x101) {
		if (dipsw & 1)
			return (reg[cmd] & 0x03) | (cpu.openbus & 0xFC);
		else
			return (reg[cmd] & 0x07) | (cpu.openbus & 0xF8);
>>>>>>> 5926d713 (Update libretro.c)
	}
	return cpu.openbus;
}

static DECLFW(M150Write) {
	if (dipsw & 0x01)
		V |= 0x04;
	switch (A & 0x101) {
	case 0x100:
		cmd = V & 0x07;
		break;
	case 0x101:
		reg[cmd] = V & 0x07;
		Sync();
		break;
	}
}

<<<<<<< HEAD
static void S74LS374NRestore(int version) {
	cmd &= 7;	/* latch[] has 8 entries; clamp savestate value */
	S74LS374NSynco();
=======
static void M150Restore(int version) {
	Sync();
>>>>>>> 5926d713 (Update libretro.c)
}

static void M150Reset(void) {
	dipsw ^= 0x01;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	reg[4] = reg[5] = reg[6] = reg[7] = 0;
	Sync();
}

static void M150Power(void) {
	dipsw = 0;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	reg[4] = reg[5] = reg[6] = reg[7] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
    SetReadHandler(0x4100, 0x5FFF, M150Read);
	SetWriteHandler(0x4100, 0x5FFF, M150Write);
}

void Mapper150_Init(CartInfo *info) {
	info->Power = M150Power;
	info->Reset = M150Reset;
	GameStateRestore = M150Restore;
	AddExState(StateRegs, ~0, 0, NULL);
}
